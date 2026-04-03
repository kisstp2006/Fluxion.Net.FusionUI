#include "Fusion/Widgets.h"

namespace Fusion
{

	FPen::FPen(const FColor& color, f32 thickness, EPenStyle style) : m_Color(color), m_Thickness(thickness), m_Style(style)
	{
		
	}

	FPen::FPen(const FGradient& gradient, f32 thickness, EPenStyle style, const FColor& tintColor) 
		: m_Gradient(gradient), m_Color(tintColor), m_Thickness(thickness), m_Style(style)
	{
		
	}

}
