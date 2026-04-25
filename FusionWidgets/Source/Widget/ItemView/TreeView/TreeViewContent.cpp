#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTreeViewContent::FTreeViewContent()
    {

    }

    void FTreeViewContent::Construct()
    {
        Super::Construct();


    }

    FVec2 FTreeViewContent::MeasureContent(FVec2 availableSize)
    {
        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView)
            return FVec2();

        return m_DesiredSize = ApplyLayoutConstraints(availableSize);
    }

    void FTreeViewContent::ArrangeContent(FVec2 finalSize)
    {
        Super::ArrangeContent(finalSize);

        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView)
            return;


    }

} // namespace Fusion
