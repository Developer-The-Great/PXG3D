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
		//Debug::SetDebugState(false);
		//do bounding volume check and exit if bounding volumes do not intersect 

		
		//TODO Implement Bounding Volumes


		//Since we have reached this point, the bounding volumes do intersect.

		Debug::Log("checking collision with convexCollider and convexCollider");

		Vector3 positionA = glm::vec3(manifold.transformA.Matrix[3]);
		Vector3 positionB = glm::vec3(manifold.transformB.Matrix[3]);



		auto meshA = this->GetMesh();
		auto meshB = convexCollider->GetMesh();


		Vector3 aToB = positionB - positionA;

		//Test all axes parallel to face normals of meshA
		for (int index = 0; index < meshA->Indices.size(); index += 3)
		{
			int v0Index = meshA->Indices.at(index);

			glm::vec3 l =  manifold.transformA.ToGLM() * glm::vec4(meshA->Vertices.at(v0Index).normal.ToGLMVec3(),0);
			l = glm::normalize(l);


			Vector3 directionA = Mathf::Dot(aToB, l) > Mathf::Dot(aToB * -1, l) ? (l) : (-l);
			Vector3 directionB = directionA * -1;


			unsigned int indexA = -1, indexB = -1;
			Vector3 supportPointA, supportPointB;

			PhysicsEngine::GetSupportPoint(meshA, manifold.transformA, positionA, directionA, indexA, supportPointA);
			PhysicsEngine::GetSupportPoint(meshB, manifold.transformB, positionB, directionB, indexB, supportPointB);

			Vector3 CenterAToSupportPointA = supportPointA - positionA;
			Vector3 CenterBToSupportPointB = supportPointB - positionB;

			//Debug::Log("manifold A {0} ", manifold.transformA.ToString());
			//Debug::Log("testing normal {0} ",meshA->Vertices.at(v0Index).normal.ToString());

			//d is the distance between the centers of ColliderA and ColliderB, projected into 'l'
			float d = Mathf::Abs(Mathf::Dot(l, aToB));

			//ra is the distance from the center of ColliderA to the vertex nearest vertex to B, projected into 'l'
			float ra = Mathf::Abs(Mathf::Dot(l, CenterAToSupportPointA));

			//rb is the distance from the center of ColliderB to the vertex nearest vertex to A, projected into 'l'
			float rb = Mathf::Abs(Mathf::Dot(l, CenterBToSupportPointB));

			

			if (d > ra + rb)
			{

				/*Debug::Log("CenterAToSupportPointA {0}", CenterAToSupportPointA.ToString());
				Debug::Log("CenterBToSupportPointB {0}", CenterBToSupportPointB.ToString()); 

				Debug::Log("ra {0}", ra);
				Debug::Log("rb {0}", rb);
				Debug::Log("d {0}", d);
				Debug::Log("ra + rb {0}", (ra + rb));
				Debug::Log("Axis of seperation found! It is perpendicular to {0}", glm::to_string(l));*/

				//Debug::Log("No Collision found between gameObject: {0} and gameObject: {1}"
					//, manifold.physicsComponentA->GetOwner()->name
					//, manifold.physicsComponentB->GetOwner()->name);

				return;
			}

		}

		//Test all axes parallel to face normals of meshB
		for (int index = 0; index < meshB->Indices.size(); index += 3)
		{
			int v0Index = meshB->Indices.at(index);


			glm::vec3 l = manifold.transformB.ToGLM() * glm::vec4(meshB->Vertices.at(v0Index).normal.ToGLMVec3(), 0);
			l = glm::normalize(l);


			Vector3 directionA = Mathf::Dot(aToB, l) > Mathf::Dot(aToB * -1, l) ? (l) : (-l);
			Vector3 directionB = directionA * -1;


			unsigned int indexA = -1, indexB = -1;
			Vector3 supportPointA, supportPointB;

			PhysicsEngine::GetSupportPoint(meshA, manifold.transformA, positionA, directionA, indexA, supportPointA);
			PhysicsEngine::GetSupportPoint(meshB, manifold.transformB, positionB, directionB, indexB, supportPointB);

			Vector3 CenterAToSupportPointA = supportPointA - positionA;
			Vector3 CenterBToSupportPointB = supportPointB - positionB;

			//Debug::Log("manifold B {0} ", manifold.transformB.ToString());
			//Debug::Log("testing normal {0} ", glm::to_string(glm::vec4(meshB->Vertices.at(v0Index).normal.ToGLMVec3(), 0)));

			//d is the distance between the centers of ColliderA and ColliderB, projected into 'l'
			float d = Mathf::Abs(Mathf::Dot(l, aToB));

			//ra is the distance from the center of ColliderA to the vertex nearest vertex to B, projected into 'l'
			float ra = Mathf::Abs(Mathf::Dot(l, CenterAToSupportPointA));

			//rb is the distance from the center of ColliderB to the vertex nearest vertex to A, projected into 'l'
			float rb = Mathf::Abs(Mathf::Dot(l, CenterBToSupportPointB));

			


			if (d > ra + rb)
			{

				/*Debug::Log("CenterAToSupportPointA {0}", CenterAToSupportPointA.ToString());
				Debug::Log("CenterBToSupportPointB {0}", CenterBToSupportPointB.ToString());

				Debug::Log("ra {0}", ra);
				Debug::Log("rb {0}", rb);
				Debug::Log("ra + rb {0}", (ra+rb));
				Debug::Log("d {0}", d);
				Debug::Log("Axis of seperation found! It is perpendicular to {0}", glm::to_string(l));*/

				//Debug::Log("No Collision found between gameObject: {0} and gameObject: {1}"
					//, manifold.physicsComponentA->GetOwner()->name
					//, manifold.physicsComponentB->GetOwner()->name);

				return;
			}

		}


		//edge-to-edge Collision Detection






		manifold.isColliding = true;

		Debug::Log("Collision found between gameObject: {0} and gameObject: {1}"
			, manifold.physicsComponentA->GetOwner()->name
			, manifold.physicsComponentB->GetOwner()->name);
		Debug::Log("No Seperating Axis was found");

	}





}

