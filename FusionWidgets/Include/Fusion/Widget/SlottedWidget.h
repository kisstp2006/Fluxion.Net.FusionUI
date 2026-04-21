#pragma once

namespace Fusion
{

    class FUSIONWIDGETS_API FSlottedWidget : public FWidget
    {
        FUSION_WIDGET(FSlottedWidget, FWidget)
    protected:

        FSlottedWidget();

        void OnBeforeDestroy() override;

    public:

        virtual u32 GetSlotCount() = 0;

        int GetChildCount() override
        {
            int count = 0;
            for (Ref<FWidget>* ptr : m_Slots)
                if (*ptr) count++;
            return count;
        }

        Ref<FWidget> GetChildAt(u32 index) override
        {
            u32 current = 0;
            for (Ref<FWidget>* ptr : m_Slots)
            {
                if (*ptr)
                {
                    if (current == index) return *ptr;
                    current++;
                }
            }
            return nullptr;
        }

    private:

        FArray<Ref<FWidget>*> m_Slots;

    protected:

        void RegisterSlotPtr(Ref<FWidget>& slot)
        {
            m_Slots.Add(&slot);
        }

        void SetSlotWidgetInternal(Ref<FWidget>& slot, Ref<FWidget> newWidget)
        {
            if (slot == newWidget) return;
            if (slot) DetachChildWidget(slot);
            slot = newWidget;
            if (slot) AttachChildWidget(slot);
        }

    };
    
} // namespace Fusion
