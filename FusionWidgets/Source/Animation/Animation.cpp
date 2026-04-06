#include "Fusion/Widgets.h"

namespace Fusion
{
	FAnimation::FAnimation(FName name) : Super(name)
	{
		
	}

	void FAnimation::Play()
    {
        m_ElapsedTime = 0.0f;
        m_DelayElapsed = 0.0f;
        m_Reversed = false;
        m_StartFired = false;
        m_State = EAnimationState::Playing;
    }

    void FAnimation::Pause()
    {
        if (m_State == EAnimationState::Playing)
        {
            m_State = EAnimationState::Paused;
        }
    }

    void FAnimation::Resume()
    {
        if (m_State == EAnimationState::Paused)
        {
            m_State = EAnimationState::Playing;
        }
    }

    void FAnimation::Stop()
    {
        m_ElapsedTime = 0.0f;
        m_DelayElapsed = 0.0f;
        m_Reversed = false;
        m_StartFired = false;
        m_State = EAnimationState::Idle;
    }

    void FAnimation::Restart()
    {
        Stop();
        Play();
    }

    void FAnimation::Tick(f32 deltaTime)
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
            m_DelayElapsed += deltaTime;
            if (m_DelayElapsed < m_Delay)
                return;

            m_StartFired = true;
            m_OnStart.Broadcast();
        }

        // Advance elapsed time — speed magnitude controls rate, sign controls direction
        m_ElapsedTime += deltaTime * FMath::Abs(speed);

        // Raw normalized time [0, inf)
        f32 rawT = m_Duration > 0.0f ? m_ElapsedTime / m_Duration : 1.0f;

        if (rawT >= 1.0f)
        {
            switch (m_LoopMode)
            {
            case EAnimationLoopMode::Once:
                rawT = 1.0f;
                m_State = EAnimationState::Completed;
                break;

            case EAnimationLoopMode::Loop:
                m_ElapsedTime = std::fmod(m_ElapsedTime, m_Duration);
                rawT = m_ElapsedTime / m_Duration;
                break;

            case EAnimationLoopMode::PingPong:
                m_ElapsedTime = std::fmod(m_ElapsedTime, m_Duration);
                rawT = m_ElapsedTime / m_Duration;
                m_Reversed = !m_Reversed;
                break;
            }
        }

        // speed < 0 and reversed XOR together to determine final direction
        const bool effectiveReversed = (speed < 0.0f) != m_Reversed;
        const f32 normalizedTime = effectiveReversed ? 1.0f - rawT : rawT;

        Apply(normalizedTime);
        //m_OnUpdate.Broadcast(normalizedTime);

        if (m_State == EAnimationState::Completed)
        {
            m_OnComplete.Broadcast();
        }
    }
} // namespace Fusion
