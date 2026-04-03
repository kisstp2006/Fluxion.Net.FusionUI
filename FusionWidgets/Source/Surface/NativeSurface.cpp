#include "Fusion/Widgets.h"

namespace Fusion
{
	FNativeSurface::FNativeSurface(FWindowHandle windowHandle, FObject* outer) : Super(outer), m_WindowHandle(windowHandle)
	{
		
	}

	void FNativeSurface::Initialize()
	{
		Super::Initialize();

		if (Ref<FApplicationInstance> application = GetApplication())
		{
			m_RenderTarget = application->AcquireWindowRenderTarget(m_WindowHandle);

			m_DpiScale = application->GetDpiScaleForWindow(m_WindowHandle);
		}
	}

	void FNativeSurface::Shutdown()
	{
		Super::Shutdown();
	}
} // namespace Fusion
