#pragma once

#include <type_traits>

namespace Fusion
{

    template<bool TValue>
    struct TFBoolConst
    {
        static constexpr bool Value = TValue;
    };

    struct TFFalseType : TFBoolConst<false> {};

    struct TFTrueType : TFBoolConst<true> {};

    template<typename T1, typename T2>
    struct TFIsSameType : TFFalseType {};

    template<typename T>
    struct TFIsSameType<T, T> : TFTrueType {};

    template<class TBase, class TDerived>
    using TFIsBaseClassOf = TFBoolConst<__is_base_of(TBase, TDerived)>;

    template<typename T>
    struct TFIsTemplate : TFFalseType {};

    template<typename T, template <typename> class U>
    struct TFIsTemplate<U<T>> : TFTrueType {};

    template<typename T, typename = void>
    struct TFHasGetHashFunction : TFFalseType
    {
        static SizeT GetHash(const T* instance) { return 0; }
    };

    template<typename T>
    struct TFHasGetHashFunction<T, std::void_t<decltype(std::declval<T>().GetHash())>> : TFTrueType
    {
        static SizeT GetHash(const T* instance) { return instance->GetHash(); }
    };

    template<typename T>
    constexpr std::remove_reference_t<T>&& MoveTemp(T&& value)
    {
        return static_cast<std::remove_reference_t<T>&&>(value);
    }
    
} // namespace Fusion
