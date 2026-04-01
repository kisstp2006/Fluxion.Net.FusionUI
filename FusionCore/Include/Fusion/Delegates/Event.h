#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#include "Fusion/Delegates/Delegate.h"
#include "Fusion/Containers/Array.h"

namespace Fusion
{

    struct FEventHandle
    {
        u32 ID = u32(-1);

        bool IsValid() const { return ID != u32(-1); }

        bool operator==(const FEventHandle&) const = default;
        bool operator!=(const FEventHandle&) const = default;

        static FEventHandle Invalid() { return {}; }
    };

    // ---------------------------------------------------------------------------

    template<typename TSignature>
    class FEvent;

    template<typename TReturn, typename... TArgs>
    class FEvent<TReturn(TArgs...)>
    {
    public:
        FEvent()  = default;
        ~FEvent() = default;

        // Non-copyable — copying an event's subscriber list is almost never intentional
        FEvent(const FEvent&)            = delete;
        FEvent& operator=(const FEvent&) = delete;

        FEvent(FEvent&&)            = default;
        FEvent& operator=(FEvent&&) = default;

        // ----------------------------------------------------------------
        // Add
        // ----------------------------------------------------------------

        // Lambda or free function
        template<typename TCallable>
        FEventHandle Add(TCallable&& callable)
        {
            FEventHandle handle{ m_NextID++ };
            auto& binding = m_Bindings.Emplace(handle.ID, FDelegate<TReturn(TArgs...)>{});
            binding.Delegate.Bind(std::forward<TCallable>(callable));
            return handle;
        }

        // Non-const member function (raw pointer)
        template<typename TObject, typename TMethod>
        FEventHandle Add(TObject* object, TMethod method)
        {
            FEventHandle handle{ m_NextID++ };
            auto& binding = m_Bindings.Emplace(handle.ID, FDelegate<TReturn(TArgs...)>{});
            binding.Delegate.Bind(object, method);
            return handle;
        }

        // Const member function (raw pointer)
        template<typename TObject, typename TMethod>
        FEventHandle Add(const TObject* object, TMethod method)
        {
            FEventHandle handle{ m_NextID++ };
            auto& binding = m_Bindings.Emplace(handle.ID, FDelegate<TReturn(TArgs...)>{});
            binding.Delegate.Bind(object, method);
            return handle;
        }

        // Non-const member function (Ptr<T>)
        template<typename TObject, typename TMethod>
        FEventHandle Add(Ptr<TObject> object, TMethod method)
        {
            FEventHandle handle{ m_NextID++ };
            auto& binding = m_Bindings.Emplace(handle.ID, FDelegate<TReturn(TArgs...)>{});
            binding.Delegate.Bind(object, method);
            return handle;
        }

        // Const member function (Ptr<const T>)
        template<typename TObject, typename TMethod>
        FEventHandle Add(Ptr<const TObject> object, TMethod method)
        {
            FEventHandle handle{ m_NextID++ };
            auto& binding = m_Bindings.Emplace(handle.ID, FDelegate<TReturn(TArgs...)>{});
            binding.Delegate.Bind(object, method);
            return handle;
        }

        // ----------------------------------------------------------------
        // Remove
        // ----------------------------------------------------------------

        void Remove(FEventHandle handle)
        {
            for (SizeT i = 0; i < m_Bindings.Size(); ++i)
            {
                if (m_Bindings[i].ID == handle.ID)
                {
                    if (m_Broadcasting)
                    {
                        // Defer removal — Broadcast() will sweep after the loop
                        m_Bindings[i].Delegate.Unbind();
                    }
                    else
                    {
                        m_Bindings.RemoveAt(i);
                    }
                    return;
                }
            }
        }

        void RemoveAll()
        {
            if (m_Broadcasting)
            {
                for (SizeT i = 0; i < m_Bindings.Size(); ++i)
                    m_Bindings[i].Delegate.Unbind();
            }
            else
            {
                m_Bindings.Clear();
            }
        }

        // ----------------------------------------------------------------
        // Broadcast
        // ----------------------------------------------------------------

        void Broadcast(TArgs... args)
        {
            m_Broadcasting = true;

            for (SizeT i = 0; i < m_Bindings.Size(); ++i)
            {
                if (m_Bindings[i].Delegate.IsBound())
                    m_Bindings[i].Delegate.Execute(std::forward<TArgs>(args)...);
            }

            m_Broadcasting = false;

            // Sweep any entries that were unbound during broadcast
            for (SizeT i = m_Bindings.Size(); i > 0; --i)
            {
                if (!m_Bindings[i - 1].Delegate.IsBound())
                    m_Bindings.RemoveAtSwapLast(i - 1);
            }
        }

        // ----------------------------------------------------------------
        // Query
        // ----------------------------------------------------------------

        bool IsBound()  const { return !m_Bindings.Empty(); }
        SizeT Count()   const { return m_Bindings.Size(); }

    private:
        struct FBinding
        {
            u32 ID = u32(-1);
            FDelegate<TReturn(TArgs...)> Delegate;
        };

        FArray<FBinding> m_Bindings;
        u32              m_NextID      = 0;
        bool             m_Broadcasting = false;
    };

} // namespace Fusion
