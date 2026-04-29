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

    FNativeChromeMetrics FSDL3MacPlatform::GetChromeMetrics(SDL_Window* sdlWindow)
    {
        SDL_PropertiesID props = SDL_GetWindowProperties(sdlWindow);
        NSWindow* nswindow = (NSWindow*)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
        if (!nswindow) return {};

        NSButton* close    = [nswindow standardWindowButton:NSWindowCloseButton];
        NSButton* miniaturize = [nswindow standardWindowButton:NSWindowMiniaturizeButton];
        NSButton* zoom     = [nswindow standardWindowButton:NSWindowZoomButton];
        if (!close || !miniaturize || !zoom) return {};

        // Button frames are in contentView coords (bottom-left origin on macOS).
        // Convert to top-left origin (FusionUI convention).
        NSRect contentBounds = nswindow.contentView.bounds;
        float  winH = (float)contentBounds.size.height;

        auto toFRect = [&](NSButton* btn) -> FRect {
            NSRect f = [btn convertRect:btn.bounds toView:nil]; // window coords, bottom-left
            return FRect(f.origin.x, winH - f.origin.y - f.size.height,
                         f.size.width, f.size.height);
        };

        FRect cr = toFRect(close);
        FRect zr = toFRect(zoom);

        // Union close→zoom to get the full reserved rect
        FRect combined = FRect::Union(cr, zr);

        // Add a small margin so custom content doesn't hug the buttons
        combined.max.x  += 8.0f;

        return FNativeChromeMetrics{ combined, 0.0f };
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
                break;
            case ETitleBarStyle::ToolBarWithNativeControls:
                break;
            }

        }
    }
    
} // namespace Fusion
