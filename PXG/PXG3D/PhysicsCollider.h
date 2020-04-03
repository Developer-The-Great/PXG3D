#pragma once
#include <memory>
#include <vector>
#include "Mat4.h"
#include "Vector3.h"
#include <memory>
#include "PhysicsComponent.h"

namespace PXG
{
	class Mesh;
	class ConvexCollider;
	class CubeCollider;
	class SphereCollider;


	struct Manifold
	{
		Mat4 transformA;
		Mat4 transformB;

		std::shared_ptr<PhysicsComponent> physicsComponentA;
		std::shared_ptr<PhysicsComponent> physicsComponentB;

		bool isColliding;
		

	};

	enum class ColliderType
	{
		CONVEX,
		SPHERE,
		CUBE
	};




	class PhysicsCollider
	{
	public:

		PhysicsCollider() = default;

		/*virtual bool BoundingVolumeCheck(std::shared_ptr<PhysicsCollider> otherPhysicsCollider) = 0;

		virtual bool BoundingVolumeCheckWith(std::shared_ptr<SphereCollider> sphereCollider) = 0;

		virtual bool BoundingVolumeCheckWith(std::shared_ptr<ConvexCollider> convexCollider) = 0;*/





		virtual void CheckCollision(std::shared_ptr<PhysicsCollider> otherPhysicsCollider,Manifold& manifold) { }

		virtual void CheckCollisionWith( SphereCollider* sphereCollider, Manifold& manifold) { }

		virtual void CheckCollisionWith(ConvexCollider* convexCollider, Manifold& manifold) {  }

		


		std::shared_ptr<Mesh> GetMesh() const;

		void SetMesh(std::shared_ptr<Mesh> mesh);



	protected:

		std::shared_ptr<Mesh> mesh;
	private:

		


		



	};
}


