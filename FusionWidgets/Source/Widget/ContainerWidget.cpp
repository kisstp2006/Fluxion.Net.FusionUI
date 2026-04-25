#include "Fusion/Widgets.h"

namespace Fusion
{
	FContainerWidget::FContainerWidget()
	{
		m_ClipShape = FRectangle();
	}

	void FContainerWidget::SetParentSurfaceRecursive(Ref<FSurface> surface)
	{
		ZoneScoped;

		Super::SetParentSurfaceRecursive(surface);

		for (SizeT i = 0; i < m_Children.Size(); i++)
		{
			m_Children[i]->SetParentSurfaceRecursive(surface);
		}
	}

	void FContainerWidget::AddChildWidget(Ref<FWidget> child)
	{
		ZoneScoped;

		if (!child || m_Children.Contains(child)) return;
		m_Children.Add(child);
		AttachChildWidget(child);
	}

	void FContainerWidget::RemoveChildWidget(Ref<FWidget> child)
	{
		int index = (int)m_Children.IndexOf(child);
		if (index < 0) return;
		m_Children.RemoveAt(index);
		DetachChildWidget(child);
	}

	void FContainerWidget::DetachChild(Ref<FWidget> child)
	{
		Super::DetachChild(child);

		RemoveChildWidget(child);
	}

	void FContainerWidget::SetChildIndex(Ref<FWidget> childWidget, int index)
	{
		i64 curIndex = m_Children.IndexOf(childWidget);
		if (curIndex < 0 || curIndex == index)
			return;

		m_Children.RemoveAt(curIndex);
		m_Children.Insert(childWidget, index);

	    MarkLayoutDirty();
	    MarkPaintDirty();
	}

	void FContainerWidget::SetWidgetFlagInternal(EWidgetFlags flag, bool set)
	{
		SetWidgetFlag(flag, set);
	}

} // namespace Fusion
