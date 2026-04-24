#include <Fusion/Core.h>
#include <Fusion/Widgets.h>

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

using namespace Fusion;

class SampleWindow : public FDecoratedBox
{
	FUSION_WIDGET(SampleWindow, FDecoratedBox)
public:

	void Construct() override
	{
		Super::Construct();

		Name("Text Samples");


		Child(
			FNew(FVerticalStack)
			.ContentHAlign(EHAlign::Fill)
			.HAlign(EHAlign::Fill)
			.VAlign(EVAlign::Fill)
			.Spacing(10)
			.Name("RootStack")
			(

				FNew(FHorizontalStack)
				.ContentHAlign(EHAlign::Center)
				.ContentVAlign(EVAlign::Center)
				.Spacing(10)
				.ClipContent(true)
				.Name("hstack")
				(
					FNew(FTextButton)
					.FillRatio(1.0f)
					.Height(32)
					.Style("Button/Primary")
					.Text("Primary")
					.OnClick([this]
					{
					}),

					FNew(FTextButton)
					.FillRatio(1.0f)
					.Height(32)
					.Text("Secondary")
					.Style("Button/Secondary")
					.OnClick([this]
					{
					}),

					FNew(FTextButton)
					.FillRatio(1.0f)
					.Height(32)
					.Text("Destructive")
					.Style("Button/Destructive")
					.OnClick([this]
					{
					}),

					FNew(FTextInput)
					.Style("TextInput/Base")
					.Placeholder("Type here...")
					.FillRatio(1.0f)
					.OnTextChanged([this](const FString& text)
					{
					})
				),

				FNew(FExpanderBox)
				.Title("Expander Box")
				.Child(
					FNew(FVerticalStack)
					(
						FNew(FLabel)
						.Text("Header")
						.FontSize(16),

						FNew(FLabel)
						.Text("Section Title")
						.FontSize(12),

						FNew(FLabel)
						.Text("This is the content body.")
						.FontSize(10)
					)
				),

				FNew(FWidget)
				.FillRatio(1.0f)
			)
		);
	}

	SampleWindow()
	{
	}
};

