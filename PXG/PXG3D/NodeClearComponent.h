#pragma once
#include <set>

#include "Component.h"
#include "Subscriber.h"
#include "GameObject.h"
#include "PXGNode.h"

namespace PXG
{
	class NodeClearComponent :public  Component, public subscriber_base
	{
	public:
		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			if(m_listenerEvents.find(event) != m_listenerEvents.end())
			{
				if(owner.lock()->HasComponent<Node>())
				{
					owner.lock()->GetComponent<Node>()->SetNodeWeight(1);
				}
			}
		}

		void AddListener(subject_base::event_t event)
		{
			m_listenerEvents.insert(event);
		}

	private:
		std::set<subject_base::event_t> m_listenerEvents;
		
	};

}

