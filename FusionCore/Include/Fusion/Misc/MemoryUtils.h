#pragma once

namespace Fusion
{
    class FUSIONCORE_API FMemoryUtils
    {
    public:
        FMemoryUtils() = delete;


        static SizeT AlignUp(SizeT baseSize, SizeT alignment)
        {
            FUSION_ASSERT(FMath::IsPowerOf2(alignment), "AlignUp() called with an alignment that is not a power of 2.");
            return (baseSize + alignment - 1) & ~(alignment - 1);
        }
    };
}
