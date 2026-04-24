#pragma once

#include "String.h"

#include <string_view>
#include <cstdint>

namespace Fusion
{

    // Non-owning view over a UTF-8 byte range.
    // Not guaranteed to be null-terminated — do not call CStr() on a sub-view.
    // Construct from FString, const char*, std::string_view, or a pointer+length pair.
    class FUSIONCORE_API FStringView
    {
    public:
        static constexpr size_t npos = static_cast<size_t>(-1);

        struct CodepointIterator
        {
            using value_type      = char32_t;
            using difference_type = std::ptrdiff_t;

            const char* Ptr;

            char32_t operator*() const
            {
                auto b = static_cast<uint8_t>(*Ptr);
                if (b < 0x80) return b;
                if (b < 0xE0) return ((b & 0x1F) << 6)  |  (static_cast<uint8_t>(Ptr[1]) & 0x3F);
                if (b < 0xF0) return ((b & 0x0F) << 12) | ((static_cast<uint8_t>(Ptr[1]) & 0x3F) << 6)  |  (static_cast<uint8_t>(Ptr[2]) & 0x3F);
                              return ((b & 0x07) << 18) | ((static_cast<uint8_t>(Ptr[1]) & 0x3F) << 12) | ((static_cast<uint8_t>(Ptr[2]) & 0x3F) << 6) | (static_cast<uint8_t>(Ptr[3]) & 0x3F);
            }

            CodepointIterator& operator++()
            {
                auto b = static_cast<uint8_t>(*Ptr);
                if      (b < 0x80) Ptr += 1;
                else if (b < 0xE0) Ptr += 2;
                else if (b < 0xF0) Ptr += 3;
                else               Ptr += 4;
                return *this;
            }

            CodepointIterator operator++(int)
            {
                CodepointIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const CodepointIterator& o) const { return Ptr == o.Ptr; }
            bool operator!=(const CodepointIterator& o) const { return Ptr != o.Ptr; }
        };

        struct CodepointRange
        {
            const char* Begin;
            const char* End;

            CodepointIterator begin() const { return { Begin }; }
            CodepointIterator end()   const { return { End };   }
        };

        // ── Constructors ──────────────────────────────────────────────────

        FStringView() noexcept : m_Data(""), m_Size(0) {}
        FStringView(const char* str) noexcept;
        constexpr FStringView(const char* str, size_t byteLength) noexcept
            : m_Data(str ? str : ""), m_Size(byteLength) {}
        FStringView(std::string_view sv) noexcept : m_Data(sv.data()), m_Size(sv.size()) {}
        FStringView(const FString& str) noexcept : m_Data(str.CStr()), m_Size(str.ByteLength()) {}

        // ── Data access ───────────────────────────────────────────────────

        const char* Data()       const noexcept { return m_Data; }
        size_t      ByteLength() const noexcept { return m_Size; }
        size_t      Size()       const noexcept { return m_Size; }
        size_t      Length()     const noexcept { return m_Size; }
        bool        Empty()      const noexcept { return m_Size == 0; }

        // Byte-level indexing. Asserts on out-of-bounds.
        char operator[](size_t byteIndex) const;

        // ── Conversion ────────────────────────────────────────────────────

        std::string_view View()        const noexcept { return { m_Data, m_Size }; }
        FString          ToString()    const          { return FString(m_Data, m_Size); }
        std::string      ToStdString() const          { return { m_Data, m_Size }; }

        // ── UTF-8 iteration ───────────────────────────────────────────────

        CodepointRange Codepoints() const noexcept { return { m_Data, m_Data + m_Size }; }

        // ── Sub-views ─────────────────────────────────────────────────────

        // Returns a sub-view starting at byteOffset with byteLength bytes.
        // If byteOffset >= ByteLength(), returns an empty view.
        // byteLength is clamped to the available bytes.
        FStringView Substr(size_t byteOffset, size_t byteLength = npos) const noexcept;

        // ── Whitespace trimming ───────────────────────────────────────────

        FStringView TrimLeft()  const noexcept;
        FStringView TrimRight() const noexcept;
        FStringView Trim()      const noexcept;

        // ── Search ────────────────────────────────────────────────────────

        bool StartsWith(FStringView prefix) const noexcept;
        bool EndsWith(FStringView suffix)   const noexcept;

        // ── Equality ──────────────────────────────────────────────────────

        bool operator==(FStringView other)      const noexcept;
        bool operator==(const FString& other)   const noexcept;
        bool operator==(const char* other)      const noexcept;
        bool operator==(std::string_view other) const noexcept;

        bool operator!=(FStringView other)      const noexcept { return !(*this == other); }
        bool operator!=(const FString& other)   const noexcept { return !(*this == other); }
        bool operator!=(const char* other)      const noexcept { return !(*this == other); }
        bool operator!=(std::string_view other) const noexcept { return !(*this == other); }

        // ── Hash ──────────────────────────────────────────────────────────

        SizeT GetHash() const;

    private:
        const char* m_Data;
        size_t      m_Size;
    };

} // namespace Fusion

template<>
struct std::formatter<Fusion::FStringView> : std::formatter<std::string_view>
{
    auto format(const Fusion::FStringView& sv, std::format_context& ctx) const
    {
        return std::formatter<std::string_view>::format(sv.View(), ctx);
    }
};
