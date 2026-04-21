#include "Fusion/Widgets.h"

namespace Fusion
{
	FSlottedWidget::FSlottedWidget()
	{
		
	}

	void FSlottedWidget::OnBeforeDestroy()
	{
		Super::OnBeforeDestroy();

		m_Slots.Clear();
	}

} // namespace Fusion
