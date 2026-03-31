#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#pragma warning(push)
#pragma warning(disable: 4100)

namespace Fusion
{

    class FUSIONPLATFORM_API IFPlatformEventSink
    {
    public:

		virtual ~IFPlatformEventSink() = default;

        virtual void OnExitRequested() {}

        virtual void OnWindowShown(FWindowHandle window) {}
        virtual void OnWindowMaximized(FWindowHandle window) {}
        virtual void OnWindowRestored(FWindowHandle window) {}
        virtual void OnWindowDestroyed(FWindowHandle window) {}
        virtual void OnWindowClosed(FWindowHandle window) {}
        virtual void OnWindowResized(FWindowHandle window, u32 newWidth, u32 newHeight) {}
        virtual void OnWindowMoved(FWindowHandle window, int x, int y) {}
        virtual void OnWindowMinimized(FWindowHandle window) {}
        virtual void OnWindowCreated(FWindowHandle window) {}
        virtual void OnWindowExposed(FWindowHandle window) {}
        virtual void OnWindowDisplayChanged(FWindowHandle window, FDisplayId displayId) {}

        virtual void OnWindowMouseFocusChanged(FWindowHandle window, bool gotFocus) {}
        virtual void OnWindowKeyboardFocusChanged(FWindowHandle window, bool gotFocus) {}

	private:

    };
    
} // namespace Fusion

#pragma warning(pop)
