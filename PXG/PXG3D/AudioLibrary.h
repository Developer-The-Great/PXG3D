#pragma once
#include <string>
#include <unordered_map>


#include "AudioClip.hpp"

namespace PXG
{
	class AudioLibrary
	{
	public:
		static AudioClip GetSample(const std::string& identifier);

	private:
		static std::unordered_map<std::string, AudioClip> clips;
	};

}
