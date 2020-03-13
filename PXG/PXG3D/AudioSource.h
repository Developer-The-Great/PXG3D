#pragma once
#include "Component.h"
#include "AudioClip.hpp"
#include "Subscriber.h"
#include "Subject.h"
#include <set>

namespace PXG
{

	class AudioSourceComponent : public Component, public subscriber_base
	{
	public:
		AudioSourceComponent() = default;
		AudioSourceComponent(AudioClip c);


		void Play() const;
		void Stop() const;

		//Loop ?
		void Loop(bool loop = true) const;
		
		void SetClip(AudioClip c)
		{
			clip = c;
		}
		

		void AddPlayEvent(subject_base::event_t event);
		void AddStopEvent(subject_base::event_t event);

		void onNotify(subject_base* subject_base, subject_base::event_t event) override;
		
	private:

		std::set<subject_base::event_t> play_events, stop_events;
		AudioClip clip;
	};
}
