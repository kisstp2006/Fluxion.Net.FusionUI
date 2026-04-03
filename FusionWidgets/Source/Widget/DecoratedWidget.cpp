#include "Fusion/Widgets.h"

namespace Fusion
{
	FDecoratedWidget::FDecoratedWidget(Ref<FObject> outer) : Super(outer)
	{
		m_Shape = FRectangle();
	}

	void FDecoratedWidget::Paint(FPainter& painter)
	{
		Super::Paint(painter);

		painter.SetBrush(m_Background);
		painter.SetPen(m_Border);

		painter.FillAndStrokeShape(FRect(0, 0, GetLayoutSize().width, GetLayoutSize().height), m_Shape);
	}

} // namespace Fusion
