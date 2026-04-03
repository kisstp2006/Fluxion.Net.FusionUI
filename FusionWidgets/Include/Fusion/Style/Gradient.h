#pragma once

namespace Fusion
{

    enum class EGradientType
    {
	    Linear,
    	Radial,
    	Conical
    };
    FUSION_ENUM_CLASS(EGradientType);

	enum class EGradientExtend
	{
		Clamp,
		Repeat,
		Reflect
	};
    FUSION_ENUM_CLASS(EGradientExtend);

    struct FGradientStop
    {
        FColor Color;
        float Position = 0.0f; // 0-1
    };

    struct FUSIONWIDGETS_API FGradient
    {
    public:

        // - Factory Methods -

        static FGradient Linear(FVec2 start, FVec2 end);
        static FGradient Radial(FVec2 center, float radius);
        static FGradient Conical(FVec2 center, float angle);

        // - Builder -

        FGradient& AddStop(FColor color, float position);
        FGradient& SetExtend(EGradientExtend extend);

        // - Getters -

        bool IsValid() const { return m_Stops.Size() >= 2; }
        bool IsLinear() const { return m_Type == EGradientType::Linear; }
        bool IsRadial() const { return m_Type == EGradientType::Radial; }
        bool IsConical() const { return m_Type == EGradientType::Conical; }

        bool operator==(const FGradient& rhs) const;
        bool operator!=(const FGradient& rhs) const { return !(*this == rhs); }

        EGradientType               GetType()   const { return m_Type; }
        EGradientExtend             GetExtend() const { return m_Extend; }
        const FArray<FGradientStop>& GetStops() const { return m_Stops; }

        // Linear
        FVec2 GetStartPoint() const;
        FVec2 GetEndPoint()   const;

        // Radial + Conical
        FVec2 GetCenter() const;

        // Radial
        float GetRadius() const;

        // Conical
        float GetAngle()  const;

        FGradient() = default;

    private:

        EGradientType m_Type = EGradientType::Linear;
        EGradientExtend m_Extend = EGradientExtend::Clamp;
        FArray<FGradientStop> m_Stops;

        union
        {
            struct { FVec2 Start, End; } Linear;
            struct { FVec2 Center; float Radius; } Radial;
            struct { FVec2 Center; float Angle; } Conical;
        } m_Params = {};
    };

} // namespace Fusion
