#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FWindow : public FDecoratedWidget
    {
        FUSION_WIDGET(FWindow, FDecoratedWidget)
    protected:

        FWindow();

        void Construct() override;

    public:

        FVec2 MeasureContent(FVec2 availableSize) override;

        void ArrangeContent(FVec2 finalSize) override;

    private:



    public:

        // - Fusion Properties -

        FUSION_SLOTS(
            (FTitleBar, TitleBar),
            (FWidget, Content)
        );

        FUSION_STYLE_PROPERTIES(
            (f32, TitleBarHeight, Layout)
        );

        FUSION_PROPERTY_FORWARD(FString, WindowTitle, m_TitleBar, Title)
    };
    
} // namespace Fusion
