#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FNativeSurface : public FSurface
    {
        FUSION_CLASS(FNativeSurface, FSurface)
    public:

        FNativeSurface(FWindowHandle windowHandle, FObject* outer = nullptr);

        // - Public API -

        void Initialize() override;

        void Shutdown() override;

        bool IsNativeSurface() const final { return true; }

        void OnWindowResized();

    protected:

        FWindowHandle m_WindowHandle = FWindowHandle::NullValue;
    };
    
} // namespace Fusion
