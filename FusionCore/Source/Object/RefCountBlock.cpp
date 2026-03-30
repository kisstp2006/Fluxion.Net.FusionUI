#include "Fusion/Object/RefCountBlock.h"
#include "Fusion/Object/Object.h"

namespace Fusion::Internal
{
    void RefCountBlock::AddStrongRef()
    {
        m_StrongRefs.fetch_add(1, std::memory_order_relaxed);
    }

    void RefCountBlock::ReleaseStrongRef()
    {
        if (m_StrongRefs.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            m_Object->m_Control.store(nullptr, std::memory_order_release);
            m_Object->OnDestroy();
            delete m_Object;
            m_Object = nullptr;

            bool shouldDestroy;
            {
                std::lock_guard lock(m_Mutex);
                m_Destroyed   = true;
                shouldDestroy = (m_WeakRefs.load(std::memory_order_relaxed) == 0);
            }

            if (shouldDestroy)
                SelfDestroy();
        }
    }

    void RefCountBlock::AddWeakRef()
    {
        m_WeakRefs.fetch_add(1, std::memory_order_relaxed);
    }

    void RefCountBlock::ReleaseWeakRef()
    {
        bool shouldDestroy;
        {
            std::lock_guard lock(m_Mutex);
            shouldDestroy = (m_WeakRefs.fetch_sub(1, std::memory_order_relaxed) == 1) && m_Destroyed.load();
        }

        if (shouldDestroy)
            SelfDestroy();
    }

    bool RefCountBlock::TryAddStrongRef()
    {
        uint32_t expected = m_StrongRefs.load(std::memory_order_relaxed);

        while (expected > 0)
        {
            if (m_StrongRefs.compare_exchange_weak(expected, expected + 1,
                std::memory_order_acq_rel, std::memory_order_relaxed))
                return true;
        }

        return false;
    }

    void RefCountBlock::SelfDestroy()
    {
        delete this;
    }

} // namespace Fusion::Internal
