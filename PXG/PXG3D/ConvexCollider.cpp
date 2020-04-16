#include "ConvexCollider.h"
#include "Debug.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "GameObject.h"

namespace PXG
{
	/*bool ConvexCollider::BoundingVolumeCheck(std::shared_ptr<PhysicsCollider> otherPhysicsCollider)
	{
		return true;
	}

	bool ConvexCollider::BoundingVolumeCheckWith(std::shared_ptr<ConvexCollider> otherPhysicsCollider)
	{
		return true;
	}

	bool ConvexCollider::BoundingVolumeCheckWith(std::shared_ptr<SphereCollider> otherPhysicsCollider)
	{
		return true;
	}*/



	void ConvexCollider::CheckCollision(std::shared_ptr<PhysicsCollider> physicsCollider, Manifold& manifold)
	{
		physicsCollider->CheckCollisionWith(this,manifold);
	}


	void ConvexCollider::CheckCollisionWith(SphereCollider* sphereCollider, Manifold& manifold)
	{
		Debug::Log("checking collision with sphereCollider and convexCollider");


	}

	void ConvexCollider::CheckCollisionWith(ConvexCollider* convexCollider, Manifold& manifold)
	{
		Vector3 positionA = glm::vec3(manifold.transformA.Matrix[3]);
		Vector3 positionB = glm::vec3(manifold.transformB.Matrix[3]);

		auto meshA = this->GetMesh();
		auto meshB = convexCollider->GetMesh();

		float seperationAB;

		//*
		if (PhysicsEngine::FindSeperatingAxisByProjectingMeshAandBToFaceNormals(meshA, meshB, manifold.transformA, manifold.transformB, positionA, 
			positionB, seperationAB))
		{
			manifold.isColliding = false;
			return;
		}

		if (PhysicsEngine::FindSeperatingAxisByProjectingMeshAandBToFaceNormals(meshB,meshA, manifold.transformB, manifold.transformA, positionB, 
			positionA, seperationAB))
		{
			manifold.isColliding = false;
			return;
		}
		//*/

		/*
		int indexA = -1;
		if (PhysicsEngine::FindSeperatingAxisByExtremePointProjection(meshA, meshB, manifold.transformA, manifold.transformB, positionA,
			positionB, indexA))
		{
			manifold.isColliding = false;
			return;
		}

		int indexB = -1;
		if (PhysicsEngine::FindSeperatingAxisByExtremePointProjection(meshB, meshA, manifold.transformB, manifold.transformA, positionB,
			positionA, indexB))
		{
			manifold.isColliding = false;
			return;
		}
		//*/

		Debug::Log("Testing edge to edge");

		if (PhysicsEngine::FindSeperatingAxisByBruteForceEdgeToEdgeCheck(meshB, meshA, manifold.transformB, manifold.transformA, positionB,
			positionA, seperationAB))
		{
			manifold.isColliding = false;
			return;
		}

		manifold.isColliding = true;

		Debug::Log("No Seperating Axis was found");
		Debug::Log("Collision found between gameObject: {0} and gameObject: {1}"
			, manifold.physicsComponentA->GetOwner()->name
			, manifold.physicsComponentB->GetOwner()->name);
		

	}





}

