#pragma once

#include "Fusion/Object/Ptr.h"
#include <concepts>

namespace Fusion
{
    template<typename T>
    class WeakPtr
    {
    public:
        WeakPtr() = default;
        WeakPtr(std::nullptr_t) {}

        WeakPtr(const Ptr<T>& ptr)
        {
            m_Control = ptr.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        // Upcasting: WeakPtr<Derived> -> WeakPtr<Base>
        template<typename U> requires std::derived_from<U, T>
        WeakPtr(const Ptr<U>& ptr)
        {
            m_Control = ptr.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        WeakPtr(const WeakPtr& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        WeakPtr(WeakPtr&& other) noexcept
        {
            m_Control       = other.m_Control;
            other.m_Control = nullptr;
        }

        template<typename U> requires std::derived_from<U, T>
        WeakPtr(const WeakPtr<U>& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        ~WeakPtr()
        {
            if (m_Control)
                m_Control->ReleaseWeakRef();
        }

        WeakPtr& operator=(const WeakPtr& other)
        {
            if (this != &other)
            {
                if (m_Control)
                    m_Control->ReleaseWeakRef();

                m_Control = other.m_Control;
                if (m_Control)
                    m_Control->AddWeakRef();
            }
            return *this;
        }

        WeakPtr& operator=(WeakPtr&& other) noexcept
        {
            if (this != &other)
            {
                if (m_Control)
                    m_Control->ReleaseWeakRef();

                m_Control       = other.m_Control;
                other.m_Control = nullptr;
            }
            return *this;
        }

        WeakPtr& operator=(std::nullptr_t)
        {
            if (m_Control)
                m_Control->ReleaseWeakRef();
            m_Control = nullptr;
            return *this;
        }

        // Attempts to promote to a strong Ptr<T>. Returns null if object was destroyed.
        Ptr<T> Lock() const
        {
            if (m_Control && m_Control->TryAddStrongRef())
                return Ptr<T>(m_Control, typename Ptr<T>::AlreadyRetained{});

            return nullptr;
        }

        bool IsValid() const { return m_Control && m_Control->IsAlive(); }
        bool IsNull()  const { return !IsValid(); }
        explicit operator bool() const { return IsValid(); }

        bool operator==(const WeakPtr& other) const { return m_Control == other.m_Control; }
        bool operator!=(const WeakPtr& other) const { return !(*this == other); }
        bool operator==(std::nullptr_t)       const { return IsNull(); }
        bool operator!=(std::nullptr_t)       const { return IsValid(); }

    private:
        Internal::RefCountBlock* m_Control = nullptr;

        template<typename U> friend class WeakPtr;
    };

} // namespace Fusion
