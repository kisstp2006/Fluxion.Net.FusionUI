#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
	FUSION_DEFINE_HANDLE_TYPE(FRenderTargetHandle, u32);

	struct FUIRenderPass
	{
		FRenderTargetHandle Handle;

		SizeT LayerIndex = 0;
		SizeT DrawCmdStartIndex = 0;
		SizeT DrawCmdCount = 0;
	};

	using FUISplitRangeArray = FStableGrowthArray<FSplitRange,   64>;
	using FUIRenderPassArray = FStableGrowthArray<FUIRenderPass, 32>;

    class FUSIONRHI_API FRenderSnapshot : public FIntrusiveBase
    {
    public:

		FRenderSnapshot() = default;

		FUIVertexArray vertexArray;
		FUISplitRangeArray vertexSplits;

		FUIIndexArray indexArray;
		FUISplitRangeArray indexSplits;

		FUIDrawItemArray drawItemArray;
		FUISplitRangeArray drawItemSplits;

		FUIDrawCmdArray drawCmdArray;
		FUISplitRangeArray drawCmdSplits;

		FUIClipRectArray clipRectArray;
		FUISplitRangeArray clipRectSplits;

		FUIGradientStopArray gradientStopArray;
		FUISplitRangeArray gradientStopSplits;

		//! @brief This array defines the actual order of draw commands.
		FUIRenderPassArray renderPassArray;

		FUIViewData viewData;

		FUIMatrixArray transformMatricesPerLayer;

    };
    
} // namespace Fusion

