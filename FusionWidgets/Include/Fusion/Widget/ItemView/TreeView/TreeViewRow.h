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

    protected:

        Ref<FHorizontalStack> m_HStack;
        WeakRef<FTreeView> m_TreeView;

    private:

        friend class FTreeView;
        friend class FTreeViewContent;
    };
    
} // namespace Fusion
