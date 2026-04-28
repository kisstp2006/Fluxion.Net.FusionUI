#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FTreeViewRow : public FDecoratedBox
    {
        FUSION_WIDGET(FTreeViewRow, FDecoratedBox)
    protected:

        FTreeViewRow();

        void Construct() override;

    public:

        Ref<FTreeView> GetTreeView() const { return m_TreeView.Lock(); }

        void Paint(FPainter& painter) override;

        void SetData(const TArray<FModelIndex>& columns);

    protected:

        Ref<FHorizontalStack> m_HStack;
        WeakRef<FTreeView> m_TreeView;
        TArray<FModelIndex> m_Columns;

    private:

        friend class FTreeView;
        friend class FTreeViewContent;
    };
    
} // namespace Fusion
