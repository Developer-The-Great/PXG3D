#pragma once
#include "PhysicsSceneGraphIterationInfo.h"
#include "AABBBox.h"
#include <memory>
namespace PXG
{
	//contains the AABB corressponding to the physicsComponent and transform
	struct PSGIIToAABB
	{
		PhysicsSceneGraphIterationInfo PSGII;
		std::shared_ptr<AABBBox> AABB;
	};
}
