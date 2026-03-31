#include "VulkanRHIPrivate.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#include "PAL/VulkanPlatform.h"

namespace Fusion
{
	VKAPI_ATTR static VkBool32 VulkanValidationCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			FUSION_LOG_ERROR("VulkanValidation", "Vulkan Error: {}", pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			FUSION_LOG_WARNING("VulkanValidation", "Vulkan Warning: {}", pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			FUSION_LOG_INFO("VulkanValidation", "Vulkan Info: {}", pCallbackData->pMessage);
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			FUSION_LOG_TRACE("VulkanValidation", "Vulkan Verbose: {}", pCallbackData->pMessage);
		}
		return VK_FALSE;
	}


	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = VulkanValidationCallback;
	}

	FRenderCapabilities FVulkanRenderBackend::GetRenderCapabilities()
	{
		// TODO
		return {

		};
	}

	bool FVulkanRenderBackend::InitializeInstance(FInstanceHandle instance)
	{
		if (instances.KeyExists(instance))
		{
			return true;
		}

		if (m_VulkanInstance == VK_NULL_HANDLE)
		{
			InitializeVulkan();
		}

		IntrusivePtr<FRenderInstance> renderInstance = new FRenderInstance();
		


		instances.Add(instance, renderInstance);

		return true;
	}

	void FVulkanRenderBackend::ShutdownInstance(FInstanceHandle instance)
	{
		if (!instances.KeyExists(instance))
		{
			return;
		}

		instances.Remove(instance);

		if (instances.IsEmpty())
		{
			ShutdownVulkan();
		}
	}

	// ------------------------------------------------------------------------------------------------------------
	// - Vulkan
	// ------------------------------------------------------------------------------------------------------------

	void FVulkanRenderBackend::InitializeVulkan()
	{
		// - Instance -

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Fusion Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Fusion Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pNext = nullptr;

		FArray<const char*> requiredExtensions = FVulkanPlatform::GetRequiredVulkanInstanceExtensions();
		FArray<const char*> requiredLayers = FVulkanPlatform::GetRequiredInstanceLayers();

		VkInstanceCreateInfo instanceCI{};
		instanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCI.flags = 0;
		instanceCI.pNext = nullptr;

		instanceCI.pApplicationInfo = &appInfo;
		
		instanceCI.enabledLayerCount = requiredLayers.Size();
		instanceCI.ppEnabledLayerNames = requiredLayers.Data();

		instanceCI.enabledExtensionCount = requiredExtensions.Size();
		instanceCI.ppEnabledExtensionNames = requiredExtensions.Data();

		VkDebugUtilsMessengerCreateInfoEXT debugCI{};
		debugCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCI.pNext = nullptr;
		debugCI.flags = 0;

		if (FVulkanPlatform::IsValidationEnabled())
		{
			PopulateDebugMessengerCreateInfo(debugCI);
			instanceCI.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCI;
		}

		auto result = vkCreateInstance(&instanceCI, VULKAN_CPU_ALLOCATOR, &m_VulkanInstance);
		FUSION_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan instance: {}", result);

		if (FVulkanPlatform::IsValidationEnabled())
		{
			result = CreateDebugUtilsMessengerEXT(m_VulkanInstance, &debugCI, VULKAN_CPU_ALLOCATOR, &m_VkMessenger);
			FUSION_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan debug messenger: {}", result);
		}


		// - Physical Device -

		// Fetch all available physical devices
		u32 physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(m_VulkanInstance, &physicalDeviceCount, nullptr);
		FArray<VkPhysicalDevice> physicalDevices{ physicalDeviceCount };
		vkEnumeratePhysicalDevices(m_VulkanInstance, &physicalDeviceCount, physicalDevices.Data());

		if (physicalDeviceCount == 0)
		{
			FUSION_LOG_ERROR("Vulkan", "Failed to find any Vulkan-compatible physical devices.");
		}

		if (physicalDeviceCount == 1)
		{
			m_PhysicalDevice = physicalDevices[0];
		}
		else
		{
			for (int i = 0; i < physicalDevices.Size(); ++i)
			{
				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

				if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					m_PhysicalDevice = physicalDevices[i];
					break;
				}
			}
		}

		FUSION_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable Vulkan physical device.");

		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties);

		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_PhysicalDeviceMemoryProperties);

		m_IsUnifiedMemory = true;
		m_IsResizableBAR = false;

		for (int i = 0; i < m_PhysicalDeviceMemoryProperties.memoryHeapCount; i++)
		{
			if (m_PhysicalDeviceMemoryProperties.memoryHeaps[i].flags != VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
			{
				m_IsUnifiedMemory = false;
			}
		}


		// - Surface -

		FVulkanTempSurface tempSurfaceData = FVulkanPlatform::CreateTempSurface(m_VulkanInstance);
		{
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, tempSurfaceData.tempSurface, &m_SurfaceCapabilities);

			uint32_t surfaceFormatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, tempSurfaceData.tempSurface, &surfaceFormatCount, nullptr);

			if (surfaceFormatCount > 0)
			{
				m_SurfaceFormats.Resize(surfaceFormatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, tempSurfaceData.tempSurface, &surfaceFormatCount, m_SurfaceFormats.Data());
			}
			
			uint32_t presentModesCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, tempSurfaceData.tempSurface, &presentModesCount, nullptr);

			if (presentModesCount > 0)
			{
				m_PresentationModes.Resize(presentModesCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, tempSurfaceData.tempSurface, &presentModesCount, m_PresentationModes.Data());
			}
		}
		FVulkanPlatform::DestroyTempSurface(m_VulkanInstance, tempSurfaceData);

		FUSION_ASSERT(!m_SurfaceFormats.Empty(), "Failed to find any Surface Formats.");
		FUSION_ASSERT(!m_PresentationModes.Empty(), "Failed to find any Surface Present Modes.");


		// - Queues -

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
		FUSION_ASSERT(queueFamilyCount > 0, "Failed to fetch Queue Family Properties");

		m_QueueFamilyProperties.Resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.Data());

		m_QueueFamilyIndex = -1;
		float queuePriority = 1.0f;

		for (int i = 0; i < queueFamilyCount; i++)
		{
			if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_QueueFamilyIndex = i;
				break;
			}
		}

		FUSION_ASSERT(m_QueueFamilyIndex >= 0, "Failed to find a valid Queue Family.");

		VkDeviceQueueCreateInfo queueCI{};
		queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCI.queueCount = 1;
		queueCI.pQueuePriorities = &queuePriority;
		queueCI.queueFamilyIndex = m_QueueFamilyIndex;
		
		
		// - Device -

		FArray<const char*> deviceExtensionNames{};

		uint32_t deviceExtensionCount = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &deviceExtensionCount, nullptr);
		FArray<VkExtensionProperties> deviceExtensionProperties(deviceExtensionCount);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &deviceExtensionCount, deviceExtensionProperties.Data());

		for (uint32_t i = 0; i < deviceExtensionCount; ++i)
		{
			// Required rule by Vulkan Specs, especially on Apple platform.
			if (strcmp(deviceExtensionProperties[i].extensionName, "VK_KHR_portability_subset") == 0)
			{
				deviceExtensionNames.Add(deviceExtensionProperties[i].extensionName);
			}
		}

		VkDeviceCreateInfo deviceCI{};
		deviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		deviceCI.queueCreateInfoCount = 1;
		deviceCI.pQueueCreateInfos = &queueCI;

		deviceCI.enabledExtensionCount = (uint32_t)deviceExtensionNames.Size();
		deviceCI.ppEnabledExtensionNames = deviceExtensionNames.Empty() ? nullptr : deviceExtensionNames.Data();

		VkPhysicalDeviceFeatures deviceFeaturesToUse{};
		deviceFeaturesToUse.samplerAnisotropy = VK_FALSE;
		
		result = vkCreateDevice(m_PhysicalDevice, &deviceCI, VULKAN_CPU_ALLOCATOR, &m_Device);
		FUSION_ASSERT(result == VK_SUCCESS, "Failed to create VkDevice.");

		vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, 0, &m_GraphicsQueue);

		// - Command Pool -

		VkCommandPoolCreateInfo commandPoolCI{};
		commandPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCI.queueFamilyIndex = m_QueueFamilyIndex;

		result = vkCreateCommandPool(m_Device, &commandPoolCI, VULKAN_CPU_ALLOCATOR, &m_CommandPool);
		FUSION_ASSERT(result == VK_SUCCESS, "Failed to create VkCommandPool");
	}

	void FVulkanRenderBackend::ShutdownVulkan()
	{
		if (m_CommandPool)
		{
			vkDestroyCommandPool(m_Device, m_CommandPool, VULKAN_CPU_ALLOCATOR);
			m_CommandPool = VK_NULL_HANDLE;
		}

		if (m_Device)
		{
			vkDestroyDevice(m_Device, VULKAN_CPU_ALLOCATOR);
			m_Device = VK_NULL_HANDLE;
		}

		if (m_VkMessenger != VK_NULL_HANDLE)
		{
			DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_VkMessenger, VULKAN_CPU_ALLOCATOR);
		}
		m_VkMessenger = VK_NULL_HANDLE;

		vkDestroyInstance(m_VulkanInstance, VULKAN_CPU_ALLOCATOR);
		m_VulkanInstance = VK_NULL_HANDLE;
	}
}
