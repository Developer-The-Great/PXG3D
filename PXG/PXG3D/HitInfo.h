#pragma once
#include "GameObject.h"
#include <memory>
#include "Vector3.h"

namespace PXG
{
	struct HitInfo
	{
		Vector3 Position;
		Vector3 Normal;
		float T;
		bool RayHit;
		std::shared_ptr<GameObject> GameObjectHit = nullptr;
	};
}
