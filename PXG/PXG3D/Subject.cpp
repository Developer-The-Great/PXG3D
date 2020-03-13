#include <vector>
#include "Subject.h"
#include "Subscriber.h"

namespace PXG
{
	void subject_base::notify(const event_t e)
	{
		for (subscriber_base* view : m_views)
		{
			view->onNotify(this, e);
		}
	}

	void subject_base::notifyFirst(const event_t e)
	{
		if (!m_views.empty()) m_views.front()->onNotify(this, e);
	}
	void subject_base::notifyLast(const event_t e)
	{
		if (!m_views.empty()) m_views.back()->onNotify(this, e);
	}
	void subject_base::notifyNext(const event_t e)
	{
		if (m_iter > m_views.size()) m_iter = 0;
		if (!m_views.empty()) m_views[m_iter++]->onNotify(this, e);
	}
}
