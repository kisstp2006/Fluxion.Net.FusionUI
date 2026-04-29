#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FWindow::FWindow()
    {
        m_TitleBarHeight = 32.0f;
    }

    void FWindow::Construct()
    {
        Super::Construct();

        StyleScopeBoundary(true);

        TitleBar(
            FNew(FTitleBar)
            .SubStyle("TitleBar")
        );
    }

    FVec2 FWindow::MeasureContent(FVec2 availableSize)
    {
        auto surface = GetParentSurface();
        if (!surface)
            return m_DesiredSize = ApplyLayoutConstraints(GetMinimumContentSize());

        const ETitleBarStyle titleBarStyle = surface->GetNativeTitleBarStyle();

        const FVec2 baseSize = GetMinimumContentSize();

        if (m_TitleBar)
            m_TitleBar->Excluded(titleBarStyle == ETitleBarStyle::Default);

        if (m_TitleBar && !m_TitleBar->Excluded())
        {
            m_TitleBar->MeasureContent(FVec2(availableSize.x, TitleBarHeight()));
            availableSize.y -= TitleBarHeight();
        }

        if (!m_Content || m_Content->Excluded())
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
        Super::ArrangeContent(finalSize);
        // Always use layoutSize from base class, as it has the layout constraints applied.

        auto surface = GetParentSurface();
        if (!surface)
            return;

        f32 posY = 0;
        const ETitleBarStyle titleBarStyle = surface->GetNativeTitleBarStyle();

        if (m_TitleBar)
            m_TitleBar->Excluded(titleBarStyle == ETitleBarStyle::Default);

        if (m_TitleBar && !m_TitleBar->Excluded())
        {
            FVec2 titleBarDesiredSize = m_TitleBar->GetDesiredSize();

            m_TitleBar->SetLayoutPosition(FVec2(0, 0));
            m_TitleBar->ArrangeContent(FVec2(finalSize.x, TitleBarHeight()));
            posY += TitleBarHeight();
            finalSize.y -= TitleBarHeight();
        }

        if (!m_Content || m_Content->IsExcluded())
            return;

        FMargin childMargin = m_Content->Margin();

        f32 childAreaWidth = FMath::Max(0.0f, finalSize.x - Padding().left - Padding().right - childMargin.left - childMargin.right);
        f32 childAreaHeight = FMath::Max(0.0f, finalSize.y - Padding().top - Padding().bottom - childMargin.top - childMargin.bottom);

        FVec2 childPos = FVec2(Padding().left + childMargin.left, posY + Padding().top + childMargin.top );
        FVec2 childSize;

        switch (m_Content->HAlign())
        {
        case EHAlign::Auto:
        case EHAlign::Fill:
            childSize.x = childAreaWidth;
            break;
        case EHAlign::Left:
            childSize.x = FMath::Min(m_Content->GetDesiredSize().x, childAreaWidth);
            break;
        case EHAlign::Center:
            childSize.x = FMath::Min(m_Content->GetDesiredSize().x, childAreaWidth);
            childPos.x += (childAreaWidth - childSize.x) / 2.0f;
            break;
        case EHAlign::Right:
            childSize.x = FMath::Min(m_Content->GetDesiredSize().x, childAreaWidth);
            childPos.x += childAreaWidth - childSize.x;
            break;
        }

        switch (m_Content->VAlign())
        {
        case EVAlign::Auto:
        case EVAlign::Fill:
            childSize.y = childAreaHeight;
            break;
        case EVAlign::Top:
            childSize.y = FMath::Min(m_Content->GetDesiredSize().y, childAreaHeight);
            break;
        case EVAlign::Center:
            childSize.y = FMath::Min(m_Content->GetDesiredSize().y, childAreaHeight);
            childPos.y += (childAreaHeight - childSize.y) / 2.0f;
            break;
        case EVAlign::Bottom:
            childSize.y = FMath::Min(m_Content->GetDesiredSize().y, childAreaHeight);
            childPos.y += childAreaHeight - childSize.y;
            break;
        }

        m_Content->SetLayoutPosition(childPos);
        m_Content->ArrangeContent(childSize);
    }
} // namespace Fusion
