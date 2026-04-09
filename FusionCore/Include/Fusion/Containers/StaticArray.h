#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <initializer_list>

#include "Fusion/Misc/Assert.h"


namespace Fusion
{
    //! @brief A fixed-capacity array whose size is determined at compile time.
    //!        All N elements are always alive (constructed). Analogous to std::array
    //!        but follows the Fusion naming and API conventions.
    template<typename T, size_t N>
    class FStaticArray
    {
        static_assert(N > 0, "FStaticArray: N must be greater than zero");

    public:
        static constexpr size_t Extent = N;
        static constexpr i64    npos   = -1;

        // ----------------------------------------------------------------
        // Construction / Destruction
        // ----------------------------------------------------------------

        FStaticArray() = default;

        FStaticArray(std::initializer_list<T> list)
        {
            FUSION_ASSERT(list.size() <= N, "FStaticArray: initializer_list is larger than the array extent");
            size_t i = 0;
            for (const T& value : list)
                m_Data[i++] = value;
        }

        FStaticArray(const FStaticArray&) = default;
        FStaticArray(FStaticArray&&) noexcept = default;

        ~FStaticArray() = default;

        // ----------------------------------------------------------------
        // Assignment
        // ----------------------------------------------------------------

        FStaticArray& operator=(const FStaticArray&) = default;
        FStaticArray& operator=(FStaticArray&&) noexcept = default;

        // ----------------------------------------------------------------
        // Element access
        // ----------------------------------------------------------------

        T&       operator[](size_t index)       { FUSION_ASSERT(index < N, "FStaticArray: index out of bounds"); return m_Data[index]; }
        const T& operator[](size_t index) const { FUSION_ASSERT(index < N, "FStaticArray: index out of bounds"); return m_Data[index]; }

        T&       First()       { return m_Data[0]; }
        const T& First() const { return m_Data[0]; }

        T&       Last()        { return m_Data[N - 1]; }
        const T& Last()  const { return m_Data[N - 1]; }

        T*       Data()        { return m_Data; }
        const T* Data()  const { return m_Data; }

        // ----------------------------------------------------------------
        // Capacity
        // ----------------------------------------------------------------

        //! @brief Both Size() and Count() return the number of elements (always equal to N).
        constexpr size_t Size()     const { return N; }
        //! @brief Both Size() and Count() return the number of elements (always equal to N).
        constexpr size_t Count()    const { return N; }
        constexpr size_t Capacity() const { return N; }
        constexpr bool   Empty()    const { return N == 0; }

        // ----------------------------------------------------------------
        // Modifiers
        // ----------------------------------------------------------------

        void Fill(const T& value)
        {
            for (size_t i = 0; i < N; ++i)
                m_Data[i] = value;
        }

        // ----------------------------------------------------------------
        // Search
        // ----------------------------------------------------------------

        bool Contains(const T& value) const
        {
            for (size_t i = 0; i < N; ++i)
                if (m_Data[i] == value)
                    return true;
            return false;
        }

        i64 IndexOf(const T& value) const
        {
            for (size_t i = 0; i < N; ++i)
                if (m_Data[i] == value)
                    return static_cast<i64>(i);
            return npos;
        }

        // ----------------------------------------------------------------
        // Iterators
        // ----------------------------------------------------------------

        T*       begin()       { return m_Data; }
        T*       end()         { return m_Data + N; }
        const T* begin() const { return m_Data; }
        const T* end()   const { return m_Data + N; }

    private:
        T m_Data[N] = {};
    };

} // namespace Fusion
