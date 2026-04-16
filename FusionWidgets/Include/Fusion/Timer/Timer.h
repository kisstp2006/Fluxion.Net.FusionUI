#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FTimer : public FObject
    {
        FUSION_CLASS(FTimer, FObject)
    public:

        FTimer();

        // - API -

        void Start();
        void Stop();
        void Reset();

        bool IsRunning() const { return m_Running; }
        f32  GetInterval() const { return m_Interval; }
        bool IsLooping() const { return m_Loop; }

        // - Builder setters -

        FTimer& Interval(f32 seconds) { m_Interval = seconds; return *this; }
        FTimer& Loop(bool loop) { m_Loop = loop; return *this; }

        
        FTimer& Assign(FTimer*& out)
        {
            out = this;
            return *out;
        }

        FTimer& Assign(Ref<FTimer>& out)
        {
            out = this;
            return *out;
        }

        // - Signal -

        FUSION_SIGNAL(FVoidSignal, OnTick);

    protected:

        void OnBeforeDestroy() override;

        void OnAttachToOuter() override;

        void Tick(f32 deltaTime);

        f32  m_Interval    = 1.0f;
        f32  m_Accumulated = 0.0f;
        bool m_Loop        = false;
        bool m_Running     = false;
        bool m_Registered  = false;
        
    	WeakRef<FWidget> m_OwnerWidget = nullptr;

        friend class FApplicationInstance;
    };

} // namespace Fusion
