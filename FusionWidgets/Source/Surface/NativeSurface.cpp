#include "Fusion/Widgets.h"

namespace Fusion
{
	FNativeSurface::FNativeSurface(FWindowHandle windowHandle, FObject* outer) : m_WindowHandle(windowHandle)
	{
		
	}

	void FNativeSurface::Initialize()
	{
		Super::Initialize();

		if (Ref<FApplicationInstance> application = GetApplication())
		{
			m_RenderTarget = application->AcquireWindowRenderTarget(m_WindowHandle);

			m_DpiScale = application->GetDpiScaleForWindow(m_WindowHandle);

			FVec2i pixelSize = application->GetWindowSizeInPixels(m_WindowHandle);

			m_AvailableSize = FVec2((int)pixelSize.x, (int)pixelSize.y) / m_DpiScale;
		}
	}

	void FNativeSurface::Shutdown()
	{
		Super::Shutdown();
	}

	void FNativeSurface::OnWindowResized()
	{
		if (Ref<FApplicationInstance> application = GetApplication())
		{
			FVec2i pixelSize = application->GetWindowSizeInPixels(m_WindowHandle);

			FVec2 availableSize = FVec2((int)pixelSize.x, (int)pixelSize.y) / m_DpiScale;

			if (m_AvailableSize != availableSize)
			{
				m_AvailableSize = availableSize;

				OnSurfaceResize();
			}
		}
	}

} // namespace Fusion