int main(int argc, char* argv[])
{
	FApplication app(argc, argv);

	Ref<FTheme> theme = app.CreateMainTheme();

	theme->MergeStyleSheet(FUSION_STYLE_SHEET
	{
		FColor WindowBackgroundColor = FColor(0.13f, 0.13f, 0.15f);
		FPen   FocusOutline			 = FPen::Solid(FColor(0.47f, 0.73f, 1.0f, 0.85f)).Thickness(2.0f);
		f32    FocusOutlineOffset	 = 2.5f;

		FColor DisabledBtnTextColor = FColor(0.35f, 0.35f, 0.38f);

		FUSION_STYLE(SampleWindow, "SampleWindow", Background, Padding)
		{
			Background = WindowBackgroundColor;
			Padding	   = FMargin(1, 1, 1, 1) * 5;
		}

		FUSION_STYLE(FDecoratedBox, "Base/FocusRing", Outline, OutlineOffset)
		{
			OutlineOffset = 0;

			Transition(Outline,			FTransition::MakeTween(0.15f));
			Transition(OutlineOffset,	FTransition::MakeTween(0.3f));

			FUSION_ON(FocusVisible)
			{
				Outline = FocusOutline;
				OutlineOffset = FocusOutlineOffset;
			}
		}

		FUSION_STYLE(FButton, "Button/Base", Shape, Background, Border, Outline, OutlineOffset)
		{
			Extends("Base/FocusRing");
			Shape = FRoundedRectangle(5.0f);

			FUSION_ON(Disabled)
			{
				Background = FColor(0.18f, 0.18f, 0.20f);
				Border     = FColor(0.25f, 0.25f, 0.28f);
			}
		}

		FUSION_STYLE(FButton, "Button/Primary", Shape, Background, Border)
		{
			Extends("Button/Base");
			Background = FColor(0.23f, 0.51f, 0.96f);
			Border     = FColor(0.16f, 0.40f, 0.82f);

			FUSION_ON(Hovered)
			{
				Background = FColor(0.38f, 0.65f, 0.98f);
				Border     = FColor(0.26f, 0.53f, 0.90f);
			}
			FUSION_ON(Pressed, Hovered)
			{
				Background = FColor(0.11f, 0.31f, 0.85f);
				Border     = FColor(0.08f, 0.23f, 0.70f);
			}
		}

		FUSION_STYLE(FButton, "Button/Secondary", Shape, Background, Border)
		{
			Extends("Button/Base");
			Background = FColor(0.22f, 0.22f, 0.25f);
			Border     = FColor(0.38f, 0.38f, 0.43f);

			FUSION_ON(Hovered)
			{
				Background = FColor(0.29f, 0.29f, 0.33f);
				Border     = FColor(0.50f, 0.50f, 0.56f);
			}
			FUSION_ON(Pressed, Hovered)
			{
				Background = FColor(0.16f, 0.16f, 0.19f);
				Border     = FColor(0.30f, 0.30f, 0.35f);
			}
		}

		FUSION_STYLE(FButton, "Button/Destructive", Shape, Background, Border)
		{
			Extends("Button/Base");
			Background = FColor(0.75f, 0.15f, 0.15f);
			Border     = FColor(0.60f, 0.10f, 0.10f);

			FUSION_ON(Hovered)
			{
				Background = FColor(0.88f, 0.22f, 0.22f);
				Border     = FColor(0.75f, 0.16f, 0.16f);
			}
			FUSION_ON(Pressed, Hovered)
			{
				Background = FColor(0.58f, 0.09f, 0.09f);
				Border     = FColor(0.45f, 0.06f, 0.06f);
			}
		}

		FUSION_STYLE(FLabel, "Button/Primary/Label", Color)
		{
			Color	  = FColors::White;

			FUSION_ON(Disabled)
			{
				Color = DisabledBtnTextColor;
			}
		}

		FUSION_STYLE(FLabel, "Button/Secondary/Label", Color)
		{
			Extends("Button/Primary/Label");
		}

		FUSION_STYLE(FLabel, "Button/Destructive/Label", Color)
		{
			Extends("Button/Primary/Label");
		}

		FUSION_STYLE(FScrollBox, "ScrollBox/Base",
			Background, Border, Shape,
			TrackBackground, TrackShape,
			ThumbBackground, ThumbHoverBackground, ThumbPressedBackground, ThumbDisabledBackground, ThumbShape,
			ScrollbarThickness, ScrollbarPadding, ContentPadding)
		{
			Background              = FColor(0.10f, 0.10f, 0.12f);
			Border                  = FColor(0.22f, 0.22f, 0.26f);
			Shape                   = FRoundedRectangle(5.0f);

			TrackBackground         = FColor(0.08f, 0.08f, 0.10f);
			TrackShape              = FRoundedRectangle(4.0f);

			ThumbBackground         = FColor(0.28f, 0.28f, 0.33f);
			ThumbHoverBackground    = FColor(0.42f, 0.42f, 0.48f);
			ThumbPressedBackground  = FColor(0.58f, 0.58f, 0.65f);
			ThumbDisabledBackground = FColor(0.18f, 0.18f, 0.20f);
			ThumbShape              = FRoundedRectangle(4.0f);

			ContentPadding		    = FMargin(1, 1, 1, 1) * 10;
			ScrollbarThickness	    = 15.0f;
			ScrollbarPadding	    = 3.0f;
		}

		FUSION_STYLE(FTextInput, "TextInput/Base", Shape,
			Background, Border, Outline, OutlineOffset, Padding, Font,
			TextColor, PlaceholderColor, SelectionColor, CursorColor)
		{
			Extends("Base/FocusRing");

			Shape            = FRoundedRectangle(5.0f);
			Background       = FColor(0.10f, 0.10f, 0.12f);
			Border           = FColor(0.30f, 0.30f, 0.34f);
			Padding          = FMargin(8, 6, 8, 6);
			Font             = FFont::Regular(FFont::kDefaultFamilyName, 14);
			TextColor        = FColors::White;
			PlaceholderColor = FColor(0.45f, 0.45f, 0.50f);
			SelectionColor   = FColor(0.23f, 0.51f, 0.96f, 0.45f);
			CursorColor      = FColors::White;

			FUSION_ON(Focused)
			{
				Border = FColor(0.47f, 0.73f, 1.0f, 0.85f);
			}

			FUSION_ON(FocusVisible)
			{
				Outline		  = FocusOutline;
				OutlineOffset = FocusOutlineOffset;
			}

			FUSION_ON(Hovered)
			{
				Border = FColor(0.45f, 0.45f, 0.50f);
			}

			FUSION_ON(Disabled)
			{
				Background       = FColor(0.08f, 0.08f, 0.10f);
				Border           = FColor(0.20f, 0.20f, 0.23f);
				TextColor        = FColor(0.35f, 0.35f, 0.38f);
				PlaceholderColor = FColor(0.25f, 0.25f, 0.28f);
				CursorColor      = FColor(0.35f, 0.35f, 0.38f);
			}
		}

		FUSION_STYLE(FExpanderBox, "FExpanderBox", Background, Border, Shape, Padding, ExpandedAmount)
		{
			Background = FColor(0.10f, 0.10f, 0.12f);
			Border = FColor(0.22f, 0.22f, 0.26f);
			ExpandedAmount = 0.0f;
			Shape = FRoundedRectangle(5.0f);

			Transition(ExpandedAmount, FTransition::MakeTween(0.2f));

			FUSION_ON(Expanded)
			{
				ExpandedAmount = 1.0f;
			}
		}

		FUSION_STYLE(FButton, "FExpanderBox/Header", Background, Border, Shape, Padding)
		{
			Extends("Button/Secondary");

			Padding = FMargin(1, 1, 1, 1) * 5.0f;
			Shape = FRoundedRectangle(5.0f);

			FUSION_ON(Expanded)
			{
				Shape = FRoundedRectangle(5.0f, 5.0f, 0, 0);
			}
		}

		FUSION_STYLE(FDecoratedBox, "FExpanderBox/Header/Chevron", Transform)
		{
			Transform = FAffineTransform::RotationDegrees(-90);

			Transition(Transform, FTransition::MakeTween(0.2f));

			FUSION_ON(Expanded)
			{
				Transform = FAffineTransform::RotationDegrees(0);
			}
		}

		FUSION_STYLE(FDecoratedBox, "FExpanderBox/Content", Padding)
		{
			Padding = FMargin(1, 1, 1, 1) * 10.0f;
		}
	});

	app.CreateMainWindow<SampleWindow>();

#if FUSION_PLATFORM_MAC
	app.SetInitialWindowSize(800, 600);
#else
	app.SetInitialWindowSize(1400, 1200);
#endif

	return app.Run();
}

