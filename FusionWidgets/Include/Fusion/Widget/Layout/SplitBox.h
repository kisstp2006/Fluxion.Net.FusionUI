#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    
    class FUSIONWIDGETS_API FSplitBox : public FStackBox
    {
        FUSION_WIDGET(FSplitBox, FStackBox)
    protected:

        FSplitBox();

    public:

        // - Cursor -

        FCursor GetActiveCursorAt(FVec2 localPos) override;

        // - Layout -

        void ArrangeContent(FVec2 finalSize) override;

        // - Events -

        bool ShouldHitTestChildren(FVec2 localMousePose) override;

        FEventReply OnMouseButtonDown(FMouseEvent& event) override;
        FEventReply OnMouseButtonUp(FMouseEvent& event) override;

        void OnMouseEnter(FMouseEvent& event) override;
        FEventReply OnMouseMove(FMouseEvent& event) override;
        void OnMouseLeave(FMouseEvent& event) override;

        // - Paint -

        void Paint(FPainter& painter) override;

    protected:

        TArray<FRect, 4> m_HandleRects;

        int   m_DraggedRect        = -1;
        bool  m_bIsDragHovered     = false;
        bool  m_bIsBeingResized    = false;

        FVec2 m_DragStartLocalPos   = FVec2(0, 0);
        f32   m_DragStartLeftSize   = 0.0f;
        f32   m_DragStartTotalSize  = 0.0f;
        f32   m_DragStartTotalRatio = 0.0f;

    public:

        // - Fusion Properties -

        FUSION_STYLE_PROPERTIES(
            (FColor, SplitterColor,      Paint),
            (FColor, SplitterHoverColor, Paint),
            (f32,    SplitterSizeRatio,  Paint)
        );

        FUSION_PROPERTY(bool, CanResizeSplitter);
        FUSION_PROPERTY(bool, RespectContentSize);
    };

} // namespace Fusion
