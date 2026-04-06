#include "Fusion/Widgets.h"

namespace Fusion
{
	FSpringAnimation::FSpringAnimation(FName name) : Super(name)
	{
		
	}

	void FSpringAnimation::Tick(f32 dt)
	{
        if (m_State != EAnimationState::Playing)
            return;

        ZoneScoped;

        if (!IsOwnerValid())
        {
            Stop();
            return;
        }

        // Handle delay
        if (!m_StartFired)
        {
            m_DelayElapsed += dt;
            if (m_DelayElapsed < m_Delay)
                return;

            m_StartFired = true;
            m_OnStart.Broadcast();
        }

        if (!springState)
            return;

        springState->Tick(dt * FMath::Abs(speed));

        if (springState->HasSettled())
        {
            m_State = EAnimationState::Completed;
            m_OnComplete.Broadcast();
        }
	}
} // namespace Fusion
