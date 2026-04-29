#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

	class FUSIONSDL3PLATFORM_API FSDL3PlatformWindow
    {
    public:

        FSDL3PlatformWindow(const FString& title, u32 width, u32 height, const FPlatformWindowInfo& info);

		~FSDL3PlatformWindow();

		bool IsValid() const { return windowHandle.IsValid(); }

		FWindowHandle GetWindowHandle() const { return windowHandle; }

		FVec2i GetSizeInPixels();
		FVec2i GetSize();

		FVec2i GetPosition();

		f32 GetDpiScale();

		EPlatformWindowFlags GetInitialFlags() const { return initialFlags; }

		SDL_Window* GetSdlHandle() const { return sdlWindow; }

	    ETitleBarStyle GetWindowTitleBarStyle() const { return titleBarStyle; }

    private:

		FWindowHandle windowHandle = FWindowHandle::NullValue;
		SDL_Window* sdlWindow = nullptr;

		EPlatformWindowFlags initialFlags = EPlatformWindowFlags::None;
	    ETitleBarStyle titleBarStyle = ETitleBarStyle::Default;
    };

} // namespace Fusion
