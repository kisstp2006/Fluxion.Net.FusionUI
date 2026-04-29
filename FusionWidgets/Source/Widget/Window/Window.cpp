#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FWindow::FWindow()
    {

    }

    void FWindow::Construct()
    {
        Super::Construct();

        TitleBar(
            FNew(FTitleBar)
        );
    }

    FVec2 FWindow::MeasureContent(FVec2 availableSize)
    {
        auto surface = GetParentSurface();
        if (!surface)
            return m_DesiredSize = ApplyLayoutConstraints(GetMinimumContentSize());

        FNativeChromeMetrics chromeMetrics = surface->GetNativeChromeMetrics();

        FVec2 baseSize = GetMinimumContentSize();

        if (!m_Content || m_Content->IsExcluded())
        {
            return m_DesiredSize = ApplyLayoutConstraints(baseSize);
        }

        FMargin childMargin = m_Content->Margin();

        FVec2 childAvailableSize = FVec2(
            FMath::Max(0.0f, availableSize.x - (childMargin.left + childMargin.right + Padding().left + Padding().right)),
            FMath::Max(0.0f, availableSize.y - (childMargin.top + childMargin.bottom + Padding().top + Padding().bottom))
        );

        FVec2 childSize;

        FUSION_TRY
        {
            childSize = m_Content->MeasureContent(childAvailableSize);
        }
        FUSION_CATCH(const FException& exception)
        {
            FUSION_LOG_ERROR("Widget", "Exception: {}. Exception thrown by a Widget [{}] in FWindow::MeasureContent.\n{}",
                exception.what(), m_Content->GetClassName(), exception.GetStackTraceString(true));

            SetFaulted();
            return {};
        }

        return m_DesiredSize = ApplyLayoutConstraints(FVec2(
            childSize.x + childMargin.left + childMargin.right + Padding().left + Padding().right,
            childSize.y + childMargin.top + childMargin.bottom + Padding().top + Padding().bottom
        ));
    }

    void FWindow::ArrangeContent(FVec2 finalSize)
    {
        FDecoratedWidget::ArrangeContent(finalSize);
    }
} // namespace Fusion
