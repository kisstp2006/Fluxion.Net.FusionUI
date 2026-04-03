#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
	class FWidget;
	class FSurface;
	class FNativeSurface;

    struct FApplicationInstanceDesc
    {
		IFPlatformBackend* platformBackend = nullptr;
		IFRenderBackend* renderBackend = nullptr;
	};

	class FUSIONWIDGETS_API FApplicationInstance : public FObject, public IFPlatformEventSink
    {
        FUSION_CLASS(FApplicationInstance, FObject)
    public:

        FApplicationInstance(FName name) : FObject(name)
        {
	        
        }

		// - Public API -

		f32 GetDpiScaleForWindow(FWindowHandle windowHandle);

		// - Lifecycle -

		bool Initialize(const FApplicationInstanceDesc& desc);

		void Shutdown();

		void Tick();

		// - Surface -

		Ref<FNativeSurface> CreateNativeSurfaceForWindow(FWindowHandle window);

		FInstanceHandle GetInstanceHandle() const { return m_InstanceHandle; }

		FRenderCapabilities GetRenderCapabilities() const { return m_RenderCapabilities; }

		// - Render Target -

		FRenderTargetHandle AcquireWindowRenderTarget(FWindowHandle window);

		void ReleaseRenderTarget(FRenderTargetHandle renderTarget);

		void SubmitSnapshot(FRenderTargetHandle renderTarget, IntrusivePtr<FRenderSnapshot> snapshot);

	protected:

		void OnWindowDestroyed(FWindowHandle window) override;

		FInstanceHandle m_InstanceHandle = FInstanceHandle::NullValue;

		FArray<Ref<FSurface>> m_Surfaces;

		FHashMap<FWindowHandle, Ref<FNativeSurface>> m_NativeSurfacesByWindow;

    private:

		bool m_PlatformBackendAllocated = false;
		IFPlatformBackend* m_PlatformBackend = nullptr;

		bool m_RenderBackendAllocated = false;
		IFRenderBackend* m_RenderBackend = nullptr;

		FRenderCapabilities m_RenderCapabilities{};
    };
    
} // namespace Fusion
