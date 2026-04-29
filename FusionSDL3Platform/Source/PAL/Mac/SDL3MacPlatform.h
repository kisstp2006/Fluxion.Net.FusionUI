#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#include "../SDL3Platform.h"

namespace Fusion
{
    class FSDL3MacPlatform : public FSDL3PlatformBase
    {
    public:
        FSDL3MacPlatform() = delete;
        ~FSDL3MacPlatform() = delete;

        static f32 GetWindowDpiScale(SDL_Window* sdlWindow);
        
        static void SetupWindow(SDL_Window* sdlWindow, const FPlatformWindowInfo& info);
    };

    typedef FSDL3MacPlatform FSDL3Platform;

} // namespace Fusion
