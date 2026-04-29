#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{


    class FUSIONWIDGETS_API FTitleBar : public FDecoratedBox
    {
        FUSION_WIDGET(FTitleBar, FDecoratedBox)
    protected:

        FTitleBar();

        void Construct() override;

    private:

        Ref<FLabel> m_TitleLabel;

    public:

        FUSION_PROPERTY_FORWARD(FString, Title, m_TitleLabel, Text);

    };

}