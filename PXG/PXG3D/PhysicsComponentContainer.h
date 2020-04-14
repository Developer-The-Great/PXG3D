#pragma once
#include "PhysicsSceneGraphIterationInfo.h"
#include "PhysicsComponent.h"
#include "PhysicsCollider.h"
#include <memory>

namespace PXG
{
	struct PhysicsComponentContainer
	{
		std::vector<PhysicsSceneGraphIterationInfo> PSGIIContainer;
	};
	
}
