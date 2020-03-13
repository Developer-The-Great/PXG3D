#include "AudioLibrary.h"


#include "AudioEngine.hpp"
#include "FileConfig.h"

namespace PXG
{

	std::unordered_map<std::string, AudioClip> AudioLibrary::clips;

	
	AudioClip AudioLibrary::GetSample(const std::string& identifier)
	{ 
		auto helper = [](const std::string& str)
		{
			return config::PXG_SOUND_PATH + "FX/"+ str;
		};

		#define MAP(X,Y) clips[#X] = engine.createClip(helper(#X"."#Y))
		
		if(clips.empty())
		{
			auto& engine = AudioEngine::GetInstance();

			MAP(Click_Down, wav);
			MAP(Click_Up, wav);
			MAP(Ladder_Down, wav);
			MAP(Ladder_Up, wav);
			MAP(Lever_Bridge, wav);
			MAP(Rocks_Falling, wav);
			MAP(Steps_Grass, wav);
			MAP(Steps_Stone, wav);
			MAP(Swipe_Left, wav);
			MAP(Swipe_Right, wav);
			MAP(Wolf_Growl, wav);
			MAP(Wolf_Howl, wav);
			MAP(SStep1, wav);
			MAP(SStep2, wav);
			MAP(SStep3, wav);
			MAP(GStep1, wav);
			MAP(GStep2, wav);
			MAP(GStep3, wav);
		}

		#undef MAP

		return clips[identifier];
	}
}
