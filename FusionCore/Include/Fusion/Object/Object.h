#pragma once

#include "Fusion/Object/RefCountBlock.h"
#include <atomic>

namespace Fusion
{
    template<typename T> class Ptr;
    template<typename T> class WeakPtr;

    enum FObjectFlags
    {
		FObject_None = 0,
    };

    class FUSIONCORE_API FObject
    {
    public:
        FObject();

        FObject(const FObject&)            = delete;
        FObject& operator=(const FObject&) = delete;

    protected:

        virtual ~FObject();

        virtual void OnBeforeDestroy() {}

    private:
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
