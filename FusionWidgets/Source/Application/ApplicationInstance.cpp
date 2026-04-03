#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
	f32 FApplicationInstance::GetDpiScaleForWindow(FWindowHandle windowHandle)
	{
		return m_PlatformBackend->GetDpiScaleForWindow(windowHandle);
	}

	bool FApplicationInstance::Initialize(const FApplicationInstanceDesc& desc)
	{
		m_PlatformBackend = desc.platformBackend;
		
#if FUSION_USE_SDL3
		if (!m_PlatformBackend)
		{
			m_PlatformBackendAllocated = true;
			m_PlatformBackend = new FSDL3PlatformBackend();
		}
#endif

		if (!m_PlatformBackend)
		{
			FUSION_LOG_ERROR("Backend", "No platform backend specified and no default available.");
			return false;
		}

		m_RenderBackend = desc.renderBackend;

#if FUSION_USE_VULKAN
		if (!m_RenderBackend)
		{
			m_RenderBackendAllocated = true;
			m_RenderBackend = new Vulkan::FVulkanRenderBackend(m_PlatformBackend);
		}
#endif

		if (!m_RenderBackend)
		{
			FUSION_LOG_ERROR("Backend", "No render backend specified and no default available.");
			return false;
		}

		m_InstanceHandle = m_PlatformBackend->InitializeInstance();

		if (m_InstanceHandle.IsNull())
		{
			FUSION_LOG_ERROR("Backend", "Failed to initialize platform backend instance.");
			return false;
		}

		m_PlatformBackend->SetInstanceEventSink(m_InstanceHandle, this);

		m_RenderBackend->InitializeInstance(m_InstanceHandle);

		m_RenderCapabilities = m_RenderBackend->GetRenderCapabilities();
		FUSION_ASSERT(m_RenderCapabilities.MinStructuredBufferOffsetAlignment > 0, "Invalid value for MinStructuredBufferOffsetAlignment.");

		return true;
	}

	void FApplicationInstance::Shutdown()
	{
		if (m_RenderBackend)
		{
			m_RenderBackend->ShutdownInstance(m_InstanceHandle);
		}

		if (m_PlatformBackend)
		{
			m_PlatformBackend->ShutdownInstance(m_InstanceHandle);
		}

		m_InstanceHandle = FInstanceHandle::NullValue;

		if (m_PlatformBackendAllocated)
		{
			delete m_PlatformBackend; m_PlatformBackend = nullptr;
		}

		if (m_RenderBackendAllocated)
		{
			delete m_RenderBackend; m_RenderBackend = nullptr;
		}

		m_PlatformBackendAllocated = m_RenderBackendAllocated = false;
	}

	void FApplicationInstance::Tick()
	{
		for (Ref<FSurface> surface : m_Surfaces)
		{
			surface->TickSurface();
		}
	}

	Ref<FNativeSurface> FApplicationInstance::CreateNativeSurfaceForWindow(FWindowHandle window)
	{
		Ref<FNativeSurface> nativeSurface = new FNativeSurface(window, this);
		nativeSurface->m_Application = Ref(this);

		m_NativeSurfacesByWindow[window] = nativeSurface;

		m_Surfaces.Add(nativeSurface);

		nativeSurface->Initialize();

		return nativeSurface;
	}

	FRenderTargetHandle FApplicationInstance::AcquireWindowRenderTarget(FWindowHandle window)
	{
		return m_RenderBackend->AcquireWindowRenderTarget(window);
	}

	void FApplicationInstance::ReleaseRenderTarget(FRenderTargetHandle renderTarget)
	{
		m_RenderBackend->ReleaseRenderTarget(renderTarget);
	}

	void FApplicationInstance::SubmitSnapshot(FRenderTargetHandle renderTarget, IntrusivePtr<FRenderSnapshot> snapshot)
	{
		m_RenderBackend->SubmitSnapshot(renderTarget, snapshot);
	}

	void FApplicationInstance::OnWindowDestroyed(FWindowHandle window)
	{
		if (!m_NativeSurfacesByWindow.KeyExists(window))
			return;

		Ref<FNativeSurface> nativeSurface = m_NativeSurfacesByWindow[window];
		if (!nativeSurface)
			return;

		nativeSurface->Shutdown();

		m_NativeSurfacesByWindow.Remove(window);
		m_Surfaces.Remove(nativeSurface);
	}

} // namespace Fusion
