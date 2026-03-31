#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

#include "Fusion/Object/RefCountBlock.h"
#include <atomic>

#include "Ptr.h"
#include "WeakPtr.h"

#define FUSION_CLASS_BODY(SelfClass) typedef SelfClass Self;\
    virtual FTypeID GetClassTypeID() const { return ::Fusion::GetTypeID<Self>(); }

#define FUSION_CLASS(SelfClass, SuperClass) typedef SuperClass Super; \
    FUSION_CLASS_BODY(SelfClass)

namespace Fusion
{

	enum FObjectFlags
    {
		FObject_None = 0,
    };

    class FUSIONCORE_API FObject
    {
		FUSION_CLASS_BODY(FObject)
    public:
        FObject(FName name = "Object", Ptr<FObject> outer = nullptr);

        FObject(const FObject&)            = delete;
        FObject& operator=(const FObject&) = delete;

		void AttachSubobject(Ptr<FObject> subobject);
		void DetachSubobject(Ptr<FObject> subobject);

		Ptr<FObject> GetOuter() const { return m_Outer.Lock(); }

		u32 GetSubobjectCount() const { return static_cast<u32>(m_Subobjects.Size()); }

        Ptr<FObject> GetSubobject(u32 index) const
        {
            FUSION_ASSERT_THROW(index < static_cast<int>(m_Subobjects.Size()), FOutOfBoundsException, "Index out of bounds");
            return m_Subobjects[static_cast<size_t>(index)];
		}

		const FName& GetName() const { return m_Name; }

		void SetName(const FName& name) { m_Name = name; }

		FUuid GetUuid() const { return m_Uuid; }

    protected:

        virtual ~FObject();

		virtual void OnSubobjectAttached(Ptr<FObject> subobject) {}

		virtual void OnSubobjectDetached(Ptr<FObject> subobject) {}

		virtual void OnDetachFromOuter() {}

		virtual void OnAttachToOuter() {}

        virtual void OnBeforeDestroy() {}

    private:

		FArray<Ptr<FObject>> m_Subobjects;

        FName m_Name;
		WeakPtr<FObject> m_Outer;
		FUuid m_Uuid;
        std::atomic<Internal::RefCountBlock*> m_Control = nullptr;
        FObjectFlags flags = FObject_None;

        template<typename T> friend class Ptr;
        template<typename T> friend class WeakPtr;
        friend struct Internal::RefCountBlock;
    };

} // namespace Fusion
