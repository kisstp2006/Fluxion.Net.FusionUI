#pragma once

#include "Fusion/Misc/CoreTypes.h"

namespace Fusion::Resources
{

    // A single embedded resource.
    struct FResource
    {
        const char* Path;  // Virtual path, e.g. "/Fonts/Roboto.ttf". Always starts with '/'.
        const u8*   Data;  // Pointer into static storage — valid for the lifetime of the process.
        SizeT       Size;  // Size in bytes.
    };

    // Non-owning, non-allocating view over a contiguous range of FResource entries.
    // Supports range-for.
    struct FResourceView
    {
        const FResource* m_Begin = nullptr;
        const FResource* m_End   = nullptr;

        const FResource* begin() const { return m_Begin; }
        const FResource* end()   const { return m_End;   }

        SizeT Count() const { return static_cast<SizeT>(m_End - m_Begin); }
        bool  Empty() const { return m_Begin == m_End; }

        const FResource& operator[](SizeT i) const { return m_Begin[i]; }
    };

} // namespace Fusion::Resources
