#include "Fusion/Widgets.h"

namespace Fusion
{
	FTextButton::FTextButton()
	{
		
	}

	void FTextButton::Construct()
	{
		Super::Construct();

		PropagatedStyleStates(EStyleState::All);
		StyleScopeBoundary(true);

		Child(
			FAssignNew(FLabel, m_Label)
			.SubStyle("Label")
			.Text("Button")
			.HAlign(EHAlign::Center)
			.VAlign(EVAlign::Center)
		);
	}

} // namespace Fusion
