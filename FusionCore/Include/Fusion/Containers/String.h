#pragma once

#include <string>
#include <string_view>
#include <cstdint>

namespace Fusion
{

    // UTF-8 string with small string optimization (SSO up to 15 bytes inline).
    // Length()     — number of Unicode codepoints.
    // ByteLength() — number of bytes.
    class FUSIONCORE_API FString
    {
    public:
        FString();
        FString(const char* str);
        FString(const char* str, size_t length);
        FString(std::string_view str);
        FString(const std::string& str);
        FString(const FString& other);
        FString(FString&& other) noexcept;
        ~FString();

        FString& operator=(const FString& other);
        FString& operator=(FString&& other) noexcept;
        FString& operator=(const char* str);
        FString& operator=(std::string_view str);

        // Returns the raw UTF-8 byte buffer, null-terminated.
        const char*      CStr()       const;
        std::string_view View()       const;
        std::string      ToStdString() const;

        size_t Length()     const { return m_Length; } // Unicode codepoints
        size_t ByteLength() const { return m_Size; }   // Raw bytes
        bool   IsEmpty()    const { return m_Size == 0; }

        FString& operator+=(const FString& other);
        FString& operator+=(std::string_view str);
        FString& operator+=(const char* str);

        FString operator+(const FString& other)    const;
        FString operator+(std::string_view other)  const;
        FString operator+(const char* other)       const;

        bool operator==(const FString& other)   const;
        bool operator==(std::string_view other) const;
        bool operator==(const char* other)      const;

        bool operator!=(const FString& other)   const { return !(*this == other); }
        bool operator!=(std::string_view other) const { return !(*this == other); }
        bool operator!=(const char* other)      const { return !(*this == other); }

    private:
        static constexpr size_t SSO_CAPACITY = 15;

        struct HeapBuffer
        {
            char*  Ptr;
            size_t Capacity;
        };

        union
        {
            HeapBuffer m_Heap;
            char       m_SSO[SSO_CAPACITY + 1];
        };

        size_t m_Size   = 0;
        size_t m_Length = 0; // Unicode codepoint count

        bool        IsHeap()       const { return m_Size > SSO_CAPACITY; }
        char*       Data()               { return IsHeap() ? m_Heap.Ptr : m_SSO; }
        const char* Data()         const { return IsHeap() ? m_Heap.Ptr : m_SSO; }

        void Assign(const char* str, size_t length);
        void AppendBytes(const char* str, size_t length);

        static size_t CountCodepoints(const char* str, size_t byteLength);
    };

} // namespace Fusion
