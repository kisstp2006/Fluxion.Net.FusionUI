#pragma once

namespace Fusion
{

    enum class EStyleState : u32
    {
        Default = 0,
        Hovered = FUSION_BIT(0),
        Pressed = FUSION_BIT(1),
        Focused = FUSION_BIT(2),
        Disabled = FUSION_BIT(3),

        // Selection / toggle
        Selected = FUSION_BIT(4),  // tabs, list items, menu items, radio buttons
        Checked = FUSION_BIT(5),  // checkboxes, toggle switches

        // Input states
        ReadOnly = FUSION_BIT(6),  // editable=false but not grayed out like Disabled
        Error = FUSION_BIT(7),  // validation failure — red border, etc.

        // Structural states
        Expanded = FUSION_BIT(8),  // tree nodes, accordions, dropdowns (collapsed = !Expanded)
    };
    FUSION_ENUM_CLASS_FLAGS(EStyleState);
    
} // namespace Fusion
