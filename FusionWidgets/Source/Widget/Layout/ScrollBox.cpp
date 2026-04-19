#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FScrollBox::FScrollBox() : Super()
    {
        m_CanScrollVertical          = true;
        m_CanScrollHorizontal        = false;
        m_ScrollOffset               = FVec2(0, 0);
        m_ScrollbarThickness         = 8.0f;
        m_ScrollbarPadding           = 2.0f;
        m_ScrollSpeed                = 40.0f;
        m_VerticalScrollVisibility   = EScrollbarVisibility::Auto;
        m_HorizontalScrollVisibility = EScrollbarVisibility::Auto;

        m_ClipContent = true;
    }

    // -------------------------------------------------
    // Layout
    // -------------------------------------------------


    // -------------------------------------------------
    // Paint
    // -------------------------------------------------

    

    // -------------------------------------------------
    // Hit Testing
    // -------------------------------------------------

    bool FScrollBox::ShouldHitTestChildren(FVec2 localMousePos)
    {
        // localMousePos is already in local widget space (converted by HitTestWidget)
        if (m_bShowVertScrollbar && m_VertTrackRect.Contains(localMousePos))
            return false;
        if (m_bShowHorzScrollbar && m_HorzTrackRect.Contains(localMousePos))
            return false;
        return true;
    }

    // -------------------------------------------------
    // Mouse Events
    // -------------------------------------------------

    FEventReply FScrollBox::OnMouseButtonDown(FMouseEvent& event)
    {
        if (!event.IsLeftButton())
            return FEventReply::Unhandled();

        // Convert surface space → local widget space
        FVec2 localPos = GetCachedLayerSpaceTransform().Inverse().TransformPoint(event.MousePosition);

        if (m_bShowVertScrollbar && m_VertTrackRect.Contains(localPos))
        {
            if (m_VertThumbRect.Contains(localPos))
            {
                m_bDraggingVert      = true;
                m_bVertThumbPressed  = true;
                m_DragStartOffset    = m_ScrollOffset;
                m_DragStartMousePos  = localPos;
            }
            else
            {
                // Click on track — jump to position centred on click
                f32 trackH  = m_VertTrackRect.GetHeight();
                f32 thumbH  = m_VertThumbRect.GetHeight();
                f32 t       = FMath::Clamp((localPos.y - m_VertTrackRect.top - thumbH * 0.5f) / FMath::Max(1.0f, trackH - thumbH), 0.0f, 1.0f);
                f32 maxScrollY = FMath::Max(0.0f, m_ContentSize.y - m_ViewportSize.y);

                m_ScrollOffset.y = t * maxScrollY;
                MarkLayoutDirty();
                m_OnScrollOffsetChanged.ExecuteIfBound(m_ScrollOffset);
            }

            MarkPaintDirty();
            return FEventReply::Handled();
        }

        if (m_bShowHorzScrollbar && m_HorzTrackRect.Contains(localPos))
        {
            if (m_HorzThumbRect.Contains(localPos))
            {
                m_bDraggingHorz      = true;
                m_bHorzThumbPressed  = true;
                m_DragStartOffset    = m_ScrollOffset;
                m_DragStartMousePos  = localPos;
            }
            else
            {
                // Click on track — jump to position
                f32 trackW  = m_HorzTrackRect.GetWidth();
                f32 thumbW  = m_HorzThumbRect.GetWidth();
                f32 t       = FMath::Clamp((localPos.x - m_HorzTrackRect.left - thumbW * 0.5f) / FMath::Max(1.0f, trackW - thumbW), 0.0f, 1.0f);
                f32 maxScrollX = FMath::Max(0.0f, m_ContentSize.x - m_ViewportSize.x);

                m_ScrollOffset.x = t * maxScrollX;
                MarkLayoutDirty();
                m_OnScrollOffsetChanged.ExecuteIfBound(m_ScrollOffset);
            }

            MarkPaintDirty();
            return FEventReply::Handled();
        }

        return FEventReply::Unhandled();
    }

    FEventReply FScrollBox::OnMouseButtonUp(FMouseEvent& event)
    {
        if (!event.IsLeftButton())
            return FEventReply::Unhandled();

        if (m_bDraggingVert || m_bDraggingHorz)
        {
            m_bDraggingVert     = false;
            m_bDraggingHorz     = false;
            m_bVertThumbPressed = false;
            m_bHorzThumbPressed = false;
            MarkPaintDirty();
            return FEventReply::Handled();
        }

        return FEventReply::Unhandled();
    }

    FEventReply FScrollBox::OnMouseMove(FMouseEvent& event)
    {
        FVec2 localPos = GetCachedLayerSpaceTransform().Inverse().TransformPoint(event.MousePosition);

        if (m_bDraggingVert)
        {
            f32 trackH     = m_VertTrackRect.GetHeight();
            f32 thumbH     = m_VertThumbRect.GetHeight();
            f32 maxScrollY = FMath::Max(0.0f, m_ContentSize.y - m_ViewportSize.y);
            f32 usable     = trackH - thumbH;

            if (usable > 0.0f)
            {
                f32 delta = localPos.y - m_DragStartMousePos.y;
                m_ScrollOffset.y = FMath::Clamp(m_DragStartOffset.y + (delta / usable) * maxScrollY, 0.0f, maxScrollY);
                MarkLayoutDirty();
                MarkPaintDirty();
                m_OnScrollOffsetChanged.ExecuteIfBound(m_ScrollOffset);
            }

            return FEventReply::Handled();
        }

        if (m_bDraggingHorz)
        {
            f32 trackW     = m_HorzTrackRect.GetWidth();
            f32 thumbW     = m_HorzThumbRect.GetWidth();
            f32 maxScrollX = FMath::Max(0.0f, m_ContentSize.x - m_ViewportSize.x);
            f32 usable     = trackW - thumbW;

            if (usable > 0.0f)
            {
                f32 delta = localPos.x - m_DragStartMousePos.x;
                m_ScrollOffset.x = FMath::Clamp(m_DragStartOffset.x + (delta / usable) * maxScrollX, 0.0f, maxScrollX);
                MarkLayoutDirty();
                MarkPaintDirty();
                m_OnScrollOffsetChanged.ExecuteIfBound(m_ScrollOffset);
            }

            return FEventReply::Handled();
        }

        // Update hover states
        bool vertHovered = m_bShowVertScrollbar && m_VertThumbRect.Contains(localPos);
        bool horzHovered = m_bShowHorzScrollbar && m_HorzThumbRect.Contains(localPos);

        if (vertHovered != m_bVertThumbHovered || horzHovered != m_bHorzThumbHovered)
        {
            m_bVertThumbHovered = vertHovered;
            m_bHorzThumbHovered = horzHovered;
            MarkPaintDirty();
        }

        return FEventReply::Unhandled();
    }

    void FScrollBox::OnMouseLeave(FMouseEvent& event)
    {
        // Keep drag state active — user may have moved outside widget bounds while dragging.
        // Only clear hover when not dragging.
        if (!m_bDraggingVert && !m_bDraggingHorz)
        {
            if (m_bVertThumbHovered || m_bHorzThumbHovered)
            {
                m_bVertThumbHovered = false;
                m_bHorzThumbHovered = false;
                MarkPaintDirty();
            }
        }
    }

    FEventReply FScrollBox::OnMouseWheel(FMouseEvent& event)
    {
        FVec2 maxScroll = FVec2(
            FMath::Max(0.0f, m_ContentSize.x - m_ViewportSize.x),
            FMath::Max(0.0f, m_ContentSize.y - m_ViewportSize.y)
        );

        bool scrolled = false;

        if (m_CanScrollVertical && event.WheelDelta.y != 0.0f)
        {
            f32 newY = FMath::Clamp(m_ScrollOffset.y - event.WheelDelta.y * m_ScrollSpeed, 0.0f, maxScroll.y);
            if (newY != m_ScrollOffset.y)
            {
                m_ScrollOffset.y = newY;
                scrolled = true;
            }
        }

        if (m_CanScrollHorizontal && event.WheelDelta.x != 0.0f)
        {
            f32 newX = FMath::Clamp(m_ScrollOffset.x - event.WheelDelta.x * m_ScrollSpeed, 0.0f, maxScroll.x);
            if (newX != m_ScrollOffset.x)
            {
                m_ScrollOffset.x = newX;
                scrolled = true;
            }
        }

        if (scrolled)
        {
            MarkLayoutDirty();
            MarkPaintDirty();
            m_OnScrollOffsetChanged.ExecuteIfBound(m_ScrollOffset);
            return FEventReply::Handled();
        }

        return FEventReply::Unhandled();
    }

} // namespace Fusion
