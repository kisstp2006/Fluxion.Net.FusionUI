#include "Fusion/Object/Object.h"

namespace Fusion
{
	FObject::FObject(Ptr<FObject> outer, FName name) : m_Outer(outer), m_Name(MoveTemp(name))
    {
	    
    }

	void FObject::AttachSubobject(Ptr<FObject> subobject)
	{
		if (!subobject)
		{
			return;
		}

		m_Subobjects.Add(subobject);
	}

	void FObject::DetachSubobject(Ptr<FObject> subobject)
	{
		if (!subobject)
		{
			return;
		}


	}

    FObject::~FObject()
	{
		
	}

} // namespace Fusion
