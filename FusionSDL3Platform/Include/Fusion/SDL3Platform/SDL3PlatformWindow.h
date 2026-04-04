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

		f32 GetDpiScale();

		FPlatformWindowFlags GetInitialFlags() const { return initialFlags; }

		SDL_Window* GetSdlHandle() const { return sdlWindow; }

    private:

		FWindowHandle windowHandle = FWindowHandle::NullValue;
		SDL_Window* sdlWindow = nullptr;

		FPlatformWindowFlags initialFlags = FPlatformWindowFlags::None;
    };

} // namespace Fusion
