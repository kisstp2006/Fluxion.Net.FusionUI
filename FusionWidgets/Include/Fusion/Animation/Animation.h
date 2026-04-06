#pragma once

namespace Fusion
{

    enum class EAnimationLoopMode
    {
        Once, Loop, PingPong
    };
    FUSION_ENUM_CLASS(EAnimationLoopMode);

    enum class EAnimationState
    {
        Idle, Playing, Paused, Completed
    };
    FUSION_ENUM_CLASS(EAnimationState);

    class FUSIONWIDGETS_API FAnimation : public FObject
    {
        FUSION_CLASS(FAnimation, FObject)
    protected:

        FAnimation(FName name);

    public:

        bool IsOwnerValid() const { return m_Owner.IsValid(); }

        EAnimationState GetState() const { return m_State; }

        f32 GetDuration() const { return m_Duration; }

        f32 GetDelay() const { return m_Delay; }

        void SetDelay(f32 value) { m_Delay = value; }

        void SetDuration(f32 value) { m_Duration = value; }

        void SetLoopMode(EAnimationLoopMode value) { m_LoopMode = value; }

        void AssignOwner(Ref<FObject> owner)
        {
            this->m_Owner = owner;
        }

        // - Playback -

        void Play();

        void Pause();

        void Resume();

        void Stop();

        void Restart();

        // - Tick -

        virtual void Tick(f32 deltaTime);

        virtual void Apply(f32 normalizedTime) = 0;

        FUSION_SIGNAL(FVoidSignal, OnStart);
        FUSION_SIGNAL(FVoidSignal, OnComplete);

    protected:

        
        f32 m_Duration = 0;

        
        f32 m_Delay = 0;

        
        f32 speed = 1.0f;
        
        EAnimationLoopMode m_LoopMode = EAnimationLoopMode::Once;

        WeakRef<FObject> m_Owner;

        f32 m_ElapsedTime = 0.0f;
        f32 m_DelayElapsed = 0.0f;
        bool m_Reversed = false;
        bool m_StartFired = false;
        EAnimationState m_State = EAnimationState::Idle;
    };
    
} // namespace Fusion
