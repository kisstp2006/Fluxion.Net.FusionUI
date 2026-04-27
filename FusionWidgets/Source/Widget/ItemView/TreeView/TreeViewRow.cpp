#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTreeViewRow::FTreeViewRow()
    {

    }

    void FTreeViewRow::Construct()
    {
        Super::Construct();

        Child(
            FAssignNew(FHorizontalStack, m_HStack)
            .ContentVAlign(EVAlign::Fill)
            .HAlign(EHAlign::Fill)
            .VAlign(EVAlign::Fill)
        );
    }



} // namespace Fusion
