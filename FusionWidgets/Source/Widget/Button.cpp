#include "Fusion/Widgets.h"

namespace Fusion
{
	FButton::FButton()
	{
		
	}

	void FButton::OnMouseEnter(FMouseEvent& event)
	{
		Super::OnMouseEnter(event);

	}

	void FButton::OnMouseLeave(FMouseEvent& event)
	{
		Super::OnMouseLeave(event);

	}

	FEventReply FButton::OnMouseButtonDown(FMouseEvent& event)
	{
		if (event.IsLeftButton())
		{
			
		}
		return FEventReply::Handled();
	}

	FEventReply FButton::OnMouseButtonUp(FMouseEvent& event)
	{
		if (event.IsLeftButton())
		{
			if (event.bIsInside)
			{
				m_OnClick.Broadcast(this);
			}
		}
		return FEventReply::Handled();
	}

} // namespace Fusion
