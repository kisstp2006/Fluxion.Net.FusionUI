#pragma once

namespace Fusion
{

    class FUSIONWIDGETS_API FTextInput : public FDecoratedWidget
    {
        FUSION_WIDGET(FTextInput, FDecoratedWidget)
    protected:

        FTextInput();

        // - Layout -

    public:

        // - Properties -

        FUSION_PROPERTY(FString, Text);
        FUSION_PROPERTY(FString, Placeholder);
        FUSION_PROPERTY(bool, IsPassword);
        FUSION_PROPERTY(int, MaxLength);

        FUSION_STYLE_PROPERTIES(
            (FFont,  Font,              LayoutAndPaint),
            (FColor, TextColor,         Paint),
            (FColor, PlaceholderColor,  Paint),
            (FColor, SelectionColor,    Paint),
            (FColor, CursorColor,       Paint)
        );

    protected:

    };
    
} // namespace Fusion
