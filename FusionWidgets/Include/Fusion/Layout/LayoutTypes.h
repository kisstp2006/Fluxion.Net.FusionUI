#pragma once

namespace Fusion
{

    enum class EHAlign : u8
    {
        Auto = 0,
        Fill,
        Left,
        Center,
        Right
    };
    FUSION_ENUM_CLASS(EHAlign);

    enum class EVAlign : u8
    {
        Auto = 0,
        Fill,
        Top,
        Center,
        Bottom
    };
    FUSION_ENUM_CLASS(EVAlign);
    
} // namespace Fusion
