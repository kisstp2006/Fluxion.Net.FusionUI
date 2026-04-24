#include "Fusion/Containers/StringView.h"

#include <cstring>

namespace Fusion
{

    FStringView::FStringView(const char* str) noexcept
        : m_Data(str ? str : "")
        , m_Size(str ? std::strlen(str) : 0)
    {}

    char FStringView::operator[](size_t byteIndex) const
    {
        FUSION_ASSERT(byteIndex < m_Size, "FStringView index out of bounds");
        return m_Data[byteIndex];
    }

    FStringView FStringView::Substr(size_t byteOffset, size_t byteLength) const noexcept
    {
        if (byteOffset >= m_Size)
            return {};
        size_t available = m_Size - byteOffset;
        size_t len = (byteLength == npos || byteLength > available) ? available : byteLength;
        return FStringView(m_Data + byteOffset, len);
    }

    FStringView FStringView::TrimLeft() const noexcept
    {
        const char* p   = m_Data;
        const char* end = m_Data + m_Size;
        while (p < end && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\f' || *p == '\v'))
            ++p;
        return FStringView(p, static_cast<size_t>(end - p));
    }

    FStringView FStringView::TrimRight() const noexcept
    {
        const char* begin = m_Data;
        const char* p     = m_Data + m_Size;
        while (p > begin && (p[-1] == ' ' || p[-1] == '\t' || p[-1] == '\n' || p[-1] == '\r' || p[-1] == '\f' || p[-1] == '\v'))
            --p;
        return FStringView(begin, static_cast<size_t>(p - begin));
    }

    FStringView FStringView::Trim() const noexcept
    {
        return TrimLeft().TrimRight();
    }

    bool FStringView::StartsWith(FStringView prefix) const noexcept
    {
        if (prefix.m_Size > m_Size)
            return false;
        return std::memcmp(m_Data, prefix.m_Data, prefix.m_Size) == 0;
    }

    bool FStringView::EndsWith(FStringView suffix) const noexcept
    {
        if (suffix.m_Size > m_Size)
            return false;
        return std::memcmp(m_Data + m_Size - suffix.m_Size, suffix.m_Data, suffix.m_Size) == 0;
    }

    bool FStringView::operator==(FStringView other) const noexcept
    {
        return m_Size == other.m_Size && std::memcmp(m_Data, other.m_Data, m_Size) == 0;
    }

    bool FStringView::operator==(const FString& other) const noexcept
    {
        return m_Size == other.ByteLength() && std::memcmp(m_Data, other.CStr(), m_Size) == 0;
    }

    bool FStringView::operator==(const char* other) const noexcept
    {
        if (!other)
            return m_Size == 0;
        size_t otherLen = std::strlen(other);
        return m_Size == otherLen && std::memcmp(m_Data, other, m_Size) == 0;
    }

    bool FStringView::operator==(std::string_view other) const noexcept
    {
        return m_Size == other.size() && std::memcmp(m_Data, other.data(), m_Size) == 0;
    }

    SizeT FStringView::GetHash() const
    {
        return CalculateHash(m_Data, m_Size);
    }

} // namespace Fusion
