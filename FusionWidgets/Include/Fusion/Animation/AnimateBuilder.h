#pragma once

namespace Fusion
{

    template<typename T>
    class FTweenBuilder
    {
    public:

        FTweenBuilder& From(T from)
        {
            fromValue = MoveTemp(from);
            hasFromGetter = false;
            return *this;
        }

        FTweenBuilder& To(T value) { toValue = MoveTemp(value); return *this; }
        FTweenBuilder& Duration(f32 d) { duration = d; return *this; }
        FTweenBuilder& Easing(FEasingCurve e) { easing = e; return *this; }
        FTweenBuilder& Delay(f32 d) { delay = d; return *this; }
        FTweenBuilder& Loop(EAnimationLoopMode m) { loopMode = m; return *this; }

        FTweenBuilder& OnComplete(std::function<void()> cb)
        {
            onComplete = MoveTemp(cb);
            onCompleteValid = true;
            return *this;
        }

        Ref<FAnimation> Build(Ref<FObject> outer)
        {
            T from = hasFromGetter ? fromGetter() : fromValue;

            Ref<FTweenAnimation> anim = NewObject<FTweenAnimation>(outer.Get(), name);
            anim->AssignOwner(owner);
            anim->SetInterpolator(MoveTemp(from), toValue, easing, setter);
            anim->SetDelay(delay);
            anim->SetDuration(duration);
            anim->SetLoopMode(loopMode);

            if (onCompleteValid)
            {
                anim->OnComplete().Add(onComplete);
            }

            return anim;
        }

        Ref<FAnimation> Play()
        {
            if (!owner.IsValid())
                return nullptr;
            Ref<FAnimation> anim = Build(owner.Get());
            application->PlayAnimation(anim, owner, name);
            return anim;
        }

        Ref<FAnimation> Play(FName slot)
        {
            if (!owner.IsValid())
                return nullptr;
            this->name = slot;
            Ref<FAnimation> anim = Build(owner.Get());
            application->PlayAnimation(anim, owner, slot);
            return anim;
        }

    private:

        FName                         name;
        Ref<FObject>                  owner;
        Ref<FApplicationInstance>     application;
        std::function<void(const T&)> setter;
        std::function<T()>            fromGetter;
        T                             fromValue{};
        T                             toValue{};
        bool                          hasFromGetter = false;
        FEasingCurve                  easing = EEasingType::EaseInOutCubic;
        f32                           duration = 0.3f;
        f32                           delay = 0.0f;
        EAnimationLoopMode            loopMode = EAnimationLoopMode::Once;
        std::function<void()>         onComplete;
        bool                          onCompleteValid = false;

        friend class FAnimate;
    };

    class FAnimate
    {
    public:

        template<class TWidgetType, typename T>
        static FTweenBuilder<T> Tween(FName name, TWidgetType* target, T(TWidgetType::* getter)() const, void (TWidgetType::* setter)(const T&))
        {
            FUSION_ASSERT(target != nullptr, "FAnimate::Tween called on null target!");

            FTweenBuilder<T> builder{};
            builder.name = name;
            builder.application = target->GetApplication();
            FUSION_ASSERT(target != nullptr, "FAnimate::Tween called on target that does not have an FApplicationInstance!");

            builder.setter = [target, setter](const T& v) { (target->*setter)(v); };
            builder.fromGetter = [target, getter]() -> T { return (target->*getter)(); };
            builder.owner = target;
            builder.hasFromGetter = true;
            return builder;
        }

    };
    
} // namespace Fusion

#define FAnimate_Tween(widgetPtr, PropertyName)\
    FAnimate::Tween<TPtrType<decltype(widgetPtr)>::Type>(\
		#PropertyName, TPtrType<decltype(widgetPtr)>::GetRawPtr(widgetPtr),\
		(decltype(std::declval<TPtrType<decltype(widgetPtr)>::Type>().##PropertyName()) (TPtrType<decltype(widgetPtr)>::Type::*)() const)& TPtrType<decltype(widgetPtr)>::Type::##PropertyName,\
		(void (TPtrType<decltype(widgetPtr)>::Type::*)(const decltype(std::declval<TPtrType<decltype(widgetPtr)>::Type>().##PropertyName())&))& TPtrType<decltype(widgetPtr)>::Type::__AnimBypassSetter_##PropertyName\
	)
