#pragma once
#include "PhysicsSceneGraphIterationInfo.h"
#include "PhysicsComponent.h"
#include "PhysicsCollider.h"
#include <memory>

namespace PXG
{
	//a container that contains PhysicsSceneGraphIterationInfos
	struct PhysicsComponentContainer
	{
		std::vector<PhysicsSceneGraphIterationInfo> PSGIIContainer;
		
	};
	
}
