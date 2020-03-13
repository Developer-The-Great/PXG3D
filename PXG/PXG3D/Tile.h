#pragma once
#include "Vector3.h"


namespace PXG
{
	struct Tile
	{
		constexpr static inline const float		SIZE = 100.0f;
				  static inline const Vector3	CENTER_OFFSET = { 0.0f,0,0.0f }; /* non literal type */
		constexpr static inline const float		WORLD_SCALE = 100.0f;
	};
}
