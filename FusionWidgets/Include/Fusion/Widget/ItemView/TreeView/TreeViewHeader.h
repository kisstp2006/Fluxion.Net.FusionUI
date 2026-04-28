#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FTreeViewHeader : public FDecoratedBox
    {
        FUSION_WIDGET(FTreeViewHeader, FDecoratedBox)
    protected:

        FTreeViewHeader();

        void Construct() override;

    public:

        void UpdateHeaderData(Ref<FItemModel> model);

        TArray<f32> GetChildrenWidths();
        f32 GetSplitterSpacing();

    protected:

        Ref<FSplitBox> m_Splitter;

    public:
        // - Fusion Properties -

        FUSION_PROPERTY_GET(bool, CanResizeColumns)
        {
            return m_Splitter->CanResizeSplitter();
        }

        FUSION_PROPERTY_SET(bool, CanResizeColumns)
        {
            static_cast<Self&>(self).m_Splitter->CanResizeSplitter(value);
            return self;
        }

        FUSION_STYLE_PROPERTIES(
            (FMargin, CellPadding, Layout)
        );
    };
    
} // namespace Fusion
