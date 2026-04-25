#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FTreeViewRow : public FDecoratedWidget
    {
        FUSION_WIDGET(FTreeViewRow, FDecoratedWidget)
    protected:

        FTreeViewRow();

    public:

        u32 GetSlotCount() override { return m_ColumnCount; }

        bool IsValidSlotWidget(u32 slot, FWidget* widget) override { return slot < m_ColumnCount; }

        int GetChildCount() override { return m_ColumnCount; }

        Ref<FWidget> GetChildAt(u32 index) override
        {
            return nullptr; // TODO
        }

    protected:

    private:

        u32 m_ColumnCount;

        friend class FTreeView;
        friend class FTreeViewContent;
    };
    
} // namespace Fusion
