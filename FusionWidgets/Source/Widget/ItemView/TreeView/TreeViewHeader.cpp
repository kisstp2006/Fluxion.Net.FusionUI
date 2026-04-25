#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTreeViewHeader::FTreeViewHeader()
    {

    }

    void FTreeViewHeader::Construct()
    {
        Super::Construct();

        Child(
            FAssignNew(FSplitBox, m_Splitter)
            .Direction(EStackDirection::Horizontal)
            .HAlign(EHAlign::Fill)
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
                        .HAlign(EHAlign::Center)
                        .FillRatio(1.0f / maxChildCount)
                        .Margin(FMargin(2, 1, 2, 1) * 5.0f)
                    );
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

} // namespace Fusion
