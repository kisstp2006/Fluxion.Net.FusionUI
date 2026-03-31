#pragma once

#include "Fusion/Object/RefCountBlock.h"
#include <atomic>

#include "Ptr.h"
#include "WeakPtr.h"

namespace Fusion
{

    enum FObjectFlags
    {
		FObject_None = 0,
    };

    class FUSIONCORE_API FObject
    {
    public:
        FObject(Ptr<FObject> outer = nullptr, FName name = "Object");

        FObject(const FObject&)            = delete;
        FObject& operator=(const FObject&) = delete;

		void AttachSubobject(Ptr<FObject> subobject);
		void DetachSubobject(Ptr<FObject> subobject);

    protected:

        virtual ~FObject();

        virtual void OnBeforeDestroy() {}

    private:

		FArray<Ptr<FObject>> m_Subobjects;

        FName m_Name;
		WeakPtr<FObject> m_Outer;
        std::atomic<Internal::RefCountBlock*> m_Control = nullptr;
        FObjectFlags flags = FObject_None;

        template<typename T> friend class Ptr;
        template<typename T> friend class WeakPtr;
        friend struct Internal::RefCountBlock;
    };

    template<typename T, typename... Args> requires std::derived_from<T, FObject>
    Ptr<T> NewObject(Args&&... args)
    {
        return Ptr<T>(new T(std::forward<Args>(args)...));
    }

} // namespace Fusion
