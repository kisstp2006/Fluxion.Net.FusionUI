#pragma once

namespace Fusion
{
    
    class FUSIONWIDGETS_API FDecoratedWidget : public FCompoundWidget
    {
        FUSION_CLASS(FDecoratedWidget, FCompoundWidget)
    public:

        FDecoratedWidget(Ref<FObject> outer = nullptr);

        void Paint(FPainter& painter) override;

    public: // - Fusion Properties - 

        FUSION_PROPERTY(FBrush, Background);
        FUSION_PROPERTY(FPen, Border);
        FUSION_PROPERTY(FShape, Shape);

        FUSION_WIDGET;

    };

} // namespace Fusion
