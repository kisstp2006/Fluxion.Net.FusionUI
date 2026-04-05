// ReSharper disable CppInconsistentNaming
#pragma once

#include <optional>

#define FNew(WidgetClass, ...) \
    (* NewObject<WidgetClass>(this))

#define FAssignNew(WidgetClass, VariableName) FNew(WidgetClass).Assign(VariableName)

#define FUSION_WIDGET(WidgetClass, SuperClass)\
	FUSION_CLASS(WidgetClass, SuperClass)

#define __FUSION_BYPASS_SETTER(PropertyType, PropertyName, DirtyFunc)\
	template<typename TSelf>\
	TSelf& __BypassSetter_##PropertyName(this TSelf& self, PropertyType const& value) {\
		ZoneScoped;\
		if constexpr (TFEquitable<PropertyType>::Value)\
		{\
			if (TFEquitable<PropertyType>::AreEqual(self.m_##PropertyName, value))\
				return self;\
		}\
		thread_local const Fusion::FName nameValue = #PropertyName;\
		self.m_##PropertyName = value;\
		if ((self.GetFlags() & EObjectFlags::PendingConstruction) == 0) {\
			 static_cast<Fusion::FWidget&>(self).OnPropertyModified(nameValue);\
			 DirtyFunc;\
		}\
		return self;\
	}

#define __FUSION_PROPERTY(PropertyType, PropertyName, DirtyFunc)\
    protected:\
		PropertyType m_##PropertyName = {};\
    public:\
		PropertyType PropertyName() const { return m_##PropertyName; }\
		template<typename TSelf>\
		TSelf& PropertyName(this TSelf& self, PropertyType const& value) {\
			ZoneScoped;\
			if constexpr (TFEquitable<PropertyType>::Value)\
			{\
				if (TFEquitable<PropertyType>::AreEqual(self.m_##PropertyName, value))\
					return self;\
			}\
			thread_local const Fusion::FName nameValue = #PropertyName;\
			self.m_##PropertyName = value;\
			if ((self.GetFlags() & EObjectFlags::PendingConstruction) == 0) {\
				 static_cast<Fusion::FWidget&>(self).OnPropertyModified(nameValue);\
				 DirtyFunc;\
			}\
			return self;\
		}\
		__FUSION_BYPASS_SETTER(PropertyType, PropertyName, DirtyFunc)

#define __FUSION_STYLE_PROPERTY(PropertyType, PropertyName, DirtyFunc)\
    protected:\
		std::optional<PropertyType> m_Inline##PropertyName;\
		PropertyType m_##PropertyName = {};\
    public:\
		PropertyType PropertyName() const { return m_Inline##PropertyName.has_value() ? m_Inline##PropertyName.value() : m_##PropertyName; }\
		template<typename TSelf>\
		TSelf& PropertyName(this TSelf& self, PropertyType const& value) {\
			ZoneScoped;\
			if constexpr (TFEquitable<PropertyType>::Value)\
			{\
				if (self.m_Inline##PropertyName.has_value() && TFEquitable<PropertyType>::AreEqual(self.m_Inline##PropertyName.value(), value))\
					return self;\
			}\
			thread_local const Fusion::FName nameValue = #PropertyName;\
			self.m_Inline##PropertyName = value;\
			self.m_##PropertyName = value;\
			if ((self.GetFlags() & EObjectFlags::PendingConstruction) == 0) {\
				 static_cast<Fusion::FWidget&>(self).OnPropertyModified(nameValue);\
				 DirtyFunc;\
			}\
			return self;\
		}\
		__FUSION_BYPASS_SETTER(PropertyType, PropertyName, DirtyFunc)


#define FUSION_PROPERTY(PropertyType, PropertyName) __FUSION_PROPERTY(PropertyType, PropertyName, self.MarkPaintDirty())
#define FUSION_LAYOUT_PROPERTY(PropertyType, PropertyName) __FUSION_PROPERTY(PropertyType, PropertyName, self.MarkLayoutDirty())

#define FUSION_STYLE_PROPERTY(PropertyType, PropertyName) __FUSION_STYLE_PROPERTY(PropertyType, PropertyName, self.MarkPaintDirty())
#define FUSION_LAYOUT_STYLE_PROPERTY(PropertyType, PropertyName) __FUSION_STYLE_PROPERTY(PropertyType, PropertyName, self.MarkLayoutDirty())

#define FUSION_PROPERTY_GET(PropertyType, PropertyName) \
	PropertyType PropertyName()

#define FUSION_PROPERTY_SET(PropertyType, PropertyName) \
	template<typename TSelf>\
	TSelf& PropertyName(this TSelf& self, PropertyType value)

#define FUSION_SIGNAL(SignalType, SignalName, ...)\
	protected:\
		SignalType m_##SignalName;\
	public:\
		auto& SignalName() { return this->m_##SignalName; }\
		template<typename TSelf, typename TLambda>\
		TSelf& SignalName(this TSelf& self, const TLambda& lambda)\
		{\
			self.m_##SignalName.Add(lambda);\
			return self;\
		}\
		template<typename TSelf, typename TLambda>\
		TSelf& SignalName(this TSelf& self, FSignalHandle& outHandle, const TLambda& lambda)\
		{\
			outHandle = self.m_##SignalName.Add(lambda);\
			return self;\
		}

#define FUSION_APPLY_STYLE(PropertyName)\
	if (!m_Inline##PropertyName.has_value())\
	{\
		decltype(m_##PropertyName) value;\
		if (style.TryGet(#PropertyName, value, GetStyleState()))\
		{\
			__BypassSetter_##PropertyName(value);\
		}\
	}

#define FUSION_APPLY_STYLES(...) FUSION_FOR_EACH(FUSION_APPLY_STYLE, __VA_ARGS__)

// Per-pair helpers used by FUSION_STYLE_PROPERTIES.
// Pair is always a parenthesised (Type, Name) token group.
//
// DECL: juxtapose the macro name with the already-parenthesised pair so the
//       preprocessor sees a normal two-argument call — no unpacking needed.
#define __FUSION_SP_DECL(Pair)        FUSION_STYLE_PROPERTY Pair
//
// APPLY: __FUSION_SP_NAME expands the pair to just its Name token, but
//        FUSION_APPLY_STYLE uses # / ## which suppress argument expansion.
//        The extra indirection (__FUSION_SP_APPLY2) forces full expansion of
//        Name before it reaches those operators.
#define __FUSION_SP_NAME(Type, Name)  Name
#define __FUSION_SP_APPLY(Pair)       __FUSION_SP_APPLY2(__FUSION_SP_NAME Pair)
#define __FUSION_SP_APPLY2(Name)      FUSION_APPLY_STYLE(Name)

// Declares every (Type, Name) style property AND generates the ApplyStyle override.
// Replaces individual FUSION_STYLE_PROPERTY calls + the hand-written ApplyStyle body.
#define FUSION_STYLE_PROPERTIES(...) \
    FUSION_MACRO_EXPAND(FUSION_FOR_EACH(__FUSION_SP_DECL, __VA_ARGS__)) \
    void ApplyStyle(FStyle& style) override \
    { \
        Super::ApplyStyle(style); \
        FUSION_MACRO_EXPAND(FUSION_FOR_EACH(__FUSION_SP_APPLY, __VA_ARGS__)) \
    }
