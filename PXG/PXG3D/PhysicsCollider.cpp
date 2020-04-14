#include "PhysicsCollider.h"

namespace PXG
{





	std::shared_ptr<Mesh> PhysicsCollider::GetMesh() const
	{
		return mesh;
	}
	void PhysicsCollider::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}
}


