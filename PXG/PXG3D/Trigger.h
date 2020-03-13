#pragma once
#include <algorithm>

#include "Component.h"
#include "Subject.h"


namespace PXG
{
	class Trigger : public Component, public subject_base
	{
	public:
		enum  event : event_t { ON_TRIGGER_RAISED };


		void FixedUpdate(float tick) override
		{
			if (triggerCondition())
			{
				notify(ON_TRIGGER_RAISED);
			}
		}

		class TriggerAccessor {
		protected:
			static bool accessor(std::shared_ptr<Trigger> t)
			{
				return t->triggerCondition();
			}
		};
		

	protected:
		friend class TriggerAccessor;
		virtual bool triggerCondition() { return false; };
	};


	template <class TriggerA, class... Others>
	class TriggerAdder : public Trigger,Trigger::TriggerAccessor
	{
	public:
		template <
			typename = std::enable_if_t <std::is_base_of_v<Trigger, TriggerA> &&
					  (std::is_base_of_v<Trigger, Others> && ...)>>
			TriggerAdder(std::shared_ptr<TriggerA> a, std::shared_ptr<Others>... others) :
			trigger_ptrs{{
					std::static_pointer_cast<Trigger>((a)),
					std::static_pointer_cast<Trigger>((others))...
				}} {}

	protected:

		bool triggerCondition() override
		{
			return std::all_of(trigger_ptrs.begin(), trigger_ptrs.end(), [&](std::shared_ptr<Trigger> trigger){
				return this->accessor(trigger);
			});
		}
	private:
		std::vector<std::shared_ptr<Trigger>> trigger_ptrs;

	};


}
