#pragma once

#include <atomic>
#include <mutex>
#include <cstdint>

namespace Fusion
{
    class FObject;

    namespace Internal
    {
        // Separate control block kept alive as long as either strong or weak refs exist.
        // Users should never interact with this directly.
        struct RefCountBlock
        {
            FObject*              m_Object     = nullptr;
            std::atomic<uint32_t> m_StrongRefs = 0;
            std::atomic<uint32_t> m_WeakRefs   = 0;
            std::atomic<bool>     m_Destroyed  = false;
            std::mutex            m_Mutex;

            void AddStrongRef();
            void ReleaseStrongRef();
            void AddWeakRef();
            void ReleaseWeakRef();

            // Atomically increments strong ref only if object is still alive.
            // Returns true on success — used by WeakPtr::Lock().
            bool TryAddStrongRef();

            bool IsAlive() const { return !m_Destroyed.load(); }

        private:
            void SelfDestroy();
        };

    } // namespace Internal

} // namespace Fusion
