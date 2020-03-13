#pragma once
#include "Vector3.h"

namespace PXG
{
	struct CollisionCubeParams
	{
		Vector3 offset;
		float widthFromMin = 0;
		float breadthFromMin = 0;
		float heightFromMin = 0;
	};
}
