#pragma once

namespace Fusion
{
    
    class FUSIONWIDGETS_API FExpanderBox : public FVerticalStack
    {
        FUSION_WIDGET(FExpanderBox, FVerticalStack)
    protected:
        
        FExpanderBox();

        void Construct() override;

        void Paint(FPainter& painter) override;

    protected:

        Ref<FButton> m_Header;
        Ref<FDecoratedBox> m_Content;

        void SetupHeader();
        void SetupContent();

    public:

        // - Fusion Properties -

        FUSION_PROPERTY_SET(FButton&, Header)
        {
            if (self.m_Header == value)
                return self;
            self.RemoveChildWidget(self.m_Header);
            self.m_Header = &value;
            self.AddChildWidget(&value);
            static_cast<FExpanderBox&>(self).SetupHeader();
            return self;
        }

        FUSION_PROPERTY_SET(FDecoratedBox&, Content)
        {
            if (self.m_Content == value)
                return self;
            self.RemoveChildWidget(self.m_Content);
            self.m_Content = &value;
            self.AddChildWidget(&value);
            static_cast<FExpanderBox&>(self).SetupContent();
            return self;
        }

        FUSION_STYLE_PROPERTIES(
            (FBrush, Background, Paint),
            (FPen,   Border,     Paint),
            (FShape, Shape,      Paint)
        );
        
    };

} // namespace Fusion
