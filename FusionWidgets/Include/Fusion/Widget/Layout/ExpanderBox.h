#pragma once

namespace Fusion
{
    
    class FUSIONWIDGETS_API FExpanderBox : public FContainerWidget
    {
        FUSION_WIDGET(FExpanderBox, FContainerWidget)
    protected:
        
        FExpanderBox();

        void Construct() override;

        // - Layout -

        FVec2 MeasureContent(FVec2 availableSize) override;

        void ArrangeContent(FVec2 finalSize) override;

        // - Paint -

        void Paint(FPainter& painter) override;

    public:
        
        bool IsExpanded() const
        {
            return TestStyleState(EStyleState::Expanded);
        }

        void SetExpanded(bool expanded);

    protected:

        Ref<FButton> m_Header;
        Ref<FDecoratedBox> m_ContentBox;
        Ref<FWidget> m_Content;
        Ref<FLabel> m_TitleLabel;

        void SetupHeader();
        void SetupContent();

    public:

        // - Fusion Properties -

        FUSION_PROPERTY_SET(FButton&, Header)
        {
            if (self.m_Header == &value)
                return self;
            self.RemoveChildWidget(self.m_Header);
            self.m_Header = &value;
            self.AddChildWidget(&value);
            static_cast<FExpanderBox&>(self).SetupHeader();
            return self;
        }

        FUSION_PROPERTY_SET(FWidget&, Content)
        {
            if (self.m_Content == &value)
                return self;
            self.m_Content = &value;
            self.m_ContentBox->Child(value);
            static_cast<FExpanderBox&>(self).SetupContent();
            return self;
        }

        FUSION_PROPERTY_GET(bool, Expanded)
        {
            return IsExpanded();
        }

        FUSION_PROPERTY_SET(bool, Expanded)
        {
            SetExpanded(value);
            return self;
        }

        FUSION_STYLE_PROPERTIES(
            (FBrush, Background, Paint),
            (FPen,   Border,     Paint),
            (FShape, Shape,      Paint)
        );
        
    };

} // namespace Fusion
