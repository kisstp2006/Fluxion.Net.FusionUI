#include "Fusion/Widgets.h"

namespace Fusion
{

    FGradient FGradient::Linear(FVec2 start, FVec2 end)
    {
        FGradient g;
        g.m_Type = EGradientType::Linear;
        g.m_Params.Linear = { start, end };
        return g;
    }

    FGradient FGradient::Radial(FVec2 center, float radius)
    {
        FGradient g;
        g.m_Type = EGradientType::Radial;
        g.m_Params.Radial = { center, radius };
        return g;
    }

    FGradient FGradient::Conical(FVec2 center, float angle)
    {
        FGradient g;
        g.m_Type = EGradientType::Conical;
        g.m_Params.Conical = { center, angle };
        return g;
    }

    FGradient& FGradient::AddStop(FColor color, float position)
    {
        m_Stops.Add({ color, position });
        return *this;
    }

    FGradient& FGradient::SetExtend(EGradientExtend extend)
    {
        m_Extend = extend;
        return *this;
    }

    FVec2 FGradient::GetStartPoint() const
    {
        FUSION_ASSERT(m_Type == EGradientType::Linear, "FGradient: GetStartPoint called on non-linear gradient");
        return m_Params.Linear.Start;
    }

    FVec2 FGradient::GetEndPoint() const
    {
        FUSION_ASSERT(m_Type == EGradientType::Linear, "FGradient: GetEndPoint called on non-linear gradient");
        return m_Params.Linear.End;
    }

    FVec2 FGradient::GetCenter() const
    {
        FUSION_ASSERT(m_Type == EGradientType::Radial || m_Type == EGradientType::Conical,
            "FGradient: GetCenter called on non-radial/conical gradient");
        return m_Type == EGradientType::Radial ? m_Params.Radial.Center : m_Params.Conical.Center;
    }

    float FGradient::GetRadius() const
    {
        FUSION_ASSERT(m_Type == EGradientType::Radial, "FGradient: GetRadius called on non-radial gradient");
        return m_Params.Radial.Radius;
    }

    bool FGradient::operator==(const FGradient& rhs) const
    {
        if (m_Type != rhs.m_Type || m_Extend != rhs.m_Extend)
            return false;

        if (m_Stops.Size() != rhs.m_Stops.Size())
            return false;

        for (SizeT i = 0; i < m_Stops.Size(); i++)
        {
            const FGradientStop& a = m_Stops[i];
            const FGradientStop& b = rhs.m_Stops[i];
            if (a.Color != b.Color || !FMath::ApproxEquals(a.Position, b.Position))
                return false;
        }

        switch (m_Type)
        {
        case EGradientType::Linear:
            return m_Params.Linear.Start == rhs.m_Params.Linear.Start &&
                   m_Params.Linear.End   == rhs.m_Params.Linear.End;

        case EGradientType::Radial:
            return m_Params.Radial.Center == rhs.m_Params.Radial.Center &&
                   FMath::ApproxEquals(m_Params.Radial.Radius, rhs.m_Params.Radial.Radius);

        case EGradientType::Conical:
            return m_Params.Conical.Center == rhs.m_Params.Conical.Center &&
                   FMath::ApproxEquals(m_Params.Conical.Angle, rhs.m_Params.Conical.Angle);
        }

        return false;
    }

    float FGradient::GetAngle() const
    {
        FUSION_ASSERT(m_Type == EGradientType::Conical, "FGradient: GetAngle called on non-conical gradient");
        return m_Params.Conical.Angle;
    }

} // namespace Fusion
