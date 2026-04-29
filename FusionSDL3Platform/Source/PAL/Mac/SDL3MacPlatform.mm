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

    void FSDL3MacPlatform::SetupWindow(SDL_Window* sdlWindow, const FPlatformWindowInfo& info)
    {
        SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
        NSWindow* nswindow = (NSWindow*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);

        if (nswindow) {
            // Set the full mask — don't rely on SDL's existing styleMask
            nswindow.styleMask = NSWindowStyleMaskTitled
                               | NSWindowStyleMaskClosable
                               | NSWindowStyleMaskMiniaturizable;

            if (info.resizable)
                nswindow.styleMask |= NSWindowStyleMaskResizable;
            else
                nswindow.styleMask &= ~NSWindowStyleMaskResizable;

            switch (info.titleBarStyle)
            {
            case ETitleBarStyle::Default:
                nswindow.styleMask &= ~NSWindowStyleMaskFullSizeContentView;
                nswindow.titlebarAppearsTransparent = NO;
                nswindow.titleVisibility            = NSWindowTitleVisible;
                break;
            case ETitleBarStyle::NoTitleBar:
                nswindow.styleMask |= NSWindowStyleMaskFullSizeContentView;
                nswindow.titlebarAppearsTransparent = YES;
                nswindow.titleVisibility            = NSWindowTitleHidden;
                // Ensure buttons aren't explicitly hidden
                [[nswindow standardWindowButton:NSWindowCloseButton]    setHidden:YES];
                [[nswindow standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
                [[nswindow standardWindowButton:NSWindowZoomButton]     setHidden:YES];
                break;
            case ETitleBarStyle::TransparentWithNativeControls:
                nswindow.styleMask |= NSWindowStyleMaskFullSizeContentView;
                nswindow.titlebarAppearsTransparent = YES;
                nswindow.titleVisibility            = NSWindowTitleHidden;
                [[nswindow standardWindowButton:NSWindowCloseButton]    setHidden:NO];
                [[nswindow standardWindowButton:NSWindowMiniaturizeButton] setHidden:NO];
                [[nswindow standardWindowButton:NSWindowZoomButton]     setHidden:NO];

                [nswindow setMovableByWindowBackground:NO];
                break;
            case ETitleBarStyle::ToolBarWithNativeControls:
                {
                    nswindow.styleMask |= NSWindowStyleMaskFullSizeContentView;
                    nswindow.titlebarAppearsTransparent = YES;
                    nswindow.titleVisibility            = NSWindowTitleHidden;
                    [[nswindow standardWindowButton:NSWindowCloseButton]    setHidden:NO];
                    [[nswindow standardWindowButton:NSWindowMiniaturizeButton] setHidden:NO];
                    [[nswindow standardWindowButton:NSWindowZoomButton]     setHidden:NO];

                    NSToolbar* toolbar = [[NSToolbar alloc] initWithIdentifier:@"FusionToolbar"];
                    toolbar.showsBaselineSeparator = NO;
                    [nswindow setToolbar:toolbar];

                    [nswindow setMovableByWindowBackground:NO];
                }
                break;
            }

        }
    }
    
} // namespace Fusion
