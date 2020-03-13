#include "AudioSource.h"
#include "AudioEngine.hpp"

namespace PXG
{
	AudioSourceComponent::AudioSourceComponent(AudioClip c) : clip(c)
	{
	}

	void AudioSourceComponent::Play() const
	{
		AudioEngine::GetInstance().Play(clip);
	}

	void AudioSourceComponent::Stop() const
	{
		AudioEngine::GetInstance().Stop(clip);
	}

	void AudioSourceComponent::AddPlayEvent(subject_base::event_t event)
	{
		play_events.emplace(event);
	}

	void AudioSourceComponent::AddStopEvent(subject_base::event_t event)
	{
		stop_events.emplace(event);
	}

	void AudioSourceComponent::onNotify(subject_base* subject_base, subject_base::event_t event)
	{
		if(play_events.find(event) != play_events.end())
		{
			Play();
		}
		if(stop_events.find(event) != stop_events.end())
		{
			Stop();
		}
	}

	void AudioSourceComponent::Loop(bool loop) const
	{
		AudioEngine::GetInstance().Loop(loop, clip);
	}
	
}
