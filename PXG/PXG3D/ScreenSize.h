#pragma once
#include "Vector2.h"

namespace PXG
{
	struct ScreenSize
	{
		constexpr static inline const float WIDTH = 1280;
		constexpr static inline const float HEIGHT = 720;
		static inline const Vector2 DIMENSIONS = Vector2(WIDTH, HEIGHT);

	};
}