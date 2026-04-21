#include "Fusion/Widgets.h"

namespace Fusion
{
	FExpanderBox::FExpanderBox()
	{
		
	}

	void FExpanderBox::Construct()
	{
		Super::Construct();

		Header(
			FNew(FButton)
			.HAlign(EHAlign::Fill)
			.Child(
				FNew(FHorizontalStack)
				.HAlign(EHAlign::Fill)
				.VAlign(EVAlign::Fill)
				
			)
		);
	}

	void FExpanderBox::Paint(FPainter& painter)
	{
		Super::Paint(painter);

		FVec2 layoutSize = GetLayoutSize();
		FRect widgetRect(0, 0, layoutSize.width, layoutSize.height);

		painter.SetBrush(Background());
		painter.SetPen(Border());
		painter.FillAndStrokeShape(widgetRect, Shape());
	}

	void FExpanderBox::SetupHeader()
	{
		if (!m_Header)
			return;


	}

	void FExpanderBox::SetupContent()
	{
		if (!m_Content)
			return;


	}

} // namespace Fusion
