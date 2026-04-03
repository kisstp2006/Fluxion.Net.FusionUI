#pragma once

namespace Fusion
{
	// Determines what content the brush renders.
	
	enum class EBrushStyle : u8
	{
		None = 0,
		SolidFill,  // Filled with a solid color (uses color)
		Image,      // Textured fill (uses imageName, color as tint, imageFit, brushSize, brushPos)
		Gradient    // Gradient fill (uses gradient, color as tint)
	};
	FUSION_ENUM_CLASS(EBrushStyle);

	// Controls how an image brush tiles when brushSize is smaller than the widget rect.
	// Only applies to FBrushStyle::Image. Mutually exclusive with NineSlice.
	enum class EBrushTiling : u8
	{
		None = 0,
		TileX,   // Repeat horizontally
		TileY,   // Repeat vertically
		TileXY   // Repeat in both axes
	};
	FUSION_ENUM_CLASS(EBrushTiling);

	// Controls how an image brush is fitted into the widget rect.
	// FPainter computes vertex UVs from this at tessellation time.
	enum class EImageFit : u8
	{
		Fill = 0, // Stretch to fill the widget rect exactly (default, ignores aspect ratio)
		Contain,  // Scale uniformly to fit within the rect, preserving aspect ratio (may letterbox)
		Cover,    // Scale uniformly to fill the rect, preserving aspect ratio (may crop)
		NineSlice // Fixed corners, stretched edges/center. Requires sliceMargins to be set.
	};
	FUSION_ENUM_CLASS(EImageFit);

    class FUSIONWIDGETS_API FBrush final
    {
    public:

		FBrush();

		FBrush(const FColor& fillColor, EBrushStyle brushStyle = EBrushStyle::SolidFill);

		// Image brush. tintColor is multiplied with the texture color (White = no tint).
		FBrush(const FName& imagePath, const FColor& tintColor = FColors::White);

		// Gradient brush. tintColor is multiplied with the gradient output (White = no tint).
		FBrush(const FGradient& gradient, const FColor& tintColor = FColors::White);

		bool IsValidBrush();

		bool IsTiled() const { return m_Tiling != EBrushTiling::None; }

		EBrushStyle GetBrushStyle() const { return m_BrushStyle; }
		EBrushTiling GetBrushTiling() const { return m_Tiling; }
		const FGradient& GetGradient() const { return m_Gradient; }

		void SetBrushStyle(EBrushStyle brushStyle) { m_BrushStyle = brushStyle; }
		void SetBrushTiling(EBrushTiling tiling) { m_Tiling = tiling; }

		// For SolidFill: the fill color. For Image/Gradient: the tint color (multiplied with texture/gradient output).
		const FColor& GetColor() const { return m_Color; }
		void SetColor(const FColor& color) { m_Color = color; }

		const FName& GetImagePath() const { return m_ImagePath; }

		EImageFit GetImageFit() const { return m_ImageFit; }
		void SetImageFit(EImageFit imageFit) { m_ImageFit = imageFit; }

		// Border sizes for NineSlice fitting (left, top, right, bottom in pixels).
		// Only used when imageFit == FImageFit::NineSlice.
		const FMargin& GetSliceMargins() const { return m_SliceMargins; }
		void SetSliceMargins(const FMargin& margins) { m_SliceMargins = margins; }

		// Explicit pixel size of the image within the widget rect. Vec2(-1,-1) = auto (driven by imageFit).
		const FVec2& GetBrushSize() const { return m_BrushSize; }
		void SetBrushSize(FVec2 brushSize) { m_BrushSize = brushSize; }

		// Normalized anchor point for image placement within the widget rect.
		// (0,0) = top-left, (0.5,0.5) = centered, (1,1) = bottom-right. Equivalent to CSS background-position.
		const FVec2& GetBrushPosition() const { return m_BrushPos; }
		void SetBrushPosition(FVec2 brushPos) { m_BrushPos = brushPos; }

		bool operator==(const FBrush& rhs) const;

		bool operator!=(const FBrush& rhs) const
		{
			return !operator==(rhs);
		}

    private:

		
		FGradient m_Gradient;
		
		FColor m_Color;

		FName m_ImagePath;

		// Explicit size of the image. Vec2(-1,-1) = auto.
		
		FVec2 m_BrushSize = FVec2(-1, -1);

		// Normalized anchor point (0=start, 1=end per axis). Default: centered (0.5, 0.5).
		
		FVec2 m_BrushPos = FVec2(0.5f, 0.5f);

		
		EBrushTiling m_Tiling = EBrushTiling::None;

		
		EBrushStyle m_BrushStyle = EBrushStyle::None;

		
		EImageFit m_ImageFit = EImageFit::Fill;

		// NineSlice border sizes in pixels (left, top, right, bottom). Only used when imageFit == NineSlice.
		FMargin m_SliceMargins;

    };
    
} // namespace Fusion
