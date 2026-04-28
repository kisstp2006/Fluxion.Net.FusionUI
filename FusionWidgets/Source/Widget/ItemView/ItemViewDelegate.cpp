#include "Fusion/Widgets.h"

namespace Fusion
{
    void FItemViewDelegate::Paint(FPainter& painter, FModelIndex index, const FItemViewPaintInfo& info)
    {
        FVariant content = info.Model->GetItemData(index, EItemRole::Content);

        painter.PushClip(info.Rect, FRectangle());

        if (content.Has<FString>())
        {
            painter.SetFont(FFont::Regular(FFont::kDefaultFamilyName, 12.0f));
            painter.SetPen(FColors::White);

            const FString text = content.Get<FString>();
            painter.DrawText(info.Rect.Translate(FVec2(5.0f, 0)),
                text, ETextWrap::None, EHAlign::Left, EVAlign::Center);
        }

        painter.PopClip();
    }
} // namespace Fusion
