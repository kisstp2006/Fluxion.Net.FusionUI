#pragma once

namespace Fusion
{

    class FUSIONWIDGETS_API FSlottedWidget : public FWidget
    {
        FUSION_WIDGET(FSlottedWidget, FWidget)
    protected:

        FSlottedWidget();

        void OnBeforeDestroy() override;

        void Construct() override;

        virtual void OnSlotSet(const FName& slotName) {}

    public:

        void SetParentSurfaceRecursive(Ref<FSurface> surface) override;

        virtual u32 GetSlotCount() = 0;

        virtual bool IsValidSlotWidget(u32 slot, Ref<FWidget> widget) = 0;

        bool SetSlotWidget(u32 slot, Ref<FWidget> widget);

        int GetChildCount() override;

        Ref<FWidget> GetChildAt(u32 index) override;

        void DetachChild(Ref<FWidget> child) override;

    private:

        FArray<Ref<FWidget>> m_Slots;
    };
    
} // namespace Fusion
