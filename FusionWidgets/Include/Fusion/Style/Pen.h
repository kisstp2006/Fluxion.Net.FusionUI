#pragma once

namespace Fusion
{
	enum class EPenStyle : u32
	{
		None,
		Solid,
		Dashed, // Uses dashLength and dashGap
		Dotted  // Uses dashLength and dashGap (typically dashLength == thickness for round dots)
	};
	FUSION_ENUM_CLASS(EPenStyle);
    
    class FUSIONWIDGETS_API FPen final
    {
    public:

		FPen() = default;

		FPen(const FColor& color, f32 thickness = 1.0f, EPenStyle style = EPenStyle::Solid);

		// Gradient pen. tintColor is multiplied with the gradient output (White = no tint).
		FPen(const FGradient& gradient, f32 thickness = 1.0f, EPenStyle style = EPenStyle::Solid, const FColor& tintColor = FColors::White);

		const FColor& GetColor() const { return m_Color; }
		void SetColor(const FColor& penColor) { this->m_Color = penColor; }

		const FGradient& GetGradient() const { return m_Gradient; }
		void SetGradient(const FGradient& g) { this->m_Gradient = g; }

		bool HasGradient() const { return m_Gradient.IsValid(); }

		f32 GetThickness() const { return m_Thickness; }
		void SetThickness(f32 thickness) { this->m_Thickness = thickness; }

		EPenStyle GetStyle() const { return m_Style; }
		void SetStyle(EPenStyle penStyle) { this->m_Style = penStyle; }

		// Length of each dash segment in pixels. Only used when style is Dashed or Dotted.
		f32 GetDashLength() const { return m_DashLength; }
		void SetDashLength(f32 dashLength) { this->m_DashLength = dashLength; }

		// Gap between dash segments in pixels. Only used when style is Dashed or Dotted.
		f32 GetDashGap() const { return m_DashGap; }
		void SetDashGap(f32 dashGap) { this->m_DashGap = dashGap; }

		// Normalized offset (0-1) applied to gradient UV along the stroke arc-length.
		// Animating this from 0 to 1 makes the gradient appear to travel along the stroke.
		// Only affects gradient strokes; ignored for solid pens and fills.
		f32 GetGradientOffset() const { return m_GradientOffset; }
		void SetGradientOffset(f32 offset) { this->m_GradientOffset = offset; }

		FPen& GradientOffset(f32 value)
		{
			m_GradientOffset = value;
			return *this;
		}

		// Arc-length offset in points applied to the start of the dash/dot pattern.
		f32 GetDashPhase() const { return m_DashPhase; }
		void SetDashPhase(f32 phase) { this->m_DashPhase = phase; }

		FPen& DashPhase(f32 value)
		{
			m_DashPhase = value;
			return *this;
		}

		// Computes the initial dashOffset and inDash state from dashPhase.
		// Uses dashLength as the "on" length for Dashed, and 2pt for Dotted.
		void InitDashState(f32& outDashOffset, bool& outInDash) const
		{
			constexpr f32 dotLen = 2.0f;
			const f32 onLen = (m_Style == EPenStyle::Dotted) ? dotLen : m_DashLength;
			const f32 period = onLen + m_DashGap;
			const f32 phase = period > 0.0f ? fmod(m_DashPhase, period) : 0.0f;
			outInDash = phase < onLen;
			outDashOffset = outInDash ? phase : phase - onLen;
		}

		bool IsValidPen() const
		{
			return (HasGradient() || m_Color.a > 0.001f) && m_Thickness > 0.01f;
		}

    private:

		
		FGradient m_Gradient;
		
		FColor m_Color;

		
		f32 m_Thickness = 0.0f;

		// Length of each dash in pixels. Only relevant for Dashed/Dotted styles.
		f32 m_DashLength = 5.0f;

		// Gap between dashes in pixels. Only relevant for Dashed/Dotted styles.
		f32 m_DashGap = 5.0f;

		
		EPenStyle m_Style = EPenStyle::None;

		// Normalized offset applied to gradient UV along the stroke arc-length. Only used for gradient strokes.
		f32 m_GradientOffset = 0.0f;

		// Arc-length offset in points applied to the start of the dash/dot pattern.
		// Increasing by 1 moves dashes forward by 1pt. Only used when style is Dashed or Dotted.
		f32 m_DashPhase = 0.0f;

    };

} // namespace Fusion
