#include "Fusion/Math/Mat4.h"

#include <cmath>

namespace Fusion
{

    FMat4 FMat4::PerspectiveProjection(float aspect, float fieldOfView, float nearPlane, float farPlane)
    {
        float tanHalfFOV = std::tan((fieldOfView * 0.5f) * (3.14159265358979323846f / 180.0f));
        float f = farPlane;
        float n = nearPlane;

        return FMat4({
            FVec4{ 1.0f / (aspect * tanHalfFOV), 0,                    0,                  0                   },
            FVec4{ 0,                             -1.0f / tanHalfFOV,  0,                  0                   },
            FVec4{ 0,                             0,                    f / (f - n),        -f * n / (f - n)    },
            FVec4{ 0,                             0,                    1,                  0                   },
        });
    }

    FMat4 FMat4::OrthographicProjection(float l, float r, float t, float b, float nearPlane, float farPlane)
    {
        return FMat4({
            FVec4{ 2.0f / (r - l), 0,              0,                                  -(r + l) / (r - l)                  },
            FVec4{ 0,              2.0f / (b - t),  0,                                  -(b + t) / (b - t)                  },
            FVec4{ 0,              0,              1.0f / (farPlane - nearPlane),       -nearPlane / (farPlane - nearPlane) },
            FVec4{ 0,              0,              0,                                  1                                   },
        });
    }

    FMat4 FMat4::operator+(const FMat4& rhs) const
    {
        FMat4 result{};

        for (int i = 0; i < 4; i++)
            result.rows[i] = rows[i] + rhs.rows[i];

        return result;
    }

    FMat4 FMat4::operator-(const FMat4& rhs) const
    {
        FMat4 result{};

        for (int i = 0; i < 4; i++)
            result.rows[i] = rows[i] - rhs.rows[i];

        return result;
    }

    FMat4 FMat4::operator*(float rhs) const
    {
        FMat4 result{};

        for (int i = 0; i < 4; i++)
            result.rows[i] = rows[i] * rhs;

        return result;
    }

    FMat4 FMat4::operator/(float rhs) const
    {
        FMat4 result{};

        for (int i = 0; i < 4; i++)
            result.rows[i] = rows[i] / rhs;

        return result;
    }

    FMat4 FMat4::Multiply(const FMat4& A, const FMat4& B)
    {
        FMat4 C;

        const float* B0 = B.rows[0].xyzw;
        const float* B1 = B.rows[1].xyzw;
        const float* B2 = B.rows[2].xyzw;
        const float* B3 = B.rows[3].xyzw;

        // Row 0
        {
            const float a0 = A.rows[0].xyzw[0], a1 = A.rows[0].xyzw[1];
            const float a2 = A.rows[0].xyzw[2], a3 = A.rows[0].xyzw[3];
            float* d = C.rows[0].xyzw;
            d[0] = a0 * B0[0] + a1 * B1[0] + a2 * B2[0] + a3 * B3[0];
            d[1] = a0 * B0[1] + a1 * B1[1] + a2 * B2[1] + a3 * B3[1];
            d[2] = a0 * B0[2] + a1 * B1[2] + a2 * B2[2] + a3 * B3[2];
            d[3] = a0 * B0[3] + a1 * B1[3] + a2 * B2[3] + a3 * B3[3];
        }
        // Row 1
        {
            const float a0 = A.rows[1].xyzw[0], a1 = A.rows[1].xyzw[1];
            const float a2 = A.rows[1].xyzw[2], a3 = A.rows[1].xyzw[3];
            float* d = C.rows[1].xyzw;
            d[0] = a0 * B0[0] + a1 * B1[0] + a2 * B2[0] + a3 * B3[0];
            d[1] = a0 * B0[1] + a1 * B1[1] + a2 * B2[1] + a3 * B3[1];
            d[2] = a0 * B0[2] + a1 * B1[2] + a2 * B2[2] + a3 * B3[2];
            d[3] = a0 * B0[3] + a1 * B1[3] + a2 * B2[3] + a3 * B3[3];
        }
        // Row 2
        {
            const float a0 = A.rows[2].xyzw[0], a1 = A.rows[2].xyzw[1];
            const float a2 = A.rows[2].xyzw[2], a3 = A.rows[2].xyzw[3];
            float* d = C.rows[2].xyzw;
            d[0] = a0 * B0[0] + a1 * B1[0] + a2 * B2[0] + a3 * B3[0];
            d[1] = a0 * B0[1] + a1 * B1[1] + a2 * B2[1] + a3 * B3[1];
            d[2] = a0 * B0[2] + a1 * B1[2] + a2 * B2[2] + a3 * B3[2];
            d[3] = a0 * B0[3] + a1 * B1[3] + a2 * B2[3] + a3 * B3[3];
        }
        // Row 3
        {
            const float a0 = A.rows[3].xyzw[0], a1 = A.rows[3].xyzw[1];
            const float a2 = A.rows[3].xyzw[2], a3 = A.rows[3].xyzw[3];
            float* d = C.rows[3].xyzw;
            d[0] = a0 * B0[0] + a1 * B1[0] + a2 * B2[0] + a3 * B3[0];
            d[1] = a0 * B0[1] + a1 * B1[1] + a2 * B2[1] + a3 * B3[1];
            d[2] = a0 * B0[2] + a1 * B1[2] + a2 * B2[2] + a3 * B3[2];
            d[3] = a0 * B0[3] + a1 * B1[3] + a2 * B2[3] + a3 * B3[3];
        }

        return C;
    }

    FVec4 FMat4::Multiply(const FMat4& lhs, const FVec4& rhs)
    {
        FVec4 out;

        const float x = rhs.xyzw[0];
        const float y = rhs.xyzw[1];
        const float z = rhs.xyzw[2];
        const float w = rhs.xyzw[3];

        // Row 0
        {
            const float* r = lhs.rows[0].xyzw;
            out.xyzw[0] = r[0] * x + r[1] * y + r[2] * z + r[3] * w;
        }
        // Row 1
        {
            const float* r = lhs.rows[1].xyzw;
            out.xyzw[1] = r[0] * x + r[1] * y + r[2] * z + r[3] * w;
        }
        // Row 2
        {
            const float* r = lhs.rows[2].xyzw;
            out.xyzw[2] = r[0] * x + r[1] * y + r[2] * z + r[3] * w;
        }
        // Row 3
        {
            const float* r = lhs.rows[3].xyzw;
            out.xyzw[3] = r[0] * x + r[1] * y + r[2] * z + r[3] * w;
        }

        return out;
    }

    FMat4 FMat4::GetTranspose(const FMat4& mat)
    {
        FMat4 result{};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                result.rows[i][j] = mat.rows[j][i];
        }

        return result;
    }

    FMat4 FMat4::GetInverse(const FMat4& m)
    {
        float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
        float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
        float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
        float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

        float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
        float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
        float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
        float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

        float det  = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
        float idet = 1.0f / det;

        FMat4 ret;

        ret[0][0] = t11 * idet;
        ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
        ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
        ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

        ret[1][0] = t12 * idet;
        ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
        ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
        ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

        ret[2][0] = t13 * idet;
        ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
        ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
        ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

        ret[3][0] = t14 * idet;
        ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
        ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
        ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

        return ret;
    }

} // namespace Fusion
