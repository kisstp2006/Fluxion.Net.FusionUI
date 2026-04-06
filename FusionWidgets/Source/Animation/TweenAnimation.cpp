#include "Fusion/Widgets.h"

namespace Fusion
{
	FTweenAnimation::FTweenAnimation(FName name) : Super(MoveTemp(name))
	{
		
	}

	void FTweenAnimation::Apply(f32 normalizedTime)
	{
		if (m_Interpolator)
		{
			m_Interpolator->Apply(normalizedTime);
		}
	}
} // namespace Fusion
