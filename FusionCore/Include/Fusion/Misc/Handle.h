#pragma once

namespace Fusion
{

    template<typename T = u32> requires TFIsIntegralType<T>::Value
    class Handle
    {
    public:
        using IndexType = T;

        static constexpr T NullValue = T(-1);

        Handle() : value(NullValue)
        {

        }

        Handle(T value) : value(value)
        {

        }

        inline operator T() const
        {
            return value;
        }

        inline bool IsNull() const
        {
            return value == NullValue;
        }

        inline bool IsValid() const
        {
            return !IsNull();
        }

        inline T Get() const
        {
            return value;
        }

        inline void Reset()
        {
            value = NullValue;
        }

        SizeT GetHash() const
        {
            return ::Fusion::GetHash<T>(value);
        }

    private:
        T value = NullValue;
    };
    
} // namespace Fusion
