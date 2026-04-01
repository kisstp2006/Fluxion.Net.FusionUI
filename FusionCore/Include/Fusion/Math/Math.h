#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <initializer_list>
#include <limits>

namespace Fusion
{
    
    class FUSIONCORE_API FMath
    {
    public:
        FMath() = delete;
        ~FMath() = delete;

        static constexpr f32 PI = (f32)M_PI;

        static u16 Tof16(f32 f32Value)
        {
            f32* ptr = &f32Value;
            unsigned int fltInt32 = *((uint32_t*)ptr);

            u16 fltInt16 = (fltInt32 >> 31) << 5;
            u16 tmp = (fltInt32 >> 23) & 0xff;
            tmp = (tmp - 0x70) & ((unsigned int)((int)(0x70 - tmp) >> 4) >> 27);
            fltInt16 = (fltInt16 | tmp) << 10;
            fltInt16 |= (fltInt32 >> 13) & 0x3ff;

            return fltInt16;
        }

        // Needs to be tested!
        static f32 Tof32(u16 f3216)
        {
            uint32_t t1;
            uint32_t t2;
            uint32_t t3;

            t1 = f3216 & 0x7fffu;                       // Non-sign bits
            t2 = f3216 & 0x8000u;                       // Sign bit
            t3 = f3216 & 0x7c00u;                       // Exponent

            t1 <<= 13u;                              // Align mantissa on MSB
            t2 <<= 16u;                              // Shift sign bit into position

            t1 += 0x38000000;                       // Adjust bias

            t1 = (t3 == 0 ? 0 : t1);                // Denormals-as-zero

            t1 |= t2;                               // Re-insert sign bit

            return *(f32*)(&t1);
        }

        static inline bool ApproxEquals(f32 a, f32 b) { return fpclassify(a - b) == FP_ZERO; }
        static inline bool ApproxEquals(double a, double b) { return fpclassify(a - b) == FP_ZERO; }
        static inline bool ApproxEquals(int a, int b) { return a == b; }

        static constexpr f32 Infinity() { return std::numeric_limits<f32>::infinity(); }

        inline static constexpr f32 Rad2Deg(f32 radians) { return radians * 180.0f / (f32)M_PI; }
        inline static constexpr f32 Deg2Rad(f32 degrees) { return degrees * (f32)M_PI / 180.0f; }

        inline static f32 Sin(f32 radians) { return sin(radians); }
        inline static f32 ASin(f32 sine) { return asin(sine); }

        inline static f32 Cos(f32 radians) { return cos(radians); }
        inline static f32 ACos(f32 cosine) { return acos(cosine); }

        inline static f32 Tan(f32 radians) { return tan(radians); }
        inline static f32 ATan(f32 tangent) { return atan(tangent); }

        inline static f32 Abs(f32 value) { return abs(value); }
        inline static double Abs(double value) { return abs(value); }

        inline static int Abs(int value) { return abs(value); }
        inline static int64_t Abs(int64_t value) { return abs(value); }

        inline static f32 Sqrt(f32 value) { return sqrt(value); }

        template<typename T1, typename T2>
        static auto Pow(T1 base, T2 power) { return pow(base, power); }

        template <typename T>
        constexpr static bool IsPowerOf2(T inV)
        {
            return inV > 0 && (inV & (inV - 1)) == 0;
        }

        template<typename T>
        constexpr static T Min(std::initializer_list<T> list)
        {
            auto min = std::numeric_limits<T>::max();

            for (auto entry : list)
            {
                if (entry < min)
                    min = entry;
            }

            return min;
        }

        template<typename T>
        constexpr static T Max(std::initializer_list<T> list)
        {
            auto max = std::numeric_limits<T>::min();
            
            for (auto entry : list)
            {
                if (entry > max)
                    max = entry;
            }

            return max;
        }

        inline static f32 Round(f32 value)
        {
            return std::round(value);
        }

        inline static double Round(double value)
        {
            return std::round(value);
        }

        inline static int RoundToInt(f32 value)
        {
            return std::lround(value);
        }

        inline static int64_t RoundToInt64(f32 value)
        {
            return std::llround(value);
        }

        inline static int RoundToInt(double value)
        {
            return std::lround(value);
        }

        inline static int64_t RoundToInt64(double value)
        {
            return std::llround(value);
        }

        template<typename T>
        inline static T Min(T a, T b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        inline static T Max(T a, T b)
        {
            return a > b ? a : b;
        }

        template<typename T>
        inline static constexpr T Clamp(T value, T min, T max)
        {
            if (min > max)
                std::swap(min, max);
	        return Min(Max(value, min), max);
        }

        template<typename T>
        inline static constexpr T Clamp01(T value) { return Clamp<T>(value, 0, 1); }

        inline static f32 Lerp(f32 from, f32 to, f32 t) { return from * (1 - Clamp01(t)) + to * Clamp01(t); }

        inline static f32 LerpUnclamped(f32 from, f32 to, f32 t) { return from * (1 - t) + to * (t); }
        
    private:

    };

	template<typename T>
    inline bool IsNan(T value)
	{
		return std::isnan(value);
	}

} // namespace Fusion
