#pragma once

namespace Fusion
{
    
    class FUSIONWIDGETS_API FDecoratedWidget : public FCompoundWidget
    {
        FUSION_CLASS(FDecoratedWidget, FCompoundWidget)
    public:

        FDecoratedWidget();

    protected:

        void Paint(FPainter& painter) override;

    public:

        FUSION_STYLE_PROPERTIES(
            (FBrush, Background),
            (FPen,   Border),
            (FShape, Shape)
        );

    };

} // namespace Fusion
