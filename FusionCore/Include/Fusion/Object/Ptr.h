#pragma once

#include "Fusion/Object/RefCountBlock.h"
#include <concepts>
#include <cstddef>

namespace Fusion
{
    template<typename T> class WeakPtr;

    template<typename T>
    class Ptr
    {
    public:
        Ptr() = default;
        Ptr(std::nullptr_t) {}

        explicit Ptr(T* object)
        {
            if (object)
            {
                Internal::RefCountBlock* expected = nullptr;
                auto* newBlock = new Internal::RefCountBlock{ .m_Object = object };

                if (!object->m_Control.compare_exchange_strong(expected, newBlock,
                    std::memory_order_release, std::memory_order_acquire))
                {
                    // Another thread already created the control block
                    delete newBlock;
                    m_Control = expected;
                }
                else
                {
                    m_Control = newBlock;
                }

                m_Control->AddStrongRef();
            }
        }

        Ptr(const Ptr& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddStrongRef();
        }

        Ptr(Ptr&& other) noexcept
        {
            m_Control       = other.m_Control;
            other.m_Control = nullptr;
        }

        // Upcasting: Ptr<Derived> -> Ptr<Base>
        template<typename U> requires std::derived_from<U, T>
        Ptr(const Ptr<U>& other)
        {
            m_Control = other.m_Control;
            if (m_Control)
                m_Control->AddStrongRef();
        }

        template<typename U> requires std::derived_from<U, T>
        Ptr(Ptr<U>&& other) noexcept
        {
            m_Control       = other.m_Control;
            other.m_Control = nullptr;
        }

        ~Ptr()
        {
            if (m_Control)
                m_Control->ReleaseStrongRef();
        }

        Ptr& operator=(const Ptr& other)
        {
            if (this != &other)
            {
                if (m_Control)
                    m_Control->ReleaseStrongRef();

                m_Control = other.m_Control;
                if (m_Control)
                    m_Control->AddStrongRef();
            }
            return *this;
        }

        Ptr& operator=(Ptr&& other) noexcept
        {
            if (this != &other)
            {
                if (m_Control)
                    m_Control->ReleaseStrongRef();

                m_Control       = other.m_Control;
                other.m_Control = nullptr;
            }
            return *this;
        }

        Ptr& operator=(std::nullptr_t)
        {
            if (m_Control)
                m_Control->ReleaseStrongRef();
            m_Control = nullptr;
            return *this;
        }

        T* Get() const
        {
	        return m_Control ? static_cast<T*>(m_Control->m_Object) : nullptr;
        }

        T* operator->() const
        {
            FUSION_ASSERT_THROW(IsValid(), FNullPointerException, "Trying to dereference a Null Ptr<T>");
	        return Get();
        }

        T& operator*() const
        {
            FUSION_ASSERT_THROW(IsValid(), FNullPointerException, "Trying to dereference a Null Ptr<T>");
	        return *Get();
        }

        bool IsValid()    const { return Get() != nullptr; }
        bool IsNull()     const { return Get() == nullptr; }

        explicit operator bool() const
        {
	        return IsValid();
        }

        inline bool operator!() const
        {
            return !IsValid();
        }

        bool operator==(const Ptr& other)  const { return m_Control == other.m_Control; }
        bool operator!=(const Ptr& other)  const { return !(*this == other); }
        bool operator==(std::nullptr_t)    const { return IsNull(); }
        bool operator!=(std::nullptr_t)    const { return IsValid(); }

    private:
        Internal::RefCountBlock* m_Control = nullptr;

        // Internal constructor used by WeakPtr::Lock() — ref already incremented via TryAddStrongRef
        struct AlreadyRetained {};
        Ptr(Internal::RefCountBlock* control, AlreadyRetained)
            : m_Control(control) {}

        template<typename U> friend class Ptr;
        template<typename U> friend class WeakPtr;
        friend class FObject;
    };

} // namespace Fusion
