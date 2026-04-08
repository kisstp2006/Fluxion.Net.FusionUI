#include "Fusion/VulkanRHI.h"

namespace Fusion::Vulkan
{
	// -----------------------------------------------------------------
	// Texture
	// -----------------------------------------------------------------

	FTexture::~FTexture()
	{
		VkImage image = !m_IsExternalImage ? m_Image : VK_NULL_HANDLE;
		VkImageView imageView = m_ImageView;
		VkDevice device = m_Device;
		FVulkanRenderBackend* backend = m_RenderBackend;

		if (imageView || image || !m_Allocations.Empty())
		{
			auto allocations = m_Allocations;

			m_RenderBackend->DeferDestruction([image, imageView, device, allocations, backend]
				{
					if (imageView)
						vkDestroyImageView(device, imageView, VULKAN_CPU_ALLOCATOR);
					if (image)
						vkDestroyImage(device, image, VULKAN_CPU_ALLOCATOR);

					for (VmaAllocation allocation : allocations)
					{
						vmaFreeMemory(backend->GetVmaAllocator(), allocation);
					}
				});
		}

		m_IsExternalImage = false;
		m_Image = VK_NULL_HANDLE;
		m_ImageView = VK_NULL_HANDLE;
	}

	// -----------------------------------------------------------------
	// Texture Atlas
	// -----------------------------------------------------------------

