#pragma once
#include "PhysicsCollider.h"

namespace PXG
{
	class ConvexCollider : public PhysicsCollider 
	{
	public:

		ConvexCollider() = default;

		/*virtual bool BoundingVolumeCheck(std::shared_ptr<PhysicsCollider> otherPhysicsCollider) override;

		virtual bool BoundingVolumeCheckWith(std::shared_ptr<SphereCollider> sphereCollider) override;
		virtual bool BoundingVolumeCheckWith(std::shared_ptr<ConvexCollider> convexCollider) override;*/

		virtual void CheckCollision(std::shared_ptr<PhysicsCollider> physicsCollider,Manifold& manifold) override;


		virtual void CheckCollisionWith(SphereCollider* sphereCollider, Manifold& manifold) override;
		virtual void CheckCollisionWith(ConvexCollider* convexCollider, Manifold& manifold) override;
		

		




	private:


	};

}

