#pragma once

#include "Fusion/Misc/CoreTypes.h"
#include "Fusion/Algorithm/Hash.h"

#include <string_view>
#include <cstring>

namespace Fusion
{

	// Lightweight string identifier for fast O(1) comparison.
	// Hashed at construction time using xxHash via CalculateHash().
	class FUSIONCORE_API FName
	{
	public:
	    FName() = default;

	    FName(const char* str)
	    {
	        if (str)
	        {
	            m_Hash = CalculateHash(str, std::strlen(str));
	#ifndef NDEBUG
	            m_DebugStr = str;
	#endif
	        }
	    }

	    FName(const FString& str)
	    {
		    if (!str.IsEmpty())
		    {
	            m_Hash = str.GetHash();
	#ifndef NDEBUG
	            m_DebugStr = str;
	#endif
		    }
	    }

	    FName(std::string_view str)
	    {
	        if (!str.empty())
	        {
	            m_Hash = CalculateHash(str.data(), str.size());
	#ifndef NDEBUG
	            // string_view is not guaranteed to be null-terminated;
	            // m_DebugStr is best-effort and safe only when backed by
	            // a null-terminated buffer (e.g. a string literal or FString).
	            m_DebugStr = str.data();
	#endif
	        }
	    }

	    SizeT GetHash()  const { return m_Hash; }
	    bool  IsValid()  const { return m_Hash != 0; }
	    bool  IsNull()   const { return m_Hash == 0; }

	    explicit operator bool() const { return IsValid(); }

	    bool operator==(const FName& other) const { return m_Hash == other.m_Hash; }
	    bool operator!=(const FName& other) const { return !(*this == other); }

	private:
	    SizeT m_Hash = 0;

	#ifndef NDEBUG
	    FString m_DebugStr = nullptr;
	#endif
	};

	template<>
	inline SizeT GetHash<FName>(const FName& name)
	{
	    return name.GetHash();
	}

} // namespace Fusion
