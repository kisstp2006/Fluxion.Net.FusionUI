#pragma once

#include <cstring>
#include <initializer_list>

#include "Fusion/Math/Vec4.h"

namespace Fusion
{

    // ----------------------------------------------------------------
    // FMat4 — Row-major 4x4 float matrix
    // Memory layout matches CE::Matrix4x4: rows[4] of FVec4 (64 bytes)
    // ----------------------------------------------------------------
    struct FUSIONCORE_API FMat4
    {
        FVec4 rows[4];

        FMat4()
        {
            memset(rows, 0, sizeof(rows));
        }

        FMat4(FVec4 r[4])
        {
            for (int i = 0; i < 4; i++)
                rows[i] = r[i];
        }

        FMat4(std::initializer_list<FVec4> r)
        {
            for (int i = 0; i < 4; i++)
                rows[i] = *(r.begin() + i);
        }

        FMat4(const FMat4& copy)
        {
            memcpy(rows, copy.rows, sizeof(rows));
        }

        // ----------------------------------------------------------------
        // Static factories
        // ----------------------------------------------------------------

        static FMat4 Zero()
        {
            static FVec4 r[4] = {
                FVec4{ 0, 0, 0, 0 },
                FVec4{ 0, 0, 0, 0 },
                FVec4{ 0, 0, 0, 0 },
                FVec4{ 0, 0, 0, 0 },
            };
            return FMat4(r);
        }

        static FMat4 Identity()
        {
            static FVec4 r[4] = {
                FVec4{ 1, 0, 0, 0 },
                FVec4{ 0, 1, 0, 0 },
                FVec4{ 0, 0, 1, 0 },
                FVec4{ 0, 0, 0, 1 },
            };
            return FMat4(r);
        }

        static FMat4 Translation(float tx, float ty, float tz)
        {
            return FMat4({
                FVec4{ 1, 0, 0, tx },
                FVec4{ 0, 1, 0, ty },
                FVec4{ 0, 0, 1, tz },
                FVec4{ 0, 0, 0, 1  },
            });
        }

        static FMat4 Scale(float sx, float sy)
        {
            return FMat4({
                FVec4{ sx, 0,  0, 0 },
                FVec4{ 0,  sy, 0, 0 },
                FVec4{ 0,  0,  1, 0 },
                FVec4{ 0,  0,  0, 1 },
            });
        }

        static FMat4 Scale(float sx, float sy, float sz)
        {
            return FMat4({
                FVec4{ sx, 0,  0,  0 },
                FVec4{ 0,  sy, 0,  0 },
                FVec4{ 0,  0,  sz, 0 },
                FVec4{ 0,  0,  0,  1 },
            });
        }

        static FMat4 PerspectiveProjection(float aspect, float fieldOfView, float nearPlane, float farPlane);

        static FMat4 OrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane);

        static FMat4 OrthographicProjection(float aspect, float nearPlane, float farPlane)
        {
            return OrthographicProjection(-aspect, aspect, -1.0f, 1.0f, nearPlane, farPlane);
        }

        // ----------------------------------------------------------------
        // Element access
        // ----------------------------------------------------------------

        FVec4& operator[](int index) { return rows[index]; }
        const FVec4& operator[](int index) const { return rows[index]; }

        // ----------------------------------------------------------------
        // Arithmetic
        // ----------------------------------------------------------------

        FMat4 operator+(const FMat4& rhs) const;
        FMat4 operator-(const FMat4& rhs) const;

        FMat4& operator+=(const FMat4& rhs) { *this = *this + rhs; return *this; }
        FMat4& operator-=(const FMat4& rhs) { *this = *this - rhs; return *this; }

        FMat4 operator*(float rhs) const;
        FMat4 operator/(float rhs) const;

        FMat4& operator*=(float rhs) { *this = *this * rhs; return *this; }
        FMat4& operator/=(float rhs) { *this = *this / rhs; return *this; }

        FMat4 operator*(const FMat4& rhs) const { return Multiply(*this, rhs); }
        FVec4 operator*(const FVec4& rhs)  const { return Multiply(*this, rhs); }

        FMat4& operator*=(const FMat4& rhs) { *this = *this * rhs; return *this; }

        bool operator==(const FMat4& rhs) const
        {
            return rows[0] == rhs.rows[0] && rows[1] == rhs.rows[1] &&
                   rows[2] == rhs.rows[2] && rows[3] == rhs.rows[3];
        }

        bool operator!=(const FMat4& rhs) const { return !(*this == rhs); }

        // ----------------------------------------------------------------
        // Matrix operations
        // ----------------------------------------------------------------

        static FMat4 Multiply(const FMat4& lhs, const FMat4& rhs);
        static FVec4 Multiply(const FMat4& lhs, const FVec4& rhs);
        static FMat4 GetTranspose(const FMat4& mat);
        static FMat4 GetInverse(const FMat4& mat);

        FMat4 GetTranspose() const { return GetTranspose(*this); }
        void  Transpose()          { *this = GetTranspose(*this); }

        FMat4 GetInverse() const { return GetInverse(*this); }
        void  Invert()           { *this = GetInverse(*this); }
    };

    inline FMat4 operator*(float scalar, const FMat4& m) { return m * scalar; }

} // namespace Fusion
