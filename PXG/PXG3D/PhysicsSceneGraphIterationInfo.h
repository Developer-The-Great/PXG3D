#pragma once
#include "Mat4.h"
#include "PhysicsComponent.h"



namespace PXG
{
	struct PhysicsSceneGraphIterationInfo
	{
		Mat4 transform;
		std::shared_ptr<PhysicsComponent> physicsComponent;
	};
}


