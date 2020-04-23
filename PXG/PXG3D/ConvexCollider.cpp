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
		for (auto edge : edges)
		{
			assert(edge->GetNormal());
			assert(edge->GetPairingNormal());
		}



		Vector3 positionA = glm::vec3(manifold.transformA.Matrix[3]);
		Vector3 positionB = glm::vec3(manifold.transformB.Matrix[3]);

		auto meshA = this->GetMesh();
		auto meshB = convexCollider->GetMesh();

		float seperationAB;

		/*
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

		//*
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

		/*
		if (PhysicsEngine::FindSeperatingAxisByBruteForceEdgeToEdgeCheck(meshB, meshA, manifold.transformB, manifold.transformA, positionB,
			positionA, seperationAB))
		{
			manifold.isColliding = false;
			return;
		}
		*/

		if (PhysicsEngine::FindSperatingAxisByGaussMapEdgeToEdgeCheck(this, convexCollider, manifold.transformB, manifold.transformA, positionB,
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

	void ConvexCollider::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		PhysicsCollider::SetMesh(mesh);

		///*
		std::vector<int> uniqueIndex;
		std::vector<Vector3> uniquePositions;

		int uniqueIndexCount = -1;

		for (int i = 0; i < mesh->Vertices.size(); ++i)
		{
			Vector3 position = mesh->Vertices.at(i).position;

			bool isVectorSeen = false;
			//have we found this vector before?
			for (int i = 0; i < uniquePositions.size(); i++)
			{
				
				if (Mathf::FloatVectorCompare(uniquePositions.at(i), position))
				{
					//we have seen this vector before
					uniqueIndex.push_back(i);
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
		for (int i =0; i < mesh->Indices.size();i+=3)
		{
			unsigned int firstVertIndex = mesh->Indices.at(i);
			unsigned int secondVertIndex = mesh->Indices.at(i+1);
			unsigned int thirdVertIndex = mesh->Indices.at(i+2);
			
			//instantiate first half edge
			edges.push_back(new HalfEdgeEdge());
			edges.back()->vert = &mesh->Vertices.at(firstVertIndex);

			//std::map<std::pair<unsigned int, unsigned int>, HalfEdgeEdge*> ;

			std::pair<unsigned int, unsigned int> firstPairing(uniqueIndex.at(firstVertIndex), uniqueIndex.at(secondVertIndex));
			vertexIndexToHalfEdge.insert(std::map<edgeVertexIndexPair, HalfEdgeEdge*>::value_type(firstPairing, edges.back()));


			//instantiate second half edge
			edges.push_back(new HalfEdgeEdge());
			edges.back()->vert = &mesh->Vertices.at(secondVertIndex);

			edgeVertexIndexPair secondPairing(uniqueIndex.at(secondVertIndex), uniqueIndex.at(thirdVertIndex));
			vertexIndexToHalfEdge.insert(VertexIndexToHalfEdgePtr::value_type(secondPairing, edges.back()));

			//link first half edge to second half edge
			edges.at(edges.size() - 2)->nextEdge = edges.back();


			//instantiate third half edge
			edges.push_back(new HalfEdgeEdge());
			edges.back()->vert = &mesh->Vertices.at(thirdVertIndex);

			edgeVertexIndexPair thirdPairing(uniqueIndex.at(thirdVertIndex), uniqueIndex.at(firstVertIndex));
			vertexIndexToHalfEdge.insert(VertexIndexToHalfEdgePtr::value_type(thirdPairing, edges.back()));

			//link second half edge to third half edge
			edges.at(edges.size() - 2)->nextEdge = edges.back();

			//link third half edge to first half edge
			edges.back()->nextEdge = edges.at(edges.size() - 3);

			//Debug::Log("mapped {0} and {1}", uniqueIndex.at(firstVertIndex), uniqueIndex.at(secondVertIndex));
			//Debug::Log("mapped {0} and {1}", uniqueIndex.at(secondVertIndex), uniqueIndex.at(thirdVertIndex));
			//Debug::Log("mapped {0} and {1}", uniqueIndex.at(thirdVertIndex), uniqueIndex.at(firstVertIndex));
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
		
		//test
		

		//*/

	}

	const std::vector<HalfEdgeEdge*>& ConvexCollider::GetEdges()
	{
		return edges;
	}





	





}