	FTextureAtlas::FTextureAtlas(FVulkanRenderBackend* backend, u32 size, u32 layerCount, VkFormat format, int imageCount)
		: m_Backend(backend), m_Device(backend->GetVkDevice()), m_Size(size), m_LayerCount(layerCount), m_Format(format)
	{
		FUSION_ASSERT(layerCount > 0, "FTextureAtlas: invalid layerCount");
		FUSION_ASSERT(size > 0, "FTextureAtlas: invalid size");

		m_SparseResidency = backend->GetFeatures().sparseResidencyImage2D;

		m_ImagesPerFrame.Resize(imageCount);

		// Image CI

		m_ImageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		m_ImageCI.format = format;
		m_ImageCI.imageType = VK_IMAGE_TYPE_2D;
		m_ImageCI.extent.depth = 1.0f;
		m_ImageCI.extent.width = size;
		m_ImageCI.extent.height = size;
		m_ImageCI.mipLevels = 1;
		m_ImageCI.arrayLayers = layerCount;
		m_ImageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_ImageCI.flags = m_SparseResidency ? (VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT | VK_IMAGE_CREATE_SPARSE_BINDING_BIT) : 0;
		
		// Image View CI

		m_ImageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		m_ImageViewCI.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		m_ImageViewCI.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		m_ImageViewCI.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		m_ImageViewCI.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		m_ImageViewCI.format = format;
		m_ImageViewCI.subresourceRange.baseArrayLayer = 0;
		m_ImageViewCI.subresourceRange.layerCount = layerCount;
		m_ImageViewCI.subresourceRange.baseArrayLayer = 0;
		m_ImageViewCI.subresourceRange.levelCount = 1;
		m_ImageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		m_ImageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;

		// Per Frame

		VkFenceCreateInfo fenceCI{};
		fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		FArray<VkFence> fences{};

		for (int i = 0; i < imageCount; i++)
		{
			VkImage image = VK_NULL_HANDLE;
			VkImageView imageView = VK_NULL_HANDLE;
			VmaAllocation allocation = VK_NULL_HANDLE;
			
			VkResult result = vkCreateImage(m_Device, &m_ImageCI, VULKAN_CPU_ALLOCATOR, &image);
			VULKAN_CHECK(result, "Failed to create VkImage.");

			m_ImageViewCI.image = image;
			result = vkCreateImageView(m_Device, &m_ImageViewCI, VULKAN_CPU_ALLOCATOR, &imageView);
			VULKAN_CHECK(result, "Failed to create VkImageView.");

			if (m_SparseResidency)
			{
				uint32_t count = 0;
				vkGetImageSparseMemoryRequirements(m_Device, image, &count, nullptr);
				FArray<VkSparseImageMemoryRequirements> sparseReqs(count);
				vkGetImageSparseMemoryRequirements(m_Device, image, &count, sparseReqs.Data());

				m_ImageGranularity = sparseReqs[0].formatProperties.imageGranularity;

				VkDeviceSize blocksX = size / m_ImageGranularity.width;
				VkDeviceSize blocksY = size / m_ImageGranularity.height;
				VkDeviceSize blockByteSize = (VkDeviceSize)m_ImageGranularity.width * m_ImageGranularity.height * GetFormatBytesPerTexel(format);

				VkMemoryRequirements imageReqs{};
				vkGetImageMemoryRequirements(m_Device, image, &imageReqs);

				imageReqs.size = blocksX * blocksY * blockByteSize;

				m_AllocCI.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
				m_AllocCI.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

				VmaAllocationInfo allocationInfo{};
				result = vmaAllocateMemory(m_Backend->GetVmaAllocator(), &imageReqs, &m_AllocCI, &allocation, &allocationInfo);
				VULKAN_CHECK(result, "Failed to allocate Vma Memory.");

				VkFence fence = VK_NULL_HANDLE;
				vkCreateFence(m_Device, &fenceCI, VULKAN_CPU_ALLOCATOR, &fence);
				fences.Add(fence);

				FArray<VkSparseImageMemoryBind> imageMemoryBindings{};
				imageMemoryBindings.Reserve(blocksX * blocksY);

				for (int x = 0; x < blocksX; x++)
				{
					for (int y = 0; y < blocksY; y++)
					{
						VkSparseImageMemoryBind bind{};
						bind.memory = allocationInfo.deviceMemory;
						bind.subresource.arrayLayer = 0;
						bind.subresource.mipLevel = 0;
						bind.subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						bind.memoryOffset = (VkDeviceSize)(x + y * blocksX) * blockByteSize;
						bind.extent = m_ImageGranularity;
						bind.offset.x = (int32_t)m_ImageGranularity.width * x;
						bind.offset.y = (int32_t)m_ImageGranularity.height * y;
						bind.offset.z = 0;

						imageMemoryBindings.Add(bind);
					}
				}

				VkSparseImageMemoryBindInfo imageBindInfo{};
				imageBindInfo.bindCount = imageMemoryBindings.Size();
				imageBindInfo.pBinds = imageMemoryBindings.Data();
				imageBindInfo.image = image;

				VkBindSparseInfo bindInfo{};
				bindInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
				bindInfo.bufferBindCount = 0;
				bindInfo.imageBindCount = 1;
				bindInfo.pImageBinds = &imageBindInfo;
				bindInfo.imageOpaqueBindCount = 0;

				vkQueueBindSparse(m_Backend->GetGraphicsQueue(), 1, &bindInfo, fence);

				m_ImagesPerFrame[i] = new FTexture(m_Backend, m_Device);
				m_ImagesPerFrame[i]->m_Image = image;
				m_ImagesPerFrame[i]->m_ImageView = imageView;
				m_ImagesPerFrame[i]->m_Allocations.Add(allocation);
				m_ImagesPerFrame[i]->m_AllocationInfos.Add(allocationInfo);
			}
			else
			{
				// TODO
			}
		}

		if (!fences.Empty())
		{
			vkWaitForFences(m_Device, fences.Size(), fences.Data(), VK_TRUE, FNumericLimits<uint64_t>::Max());

			for (VkFence fence : fences)
			{
				vkDestroyFence(m_Device, fence, VULKAN_CPU_ALLOCATOR);
			}
		}
		fences.Clear();
	}

	FTextureAtlas::~FTextureAtlas()
	{
		m_ImagesPerFrame.Clear();
	}

} // namespace Fusion::Vulkan
