#pragma once
#include <soloud/soloud.h>
#include <memory>

#include <string>
#include <regex>

#include <soloud/soloud_sfxr.h>
#include <soloud/soloud_wav.h>
#include <soloud/soloud_wavstream.h>
#include <memory>

#include "AudioClip.hpp"

namespace PXG
{
	class AudioEngine
	{
	public:
		static AudioEngine& GetInstance()
		{
			static AudioEngine engine;
			return engine;
		}


		void Play(AudioClip clip)
		{
			clip.h = audio_impl->play(*(clip.source));
		}
		void Stop(const AudioClip& clip) const
		{
			if (clip.h != 0)
				audio_impl->stop(clip.h);
		}

		
		
		AudioClip createClip(const std::string& filename)
		{
			static std::regex rgx(R"rgx([A-z][A-z1-9\\\/]+(\.streamed|)\.(sfxr|wav))rgx");

			std::smatch matches;

			if(regex_match(filename,matches,rgx))
			{
				std::shared_ptr<SoLoud::AudioSource> source;

				
				if(matches[2].str() == "wav")
				{
					if(matches[1].str().size() > 1)
					{
						auto wavstream = std::make_shared<SoLoud::WavStream>();
						wavstream->load(filename.c_str());
						source = wavstream;
					}
					else
					{
						auto wav = std::make_shared<SoLoud::Wav>();
						wav->load(filename.c_str());
						source = wav;
					}

					
				} else if (matches[2].str() == "sfxr")
				{
					auto sfxr = std::make_shared<SoLoud::Sfxr>();
					sfxr->loadParams(filename.c_str());
					source = sfxr;
					
				}
				return AudioClip{source};
			}
			return AudioClip{nullptr};
		}

		void Loop(bool loop, const AudioClip& clip)
		{
			clip.source->setLooping(loop);
		}


	private:
		AudioEngine()
		{
			audio_impl = std::make_unique<SoLoud::Soloud>();
			audio_impl->init();
		}

		std::unique_ptr<SoLoud::Soloud> audio_impl ;

		
	};
}
