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

    void FTreeViewContent::OnModelChanged(Ref<FItemModel> model)
    {
        if (!model)
            return;

        RebuildFlatRows();
        MarkLayoutDirty();
        MarkPaintDirty();
    }

    void FTreeViewContent::OnScrollOffsetChanged(FVec2 offset)
    {
        UpdateVisibleRows(m_CachedFinalSize);
    }

    void FTreeViewContent::RebuildFlatRows()
    {
        m_FlatRows.Clear();
        AppendRows({}, 0);  // start from root
    }

    void FTreeViewContent::AppendRows(FModelIndex parent, int depth)
    {
        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView)
            return;

        Ref<FItemModel> model = treeView->Model();
        if (!model)
            return;

        u32 count = model->GetRowCount(parent);
        for (u32 i = 0; i < count; i++)
        {
            FModelIndex index = model->GetIndex(i, 0, parent);
            u32 childCount    = model->GetRowCount(index);
            m_FlatRows.Add({ index, depth, childCount > 0 });

            if (childCount > 0 && m_ExpandedItems.Contains(index))
                AppendRows(index, depth + 1);  // recurse only into expanded nodes
        }
    }

    FVec2 FTreeViewContent::MeasureContent(FVec2 availableSize)
    {
        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView || !treeView->Model())
            return m_DesiredSize = ApplyLayoutConstraints(availableSize);

        Ref<FItemModel> model = treeView->Model();

        return m_DesiredSize = ApplyLayoutConstraints(FVec2(
            availableSize.x,
            m_FlatRows.Size() * treeView->RowHeight()
        ));
    }

    void FTreeViewContent::ArrangeContent(FVec2 finalSize)
    {
        ArrangeContentBase(finalSize);

        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView)
            return;

        m_CachedFinalSize = finalSize;
        UpdateVisibleRows(finalSize);
    }

    Ref<FScrollBox> FTreeViewContent::GetParentScrollBox()
    {
        if (Ref<FWidget> parent = GetParentWidget())
        {
            if (Ref<FScrollBox> scrollBox = parent->Cast<FScrollBox>())
            {
                return scrollBox;
            }
        }

        return nullptr;
    }

    void FTreeViewContent::UpdateVisibleRows(FVec2 finalSize)
    {
        Ref<FTreeView> treeView = GetTreeView();
        if (!treeView)
            return;

        Ref<FItemModel> model = treeView->Model();
        if (!model)
            return;

        Ref<FScrollBox> scrollBox = GetParentScrollBox();
        if (!scrollBox)
            return;

        f32  rowHeight    = treeView->RowHeight();
        f32  scrollY      = scrollBox->ScrollOffset().y;
        f32  viewportH    = scrollBox->GetViewportSize().y;

        int firstRow = (int)(scrollY / rowHeight);
        int lastRow  = FMath::Min((int)((scrollY + viewportH) / rowHeight) + 1,
                                  (int)m_FlatRows.Size() - 1);

        int needed = lastRow - firstRow + 1;

        // Grow pool dynamically
        while ((int)m_Rows.Size() < needed)
        {
            Ref<FTreeViewRow> row;
            FAssignNew(FTreeViewRow, row);
            AddChildWidget(row);
            m_Rows.Add(row);
        }

        // Hide pool rows not needed this frame  ← ADD THIS
        for (int i = needed; i < (int)m_Rows.Size(); i++)
            m_Rows[i]->Excluded(true);

        for (int i = firstRow; i <= lastRow; i++)
        {
            Ref<FTreeViewRow> row = m_Rows[i - firstRow];
            row->Excluded(false);
            // TODO: row->SetData();
            f32 y = i * rowHeight;
            f32 x = m_FlatRows[i].depth * treeView->IndentWidth();
            row->SetLayoutPosition(FVec2(x, y));
            row->ArrangeContent(FVec2(finalSize.x - x, rowHeight));
        }
    }

} // namespace Fusion
