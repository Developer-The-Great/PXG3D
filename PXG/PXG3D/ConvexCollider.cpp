#include "ConvexCollider.h"
#include "Debug.h"
#include "Mesh.h"
#include "PhysicsEngine.h"

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

		Debug::Log("checking collision with convexCollider and convexCollider");

		Vector3 positionA = glm::vec3(manifold.transformA.Matrix[3]);
		Vector3 positionB = glm::vec3(manifold.transformB.Matrix[3]);



		//Since we have reached this point, the bounding volumes do intersect.

		auto meshA = this->GetMesh();
		auto meshB = convexCollider->GetMesh();


		Vector3 aToB = positionB - positionA;
		Vector3 directionCheckA = (aToB).Normalized();
		Vector3 directionCheckB = directionCheckA * -1;


		
		unsigned int indexA = 0;
		Vector3 supportPointA;
		
		PhysicsEngine::GetSupportPoint(meshA, manifold.transformA,positionA, directionCheckA, indexA,supportPointA);

		Vector3 CenterAToSupportPointA = supportPointA - positionA;
		
		//Debug::Log("positionA {0}", positionA.ToString());
		//Debug::Log("CenterAToSupportPointA {0}", CenterAToSupportPointA.ToString());
		//Debug::Log("SupportPointA {0}", supportPointA.ToString());

		unsigned int indexB = 0;
		Vector3 supportPointB;
		
		PhysicsEngine::GetSupportPoint(meshB, manifold.transformB, positionB, directionCheckB, indexB,supportPointB);

		Vector3 CenterBToSupportPointB = supportPointB - positionB;

		//Debug::Log("SupportPointB {0}", supportPointB.ToString());
		//Debug::Log("positionB {0}", positionB.ToString());
		//Debug::Log("CenterBToSupportPointB {0}", CenterBToSupportPointB.ToString());
		




		for (int index = 0; index < meshA->Indices.size(); index += 3)
		{
			int v0Index = meshA->Indices.at(index);

			glm::vec3 l =  manifold.transformA.ToGLM() * glm::vec4(meshA->Vertices.at(v0Index).normal.ToGLMVec3(),0);

			//Debug::Log("manifold A {0} ", manifold.transformA.ToString());
			//Debug::Log("testing normal {0} ",meshA->Vertices.at(v0Index).normal.ToString());

			//d is the distance between the centers of ColliderA and ColliderB, projected into 'l'
			float d = Mathf::Abs(Mathf::Dot(l, aToB));

			//ra is the distance from the center of ColliderA to the vertex nearest vertex to B, projected into 'l'
			float ra = Mathf::Abs(Mathf::Dot(l, CenterAToSupportPointA));

			//rb is the distance from the center of ColliderB to the vertex nearest vertex to A, projected into 'l'
			float rb = Mathf::Abs(Mathf::Dot(l, CenterBToSupportPointB));

			//Debug::Log("ra {0}", ra);
			//Debug::Log("rb {0}", rb);
			//Debug::Log("d {0}", d);

			if (d > ra + rb)
			{
				Debug::Log("Axis of seperation found! It is perpendicular to {0}", glm::to_string(l));
				return;
			}

		}


		for (int index = 0; index < meshB->Indices.size(); index += 3)
		{
			int v0Index = meshB->Indices.at(index);


			glm::vec3 l = manifold.transformB.ToGLM() * glm::vec4(meshB->Vertices.at(v0Index).normal.ToGLMVec3(), 0);

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
				Debug::Log("Axis of seperation found! It is perpendicular to {0}", glm::to_string(l));
				return;
			}

		}

		manifold.isColliding = true;
		Debug::Log("No Seperating Axis was found");

	}





}

