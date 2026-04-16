#include "Fusion/Widgets.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
    FTimer::FTimer() : Super("Timer")
    {
    }

    void FTimer::Start()
    {
    	m_Running = true;
        m_Accumulated = 0.0f;

        if (m_Registered)
            return;

        if (Ref<FWidget> widget = m_OwnerWidget.Lock())
        {
	        if (Ref<FApplicationInstance> application = widget->GetApplication())
	        {
                application->RegisterTimer(this);
                m_Registered = true;
	        }
        }
    }

    void FTimer::Stop()
    {
        m_Running = false;

        
    }

    void FTimer::Reset()
    {
        m_Accumulated = 0.0f;
    }

    void FTimer::OnBeforeDestroy()
    {
	    Super::OnBeforeDestroy();

        if (!m_Registered)
            return;

        if (Ref<FWidget> widget = m_OwnerWidget.Lock())
        {
            if (Ref<FApplicationInstance> application = widget->GetApplication())
            {
                application->DeregisterTimer(this);
            }
        }
    }

    void FTimer::OnAttachToOuter()
    {
	    Super::OnAttachToOuter();

        if (Ref<FObject> outer = GetOuter())
        {
	        if (Ref<FWidget> widget = CastTo<FWidget>(outer))
	        {
                m_OwnerWidget = widget;
	        }
        }
    }

    void FTimer::Tick(f32 deltaTime)
    {
        if (!m_Running)
            return;

        m_Accumulated += deltaTime;

        if (m_Accumulated >= m_Interval)
        {
            m_Accumulated -= m_Interval;

            m_OnTick.Broadcast();

            if (!m_Loop)
                Stop();
        }
    }

} // namespace Fusion
