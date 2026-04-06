#pragma once

#include "Fusion/Object/Ref.h"
#include <concepts>

namespace Fusion
{
    template<typename T>
    class WeakRef
    {
    public:
        WeakRef() = default;
        WeakRef(std::nullptr_t) {}

        WeakRef(const Ref<T>& ptr)
        {
            m_Control = ptr.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        // Upcasting: WeakPtr<Derived> -> WeakPtr<Base>
        template<typename U> requires std::derived_from<U, T>
        WeakRef(const Ref<U>& ptr)
        {
            m_Control = ptr.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        WeakRef(const WeakRef& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        WeakRef(WeakRef&& other) noexcept
        {
            m_Control       = other.m_Control;
            other.m_Control = nullptr;
        }

        template<typename U> requires std::derived_from<U, T>
        WeakRef(const WeakRef<U>& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddWeakRef();
        }

        ~WeakRef()
        {
            if (m_Control)
                m_Control->ReleaseWeakRef();
        }

        WeakRef& operator=(const WeakRef& other)
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

        WeakRef& operator=(WeakRef&& other) noexcept
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

        WeakRef& operator=(std::nullptr_t)
        {
            if (m_Control)
                m_Control->ReleaseWeakRef();
            m_Control = nullptr;
            return *this;
        }

        // Attempts to promote to a strong Ptr<T>. Returns null if object was destroyed.
        Ref<T> Lock() const
        {
            if (m_Control && m_Control->TryAddStrongRef())
                return Ref<T>(m_Control, typename Ref<T>::AlreadyRetained{});

            return nullptr;
        }

        bool IsValid() const { return m_Control && m_Control->IsAlive(); }
        bool IsNull()  const { return !IsValid(); }
        explicit operator bool() const { return IsValid(); }

        bool operator==(const WeakRef& other) const { return m_Control == other.m_Control; }
        bool operator!=(const WeakRef& other) const { return !(*this == other); }
        bool operator==(std::nullptr_t)       const { return IsNull(); }
        bool operator!=(std::nullptr_t)       const { return IsValid(); }

    private:
        Internal::RefCountBlock* m_Control = nullptr;

        template<typename U> friend class WeakRef;
    };

    template <typename T>
    T* TPtrType<WeakRef<T>>::GetRawPtr(WeakRef<T> ptr)
    {
        return Ref<T>(ptr).Get();
    }

} // namespace Fusion
