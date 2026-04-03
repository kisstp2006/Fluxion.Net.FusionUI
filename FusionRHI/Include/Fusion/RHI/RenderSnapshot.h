#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
	FUSION_DEFINE_HANDLE_TYPE(FRenderTargetHandle, u32);

	struct FRenderPass
	{
		FRenderTargetHandle Handle;

		SizeT layerIndex = 0;
		SizeT drawCmdStartIndex = 0;
		SizeT drawCmdCount = 0;
	};

    class FUSIONRHI_API FRenderSnapshot : public FIntrusiveBase
    {
    public:

		FRenderSnapshot() = default;

		using FSplitRangeArray = FStableDynamicArray<FSplitRange, 64>;
		using FRenderPassArray = FStableDynamicArray<FRenderPass, 32>;
		using FMatrixArray	   = FStableDynamicArray<FMat4, 64>;

		FUIVertexArray vertexArray;
		FSplitRangeArray vertexSplits;

		FUIIndexArray indexArray;
		FSplitRangeArray indexSplits;

		FUIDrawItemArray drawItemArray;
		FSplitRangeArray drawItemSplits;

		FUIDrawCmdArray drawCmdArray;
		FSplitRangeArray drawCmdSplits;

		FUIClipRectArray clipRectArray;
		FSplitRangeArray clipRectSplits;

		FUIGradientStopArray gradientStopArray;
		FSplitRangeArray gradientStopSplits;

		FRenderPassArray renderPassArray;

		FMatrixArray matricesPerLayer;

    };
    
} // namespace Fusion

