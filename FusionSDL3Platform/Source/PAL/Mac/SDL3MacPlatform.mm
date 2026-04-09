// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#import <Cocoa/Cocoa.h>

#include "Fusion/SDL3Platform.h"
#include "PAL/Mac/SDL3MacPlatform.h"

namespace Fusion
{
    f32 FSDL3MacPlatform::GetWindowDpiScale(SDL_Window* sdlWindow)
    {
        SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
        void* rawWindow = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
        if (!rawWindow)
            return SDL_GetWindowDisplayScale(sdlWindow);

        NSWindow* nsWindow = (__bridge NSWindow*)rawWindow;
        return (f32)[nsWindow backingScaleFactor];
    }

} // namespace Fusion
