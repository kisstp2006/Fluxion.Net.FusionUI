#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    class FTreeView;

    class FUSIONWIDGETS_API FTreeViewContent : public FContainerWidget
    {
        FUSION_WIDGET(FTreeViewContent, FContainerWidget)
    protected:

        FTreeViewContent();

        void Construct() override;

    public:

        Ref<FTreeView> GetTreeView() const { return m_TreeView.Lock(); }

        // - Layout -

        FVec2 MeasureContent(FVec2 availableSize) override;

        void ArrangeContent(FVec2 finalSize) override;

    protected:

        WeakRef<FTreeView> m_TreeView;

    public:

        FUSION_PROPERTY_SET(Ref<FTreeView>, TreeView)
        {
            self.m_TreeView = value;
            return self;
        }

    };
    
} // namespace Fusion
