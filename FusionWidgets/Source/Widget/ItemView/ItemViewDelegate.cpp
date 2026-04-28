#include "Fusion/Widgets.h"

namespace Fusion
{
    void FItemViewDelegate::Paint(FPainter& painter, FModelIndex index, const FItemViewPaintInfo& info)
    {
        constexpr f32 kLeftPadding   = 2.0f;  // leading gap before first element
        constexpr f32 kChevronSize   = 10.0f;
        constexpr f32 kChevronGap    = 4.0f;  // gap between chevron and icon/text
        constexpr f32 kIconGap       = 4.0f;  // gap between icon and text

        const bool  expandable    = index.Column() == 0 && info.View->IsExpandable();
        const u32   childrenCount = info.Model->GetRowCount(index);
        const bool  showExpander  = expandable && childrenCount > 0;
        const f32   iconWidth     = info.View->IconWidth();
        const bool  hasIcons      = iconWidth > 0.001f && info.Model->HasIcons(index.Column());
        const FVec2 rectSize      = info.Rect.GetSize();
        const f32   centerY       = info.Rect.min.y + rectSize.height * 0.5f;

        painter.PushClip(info.Rect, FRectangle());

        // Cursor advances left-to-right as elements are placed
        f32 cursorX = info.Rect.min.x + kLeftPadding;

        // --- Chevron ---
        // Always reserve chevron space for expandable columns so text
        // aligns consistently across rows with and without children.
        if (expandable)
        {
            if (showExpander)
            {
                thread_local const FName caretRight = "embed:/Icons/CaretRight.png";

                FBrush chevronBrush = FBrush::Image(caretRight)
                    .BrushSize(FVec2(kChevronSize, kChevronSize));

                painter.SetBrush(chevronBrush);
                painter.FillRect(FRect::FromSize(
                    FVec2(cursorX, centerY - kChevronSize * 0.5f),
                    FVec2(kChevronSize, kChevronSize)));
            }
            cursorX += kChevronSize + kChevronGap;
        }

        // --- Icon ---
        // Always reserve icon space when the column has icons so text
        // aligns consistently across rows with and without an icon.
        if (hasIcons)
        {
            FVariant icon = info.Model->GetItemData(index, EItemRole::Icon);
            FString  iconPath;

            if (icon.Has<FName>())
                iconPath = icon.Get<FName>().ToString();
            else if (icon.Has<FString>())
                iconPath = icon.Get<FString>();

            if (!iconPath.Empty())
            {
                FBrush iconBrush = FBrush::Image(iconPath)
                    .BrushSize(FVec2(iconWidth, iconWidth));

                painter.SetBrush(iconBrush);
                painter.FillRect(FRect::FromSize(
                    FVec2(cursorX, centerY - iconWidth * 0.5f),
                    FVec2(iconWidth, iconWidth)));
            }
            cursorX += iconWidth + kIconGap;
        }

        // --- Text ---
        FVariant content = info.Model->GetItemData(index, EItemRole::Content);

        if (content.Has<FString>())
        {
            painter.SetFont(FFont::Regular(FFont::kDefaultFamilyName, 12.0f));
            painter.SetPen(FColors::White);

            FRect textRect = FRect(FVec2(cursorX, info.Rect.min.y), info.Rect.max);
            painter.DrawText(textRect, content.Get<FString>(),
                ETextWrap::None, EHAlign::Left, EVAlign::Center);
        }

        painter.PopClip();
    }
} // namespace Fusion
