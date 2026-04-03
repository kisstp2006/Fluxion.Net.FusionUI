#include "Fusion/Widgets.h"

namespace Fusion
{
	FBrush::FBrush()
	{
		
	}

	FBrush::FBrush(const FColor& fillColor, EBrushStyle brushStyle) : m_Color(fillColor), m_BrushStyle(brushStyle)
	{
		
	}

	FBrush::FBrush(const FGradient& gradient, const FColor& tintColor) : m_Gradient(gradient), m_Color(tintColor)
	{
		
	}

	FBrush::FBrush(const FName& imagePath, const FColor& tintColor) : m_ImagePath(imagePath), m_Color(tintColor)
	{
		
	}

	bool FBrush::operator==(const FBrush& rhs) const
	{
		if (m_BrushStyle != rhs.m_BrushStyle)
			return false;
		if (m_Tiling != rhs.m_Tiling)
			return false;
		if (m_ImageFit != rhs.m_ImageFit)
			return false;
		if (m_BrushPos != rhs.m_BrushPos)
			return false;
		if (m_BrushSize != rhs.m_BrushSize)
			return false;

		switch (m_BrushStyle)
		{
		case EBrushStyle::SolidFill:
			return m_Color == rhs.m_Color;
		case EBrushStyle::Gradient:
			return m_Color == rhs.m_Color && m_Gradient == rhs.m_Gradient;
		case EBrushStyle::Image:
			return m_Color == rhs.m_Color && m_ImagePath == rhs.m_ImagePath && (m_ImageFit != EImageFit::NineSlice || m_SliceMargins == rhs.m_SliceMargins);
		case EBrushStyle::None:
			break;
		}

		return false;
	}

} // namespace Fusion
