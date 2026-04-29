#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTreeViewHeader::FTreeViewHeader()
    {
        m_CellPadding = FMargin(2, 1, 2, 1) * 5.0f;
    }

    void FTreeViewHeader::Construct()
    {
        Super::Construct();

        Child(
            FAssignNew(FSplitBox, m_Splitter)
            .SubStyle("Splitter")
            .Direction(EStackDirection::Horizontal)
            .RespectContentSize(true)
            .HAlign(EHAlign::Fill)
            .Spacing(2.0f)
            .SplitterSizeRatio(1.5f)
            .ContentHAlign(EHAlign::Fill)
            .ContentVAlign(EVAlign::Fill)
        );
    }

    void FTreeViewHeader::UpdateHeaderData(Ref<FItemModel> model)
    {
        if (!model || !model->HasHeader())
        {
            return;
        }

        FUSION_TRY
        {
            u32 maxChildCount = FMath::Max<u32>(m_Splitter->GetChildCount(), model->GetColumnCount());

            for (u32 i = 0; i < maxChildCount; i++)
            {
                if (i >= m_Splitter->GetChildCount())
                {
                    m_Splitter->AddChild(
                        FNew(FLabel)
                        .Text("Column")
                        .FontSize(12)
                        .HAlign(EHAlign::Left)
                        .FillRatio(1.0f / maxChildCount)
                        .Margin(CellPadding())
                    );
                }
                else
                {
                    if (Ref<FLabel> label = m_Splitter->GetChildAt(i)->Cast<FLabel>())
                    {
                        label->Margin(CellPadding());
                    }
                }

                if (Ref<FLabel> label = m_Splitter->GetChildAt(i)->Cast<FLabel>())
                {
                    label->Text(model->GetHeaderItemData(i).Get<FString>());
                }
            }
        }
        FUSION_CATCH(const FException& exception)
        {
            FUSION_LOG_ERROR("Widget", "Exception: {}. Exception thrown in FTreeViewHeader::UpdateHeaderData.\n{}",
                exception.what(), exception.GetStackTraceString(true));

            SetFaulted();
        }
    }

    TArray<f32> FTreeViewHeader::GetColumnBoundaries()
    {
        return m_Splitter->GetColumnBoundaries();
    }

    TArray<f32> FTreeViewHeader::GetChildrenWidths()
    {
        return m_Splitter->GetChildrenWidths();
    }

    f32 FTreeViewHeader::GetSplitterSpacing()
    {
        return m_Splitter->Spacing() * m_Splitter->SplitterSizeRatio();
    }
} // namespace Fusion
