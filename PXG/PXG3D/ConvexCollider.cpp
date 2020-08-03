#include "ConvexCollider.h"
#include "Debug.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "GameObject.h"

#include "World.h"
#include "DebugDrawingManager.h"
#include <memory>
#include "Time.h"
#include <chrono>
#include "BenchmarkTimer.h"
#include "DepthPenetrationInfo.h"
#include "ContactPointPenetrationInfo.h"

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



	ConvexCollider::~ConvexCollider()
	{
		for (auto edge : edges)
		{
			delete edge;
		}
		edges.clear();
	}

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

		//------------------------------------------- Do face normal checking --------------------------------------------------------//
		float seperationAB;
		int indexA = -1;
		if (PhysicsEngine::FindSeperatingAxisByExtremePointProjection(meshA, meshB, manifold.transformA, manifold.transformB, positionA,
			positionB, indexA, seperationAB))
		{
			manifold.isColliding = false;
			return;
		}

		float seperationBA;
		int indexB = -1;
		if (PhysicsEngine::FindSeperatingAxisByExtremePointProjection(meshB, meshA, manifold.transformB, manifold.transformA, positionB,
			positionA, indexB, seperationBA))
		{
			manifold.isColliding = false;
			return;
		}
			
		//------------------------------------------- Do edge checking --------------------------------------------------------//
		float edgeToedgeSeperation;
		Vector3 edgeSeperationNormal;
		Vector3 edgeSeperationPosition;

		if (PhysicsEngine::FindSeparatingAxisByGaussMapEdgeToEdgeCheck(this, convexCollider, manifold.transformA, manifold.transformB, positionA,
			positionB, edgeToedgeSeperation,edgeSeperationNormal, edgeSeperationPosition))
		{
			manifold.isColliding = false;
			return;
		}

		manifold.isColliding = true;

		/*Debug::Log("seperationAB {0} ", seperationAB);
		Debug::Log("seperationBA {0} ", seperationBA);
		Debug::Log("edgeToedgeSeperation {0} ", edgeToedgeSeperation);*/

		Vector3 seperationPosA = manifold.transformA.MultiplyPoint(meshA->Vertices.at(indexA).position);
		Vector3 seperationNormalA = manifold.transformA.MultiplyDirection( meshA->Vertices.at(indexA).normal).Normalized();

		Vector3 seperationPosB = manifold.transformB.MultiplyPoint(meshB->Vertices.at(indexB).position);
		Vector3 seperationNormalB = manifold.transformB.MultiplyDirection(meshB->Vertices.at(indexB).normal).Normalized();

		DepthPenetrationInfo APenetrationInfo(seperationPosA, seperationNormalA, seperationAB,false);
		DepthPenetrationInfo BPenetrationInfo(seperationPosB, seperationNormalB, seperationBA, true);
		DepthPenetrationInfo EdgePenetrationInfo(edgeSeperationPosition, edgeSeperationNormal, edgeToedgeSeperation,false);

		DepthPenetrationInfo penetrationInfos[] = { APenetrationInfo,BPenetrationInfo,EdgePenetrationInfo };
		manifold.penetrationInfo =  *std::max_element(penetrationInfos, penetrationInfos + 3);

		//Debug::Log("ColliderAIsRef {0} ", manifold.penetrationInfo.isColliderARef);
		
		//Debug::Log("chosen seperation {0} ", manifold.penetrationInfo.penetration);
}

	void ConvexCollider::FillInManifold(std::shared_ptr<PhysicsCollider> otherPhysicsCollider, Manifold & manifold)
	{
		otherPhysicsCollider->FillInManifoldWith(this, manifold);
	}

	void ConvexCollider::FillInManifoldWith(ConvexCollider * convexCollider, Manifold & manifold)
	{
		DebugDrawingManager::drawingManagerStaticPtr->SetShouldDraw(false);

		//BenchmarkTimer SH("FillInManifold");

		ConvexCollider* refCollider = nullptr;
		ConvexCollider* incidentCollider = nullptr;

		float drawTime = 3.0f;

		DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube
		(manifold.penetrationInfo.seperationPlanePosition, Vector3(), Vector3(0.1), Vector3(1, 0, 1), drawTime);
		//use sutherland-hodgman to get the clipping points of the polygon

		DebugDrawingManager::drawingManagerStaticPtr->InstantiateLine
		(manifold.penetrationInfo.seperationPlanePosition
			, manifold.penetrationInfo.seperationPlanePosition + manifold.penetrationInfo.seperationPlaneNormal,
			Vector3(1, 0, 1), drawTime);

		//decide which collider would be the reference polygon
		if (manifold.penetrationInfo.isColliderARef)
		{
			refCollider = this;
			incidentCollider = convexCollider;
		}
		else
		{
			refCollider = convexCollider;
			incidentCollider = this;
			
		}

		/*Debug::Log(" ------------- Declaring Ref and Inc -------------------------------------");

		Debug::Log("ref collider is {0} ", refCollider->GetPhysicsComponentContainer()->GetOwner()->name);
		Debug::Log("inc collider is {0} ", incidentCollider->GetPhysicsComponentContainer()->GetOwner()->name);*/

		//clip incident collider with reference collider
		std::vector<Vector3> contactPoints;

		PhysicsEngine::SutherlandHodgmanClipping(refCollider, incidentCollider, contactPoints);
		PhysicsEngine::SutherlandHodgmanClipping(incidentCollider, refCollider, contactPoints);
		
	
		//seperate the elements of contactPoints into 2 list, one containing all the elements in the seperating axis and the ones that are not

		std::vector<Vector3*> pointsOnPLane;
		std::vector<ContactPointPenetrationInfo> pointsUnderPlane;

		for (Vector3& contactPoint : contactPoints)
		{
			float distanceToPlane = PhysicsEngine::PointDistanceToPlane(
				manifold.penetrationInfo.seperationPlaneNormal
				, manifold.penetrationInfo.seperationPlanePosition,
				contactPoint
			);

			if (Mathf::FloatCompare(Mathf::Abs(distanceToPlane), 0.0f,0.01))
			{
				pointsOnPLane.push_back(&contactPoint);
			}
			else 
			{
				ContactPointPenetrationInfo newContactPenetration(&contactPoint, distanceToPlane);
				pointsUnderPlane.push_back(newContactPenetration);
			}
		}

		//TODO this is for debugging only , remove it later
		for (auto contactPoint : pointsOnPLane)
		{
			//Debug::Log("contact at {0} ", contactPoint.ToString());
			DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
				*contactPoint, Vector3(0, 0, 0), Vector3(0.01, 0.01, 0.01), Vector3(0, 0.5, 0), drawTime);
		}

		for (auto contactPoint : pointsUnderPlane)
		{
			//Debug::Log("contact at {0} ", contactPoint.ToString());
			DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
				*contactPoint.contactPoint, Vector3(0, 0, 0), Vector3(0.01, 0.01, 0.01), Vector3(0, 1.0,0), drawTime);
		}


		//sort contacts with biggest penetration depth from cutting plane
		std::sort(pointsUnderPlane.begin(), pointsUnderPlane.end());

		//starting from the first element of the sorted list find the contacts that have a penetration depth equal to the biggest element 

		if (pointsUnderPlane.size() < 1) { return; }
		
		float furthestPenetration = pointsUnderPlane.at(0).penetration;
		std::vector<Vector3*> furthestPenetrationContacts;

		for (const auto& contactPenetrationInfo : pointsUnderPlane)
		{
			furthestPenetrationContacts.push_back(contactPenetrationInfo.contactPoint);
			//if (Mathf::FloatCompare(furthestPenetration, contactPenetrationInfo.penetration))
			//{
			//	furthestPenetrationContacts.push_back(contactPenetrationInfo.contactPoint);
			//}
			//else
			//{
			//	//since we already sorted the std::vector, if end up with contact point not equal to the furthest penetration, we can just exit early
			//	break;
			//}
		}
	
		Vector3 inverseNormal = -manifold.penetrationInfo.seperationPlaneNormal;

		auto gameObjA = refCollider->GetPhysicsComponentContainer()->GetOwner();
		auto gameObjB = incidentCollider->GetPhysicsComponentContainer()->GetOwner();


		auto rigidbodyA = gameObjA->GetComponent<Rigidbody>();
		auto rigidbodyB = gameObjB->GetComponent<Rigidbody>();

		Vector3 colliderAWorldPosition = manifold.penetrationInfo.isColliderARef ? manifold.transformB.Matrix[3] : manifold.transformA.Matrix[3] ;
		Vector3 colliderBWorldPosition = manifold.penetrationInfo.isColliderARef ? manifold.transformA.Matrix[3] :manifold.transformB.Matrix[3] ;

		for (const auto onPlaneContactPoint : pointsOnPLane)
		{
			float closestDotProductToNormal = -FLT_MAX;
			Vector3* closestContact = nullptr;

			for (const auto contactPoint : furthestPenetrationContacts)
			{
				float currentDistanceFromInverseNormal = Mathf::Dot((*contactPoint - *onPlaneContactPoint).Normalized(), inverseNormal);

				if (currentDistanceFromInverseNormal > closestDotProductToNormal)
				{
					closestContact = contactPoint;
					closestDotProductToNormal = currentDistanceFromInverseNormal;
				}
			}

			if (closestContact)
			{

				DebugDrawingManager::drawingManagerStaticPtr->InstantiateLine(*onPlaneContactPoint, *closestContact, Vector3(), 2.0);

				PhysicsContact newContact;

				newContact.worldContactPositionA = *onPlaneContactPoint;
				newContact.worldContactPositionB = *closestContact;
				newContact.collisionNormal = manifold.penetrationInfo.seperationPlaneNormal;
				newContact.rigidbodyA = rigidbodyA;
				newContact.rigidbodyB = rigidbodyB;
				newContact.colliderAWorldPosition = colliderAWorldPosition;
				newContact.colliderBWorldPosition = colliderBWorldPosition;
				newContact.penetrationDepth = manifold.penetrationInfo.penetration;

				manifold.contactData.push_back(newContact);

			}
		}


	}

	void ConvexCollider::FillInManifoldWith(SphereCollider * sphereCollider, Manifold & manifold)
	{
	}

	void ConvexCollider::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		PhysicsCollider::SetMesh(mesh);

		initializeColliderMesh();
	}

	void ConvexCollider::initializeColliderMesh()
	{
		initializeHalfEdgeList();

		initializeFaceList();
	}

	void ConvexCollider::initializeHalfEdgeList()
	{
		// holds the "pointer" to the unique indices inside mesh->Indices
		std::vector<int> uniqueIndex;
		// stores the unique vertices of the mesh
		std::vector<Vector3> uniquePositions;

		int uniqueIndexCount = -1;

		for (int i = 0; i < mesh->Vertices.size(); ++i)
		{
			Vector3 position = mesh->Vertices.at(i).position;

			bool isVectorSeen = false;
			//have we found this vector before?
			for (int j = 0; j < uniquePositions.size(); j++)
			{

				if (Mathf::FloatVectorCompare(uniquePositions.at(j), position))
				{
					//we have seen this vector before
					uniqueIndex.push_back(j);
					isVectorSeen = true;
					break;
				}
			}

			if (!isVectorSeen)
			{
				//we have not seen this position before,add it 
				uniqueIndexCount++;
				uniqueIndex.push_back(uniqueIndexCount);

				uniquePositions.push_back(position);

			}
		}

		VertexIndexToHalfEdgePtr vertexIndexToHalfEdge;

		//-------------instantiate Half edges and set their respective vertices and next edges----------------------//
		//vertex
		for (int i = 0; i < mesh->Indices.size(); i += 3)
		{
			unsigned int firstVertIndex = mesh->Indices.at(i);
			unsigned int secondVertIndex = mesh->Indices.at(i + 1);
			unsigned int thirdVertIndex = mesh->Indices.at(i + 2);

			unsigned int uniqueFirstIndex = uniqueIndex.at(firstVertIndex);
			unsigned int uniqueSecondIndex = uniqueIndex.at(secondVertIndex);
			unsigned int uniqueThirdIndex = uniqueIndex.at(thirdVertIndex);

			//-----------------instantiate first half edge---------------------//
			HalfEdgeEdge* firstEdge = new HalfEdgeEdge(uniqueFirstIndex);
			edges.push_back(firstEdge);
			edges.back()->vert = &mesh->Vertices.at(firstVertIndex);

			std::pair<unsigned int, unsigned int> firstPairing(uniqueFirstIndex, uniqueSecondIndex);
			vertexIndexToHalfEdge.insert(std::map<edgeVertexIndexPair, HalfEdgeEdge*>::value_type(firstPairing, firstEdge));


			//-----------------instantiate second half edge---------------------//
			HalfEdgeEdge* secondEdge = new HalfEdgeEdge(uniqueSecondIndex);
			edges.push_back(secondEdge);
			edges.back()->vert = &mesh->Vertices.at(secondVertIndex);

			edgeVertexIndexPair secondPairing(uniqueSecondIndex, uniqueThirdIndex);
			vertexIndexToHalfEdge.insert(VertexIndexToHalfEdgePtr::value_type(secondPairing, secondEdge));

			//-----------------instantiate third half edge---------------------//
			HalfEdgeEdge* thirdEdge = new HalfEdgeEdge(uniqueThirdIndex);
			edges.push_back(thirdEdge);
			edges.back()->vert = &mesh->Vertices.at(thirdVertIndex);

			edgeVertexIndexPair thirdPairing(uniqueThirdIndex, uniqueFirstIndex);
			vertexIndexToHalfEdge.insert(VertexIndexToHalfEdgePtr::value_type(thirdPairing, thirdEdge));

			//-------------------- link half edges to each other
			firstEdge->nextEdge = secondEdge;
			secondEdge->nextEdge = thirdEdge;
			thirdEdge->nextEdge = firstEdge;

		}


		//-------------set each halfEdges's edge pairing----------------------//

		for (auto indexEdgePair : vertexIndexToHalfEdge)
		{

			unsigned int u = indexEdgePair.first.first;
			unsigned int v = indexEdgePair.first.second;

			//for a Halfedge paired with vertices with an index of (u,v),
			//its pair would be a HalfEdge paired with vertices with an index of (v,u)
			auto iter = vertexIndexToHalfEdge.find(edgeVertexIndexPair(v, u));

			if (iter != vertexIndexToHalfEdge.end())
			{

				HalfEdgeEdge* edgePair = iter->second;
				HalfEdgeEdge* otherEdge = indexEdgePair.second;

				edgePair->pairingEdge = otherEdge;
				otherEdge->pairingEdge = edgePair;

			}

		}
	}

	void ConvexCollider::initializeFaceList()
	{
		std::vector<PhysicsMeshFace> polyhedronFaces;

		for (const auto& edge : edges)
		{
			if (edge->isPairingEqualNormal())
			{
				continue;
			}

			const Vector3& currentEdgeNormal = *edge->GetNormal();

			bool isNormalSeenBefore = false;
			PhysicsMeshFace* meshFacePtr = nullptr;

			for (auto& meshFace : polyhedronFaces)
			{
				if (Mathf::FloatVectorCompare(currentEdgeNormal, meshFace.normal))
				{
					meshFacePtr = &meshFace;
					isNormalSeenBefore = true;
					break;
				}
			}

			if (isNormalSeenBefore)
			{
				meshFacePtr->AddEdge(edge);
			}
			else
			{
				PhysicsMeshFace newPhysicsMeshFace(edge);
				polyhedronFaces.push_back(newPhysicsMeshFace);
			}
		}



		//for each face 
		for (auto& face : polyhedronFaces)
		{
			HalfEdgeEdge* initialEdge = face.EdgesOnFace.at(0);
			HalfEdgeEdge* iterationEdge = initialEdge;

			int iterationCount = 0;

			bool circularListNotCreated = true;

			std::vector<HalfEdgeEdge*> circularHalfEdges;

			while (iterationCount <= edges.size() && circularListNotCreated)
			{
				HalfEdgeEdge* edgeAfterIterationEdge = iterationEdge->nextEdge;

				if (edgeAfterIterationEdge->isPairingEqualNormal())
				{
					HalfEdgeEdge* neighborNext = edgeAfterIterationEdge;

					while (neighborNext->isPairingEqualNormal())
					{
						neighborNext = neighborNext->pairingEdge->nextEdge;
					}

					edgeAfterIterationEdge = neighborNext;
				}

				circularHalfEdges.push_back(edgeAfterIterationEdge);
				iterationEdge = edgeAfterIterationEdge;

				circularListNotCreated = iterationEdge != initialEdge;

				iterationCount++;
			}
			face.EdgesOnFace = circularHalfEdges;
		}

		this->polyhedronFaces = polyhedronFaces;
	}

	const std::vector<HalfEdgeEdge*>& ConvexCollider::GetEdges()
	{
		return edges;
	}

	const std::vector<PhysicsMeshFace>& ConvexCollider::GetFaces()
	{
		return polyhedronFaces;
	}

}

