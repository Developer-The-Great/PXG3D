#pragma once
#include "soloud/soloud.h"

namespace PXG
{

	struct AudioClip
	{
		std::shared_ptr<SoLoud::AudioSource> source;
		SoLoud::handle h = 0;
	};

}
