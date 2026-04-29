#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTitleBar::FTitleBar()
    {
        SetWidgetFlag(EWidgetFlags::TitleBarHint, true);
    }

    void FTitleBar::Construct()
    {
        Super::Construct();

        Child(
            FAssignNew(FLabel, m_TitleLabel)
            .Text("Title")
            .SubStyle("Title")
            .HAlign(EHAlign::Center)
            .VAlign(EVAlign::Center)
        );
    }

}
