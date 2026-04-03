#pragma once

// Copyright (c) 2026 Neil Mewada
// SPDX-License-Identifier: MIT

namespace Fusion
{

    class FUSIONWIDGETS_API FLayerTree : public FObject
    {
        FUSION_CLASS(FLayerTree, FObject)
    public:

        FLayerTree() : Super("LayerTree")
		{}

        bool IsSyncNeeded() const { return m_NeedsSync; }

        void MarkSyncNeeded();

        void DoSyncIfNeeded(FWidget* rootWidget);

        void DoPaintIfNeeded();

        Ref<FLayer> FindLayerForWidget(FUuid widgetUuid);

        Ref<FLayer> GetRootLayer() { return m_RootLayer; }

    protected:

        void SyncWidget(Ref<FWidget> widget, Ref<FLayer> parentLayer, FHashSet<FUuid>& visited);

        Ref<FLayer> m_RootLayer;

        FAffineTransform m_RootLayerTransform;

        FHashMap<FUuid, Ref<FLayer>> m_WidgetUuidToLayerMap;

        bool m_NeedsSync = true;
    };
    
} // namespace CE
