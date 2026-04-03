#include "Fusion/Object/Object.h"

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{
	FObject::FObject(FName name, Ref<FObject> outer) : m_Outer(outer), m_Name(MoveTemp(name)), m_Uuid(FUuid::Random())
    {
		m_Flags |= EObjectFlags::PendingConstruction;
    }

	void FObject::AttachSubobject(Ref<FObject> subobject)
	{
		if (!subobject)
		{
			return;
		}

		m_Subobjects.Add(subobject);
		subobject->m_Outer = Ref<FObject>(this);

		OnSubobjectAttached(subobject);
		subobject->OnAttachToOuter();
	}

	void FObject::DetachSubobject(Ref<FObject> subobject)
	{
		if (!subobject)
		{
			return;
		}

		i64 index = m_Subobjects.IndexOf(subobject);

		if (index != FArray<>::npos)
		{
			subobject->OnDetachFromOuter();
			OnSubobjectDetached(subobject);

			subobject->m_Outer = nullptr;
			m_Subobjects.RemoveAt(index);
		}
	}

    FObject::~FObject()
	{
		
	}

} // namespace Fusion
