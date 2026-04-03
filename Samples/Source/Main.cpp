
#include <Fusion/Core.h>
#include <Fusion/SDL3Platform.h>
#include <Fusion/VulkanRHI.h>
#include <Fusion/Widgets.h>

using namespace Fusion;

class SampleWindow : public FCompoundWidget
{
	FUSION_CLASS(SampleWindow, FCompoundWidget)
public:

	Ref<FVerticalStack> vstack;
	Ref<FHorizontalStack> hstack;

	void Construct() override
	{
		Super::Construct();

		Child(
			FAssignNew(FVerticalStack, vstack)
			.ContentHAlign(EHAlign::Fill)
			.HAlign(EHAlign::Fill)
			.VAlign(EVAlign::Fill)
			.Padding(FMargin(5, 5, 5, 5))
			.Spacing(10)
			.Name("RootStack")
			(
				FNew(FWidget)
				.Height(25)
				.Name("Empty"),

				FAssignNew(FHorizontalStack, hstack)
				.ContentHAlign(EHAlign::Center)
				.ContentVAlign(EVAlign::Center)
				.Spacing(10)
				.Name("hstack")
				(
					FNew(FDecoratedWidget)
					.FillRatio(1.0f)
					.Height(30)
					.Background(FBrush(FColors::Red)),

					FNew(FDecoratedWidget)
					.FillRatio(1.0f)
					.Height(30)
					.Background(FBrush(FColors::Green)),

					FNew(FDecoratedWidget)
					.FillRatio(1.0f)
					.Height(30)
					.Background(FBrush(FColors::Blue))
				),

				FNew(FWidget)
				.FillRatio(1.0f)
			)
		);
	}
	
};

int main(int argc, char* argv[])
{
	FApplication app(argc, argv);

	app.CreateWindow<SampleWindow>();
	
	return app.Run();
}

