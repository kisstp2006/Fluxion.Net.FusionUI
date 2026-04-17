#include "Fusion/Widgets.h"

namespace Fusion
{
	FTextButton::FTextButton()
	{
		
	}

	void FTextButton::Construct()
	{
		Super::Construct();

		Child(
			FAssignNew(FLabel, m_Label)
			.Text("Button")
			.Color(FColors::White)
			.HAlign(EHAlign::Center)
			.VAlign(EVAlign::Center)
		);
	}

} // namespace Fusion
