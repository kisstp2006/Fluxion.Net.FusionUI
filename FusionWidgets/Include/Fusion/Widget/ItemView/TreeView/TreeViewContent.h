#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    class FTreeViewRow;
    class FTreeView;

    class FUSIONWIDGETS_API FTreeViewContent : public FContainerWidget
    {
        FUSION_WIDGET(FTreeViewContent, FContainerWidget)
    protected:

        FTreeViewContent();

        void Construct() override;

    public:

        Ref<FTreeView> GetTreeView() const { return m_TreeView.Lock(); }

        void OnModelChanged(Ref<FItemModel> model);

        void OnScrollOffsetChanged(FVec2 offset);

        // - Layout -

        FVec2 MeasureContent(FVec2 availableSize) override;

        void ArrangeContent(FVec2 finalSize) override;

    protected:

        Ref<FScrollBox> GetParentScrollBox();

        void RebuildFlatRows();
        void AppendRows(FModelIndex parent, int depth);

        void UpdateVisibleRows(FVec2 finalSize);

        WeakRef<FTreeView> m_TreeView;

    public:

        FUSION_PROPERTY_SET(Ref<FTreeView>, TreeView)
        {
            self.m_TreeView = value;
            return self;
        }

    private:

        struct FTreeViewFlatRow
        {
            FModelIndex index;
            int         depth;
            bool        hasChildren;
        };

        TArray<FTreeViewFlatRow> m_FlatRows;
        THashSet<FModelIndex> m_ExpandedItems;
        TArray<Ref<FTreeViewRow>> m_Rows;
        FVec2 m_CachedFinalSize;
    };
    
} // namespace Fusion
