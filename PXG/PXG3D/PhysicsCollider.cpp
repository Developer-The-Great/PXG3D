#include "PhysicsCollider.h"

namespace PXG
{



	void PhysicsCollider::SetPhysicsComponentOwner(PhysicsComponent* physicsComponentOwner)
	{
		this->physicsComponentOwner = physicsComponentOwner;
	}

	PhysicsComponent* PhysicsCollider::GetPhysicsComponentContainer()
	{
		return physicsComponentOwner;
	}


	std::shared_ptr<Mesh> PhysicsCollider::GetMesh() const
	{
		return mesh;
	}
	void PhysicsCollider::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}
}


