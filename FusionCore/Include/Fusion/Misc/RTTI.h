#pragma once

#include "Fusion/Templates/TypeTraits.h"

#define FUSION_TYPEID(ClassType) ::Fusion::GetTypeID<ClassType>() 

namespace Fusion
{
	typedef SizeT FTypeID;

	template<typename Type>
	FTypeID GetTypeID()
	{
		constexpr bool isVoid = std::is_void_v<Type>;
		constexpr bool isPointer = std::is_pointer_v<Type>;
		constexpr bool isPtr = TFIsPtr<Type>::Value;
		constexpr bool isWeakPtr = TFIsWeakPtr<Type>::Value;

		typedef std::remove_pointer_t<Type> Type0;
		typedef std::remove_const_t<Type0> FinalType;

		if constexpr (isVoid)
		{
			return 0;
		}
		else if constexpr (isPtr)
		{
			return typeid(typename TFIsPtr<Type>::Type).hash_code();
		}
		else if constexpr (isWeakPtr)
		{
			return typeid(typename TFIsWeakPtr<Type>::Type).hash_code();
		}
		if constexpr (isPointer) // a pointer type
		{
			if constexpr (std::is_same_v<char, FinalType>)
			{
				return (FTypeID)typeid(char*).hash_code(); // special case: TYPEID(char) != TYPEID(char*)
			}
			else
			{
				return (FTypeID)typeid(FinalType).hash_code(); // always ignore pointers for TypeId. Ex: TYPEID(Object) == TYPEID(Object*)
			}
		}
		else // a plain data type
		{
			return (FTypeID)typeid(FinalType).hash_code();
		}
	}
}
