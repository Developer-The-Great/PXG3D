#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "World.h"
#include "Mathf.h"
#include "MeshComponent.h"
#include "Vector3.h"
#include "HitInfo.h"
#include "Mat4.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Math.h"
#include "CameraComponent.h"
#include "PhysicsSceneGraphIterationInfo.h"
#include "PhysicsComponentContainer.h"
#include "PSGIIToAABB.h"
#include "ConvexCollider.h"
#include "HalfEdgeEdge.h"
#include "World.h"
#include "DebugDrawingManager.h"
#include "ClippintIterInfo.h"
#include "Input.h"
#include "KeyCode.h"

#include <array>
#include <limits>
#include "OctreeNode.h"
#include "DebugDrawingManager.h"
#include "Time.h"
#include "DebugDrawingManager.h"
#include "BenchmarkTimer.h"
#include <set>
#include "Rigidbody.h"
#include "GameObject.h"

namespace PXG
{
	Vector3 PhysicsEngine::gravity = Vector3(0,-9.8,0);
	float PhysicsEngine::baumgarteCoeff = 0.375f;

	int PhysicsEngine::jDebugAt = 0;
	int PhysicsEngine::iDebugAt = 0;

	PhysicsEngine::PhysicsEngine()
	{
		//Debug::SetDebugState(false);
		//OptimizeBroadPhase = std::bind(&PhysicsEngine::BruteForceBroadPhase,this,std::placeholders::_1,std::placeholders::_2);
		OptimizeBroadPhase = std::bind(&PhysicsEngine::BroadPhaseOctreeOptimization, this, std::placeholders::_1, std::placeholders::_2);
		
	}

	void PhysicsEngine::Integrate(float dt)
	{
		currentDt = dt;


		if (!world) 
		{ 
			Debug::Log("world ptr is empty!");
			return; 
		}

		auto children = world->GetChildren();

		for (auto child : children)
		{
			if (auto rigidbody = child->GetComponent<Rigidbody>())
			{
				
				rigidbody->Integrate(dt);
			}
		}
	}

	void PhysicsEngine::IncrementTickCount()
	{
		currentFrameTickCount++;
	}

	void PhysicsEngine::ResetTickCount()
	{
		currentFrameTickCount = 0;
	}

	void PhysicsEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}

	float PhysicsEngine::GetTickRate() const
	{
		return tickTime;
	}

	float PhysicsEngine::GetTickTimeRemaining() const
	{
		return tickTimeRemaining;
	}

	//float PhysicsEngine::GetCurrentTickRemaining() const
	//{
	//	return tickTimeRemaining;
	//}

	void PhysicsEngine::AccumulateTickTime(float tick)
	{
		tickTimeRemaining += tick;
	}

	bool PhysicsEngine::IsTicking()
	{
		if (tickTimeRemaining > tickTime && currentFrameTickCount < maxTickCountPerFrame)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	float PhysicsEngine::DecreaseRemainingTickTime()
	{
		if (tickTimeRemaining > tickTime)
		{
			tickTimeRemaining -= tickTime;
			return tickTime;
		}
		else
		{
			return  0;
		}
	}

	void PhysicsEngine::CheckCollisions()
	{
		std::vector<PhysicsSceneGraphIterationInfo> iterationResult;
		Mat4 transform = world->GetTransform()->GetWorldTransform();

		//go through the scene graph and find all physicsComponents that have colliders
		{
			//BenchmarkTimer timer("recursiveRetrievePhysicsComponent");
			recursiveRetrievePhysicsComponent(world, iterationResult, transform);
		}
		

		std::vector<PhysicsComponentContainer> physicsComponentContainers;

		{
			//BenchmarkTimer timer("OptimizeBroadPhase");
			OptimizeBroadPhase(iterationResult, physicsComponentContainers);
		}
		

		std::vector<Manifold> resultingManifolds;

		std::set<std::pair<unsigned int, unsigned int>> collisionPairings;

		//Debug::Log("------------------------------------------- Iterating through physics containers--------------");

		{
			//BenchmarkTimer timer("physicsComponentContainer loop");
			for (const auto& physicsComponentContainer : physicsComponentContainers)
			{
				//BenchmarkTimer timer("single physicsComponentContainer loop");
				for (int i = 0; i < physicsComponentContainer.PSGIIContainer.size(); i++)
				{

					for (int j = i + 1; j < physicsComponentContainer.PSGIIContainer.size(); j++)
					{
						Manifold m;

						//for now, assume that each physicsComponent only has 1 collider
						auto PSGIIA = physicsComponentContainer.PSGIIContainer.at(i);
						auto PSGIIB = physicsComponentContainer.PSGIIContainer.at(j);

						//TODO check for the following combinations
							//one of them is a non trigger object and the other is a trigger object
							//at least of them is a rigidbody
							
						auto searchResult = collisionPairings.find(std::pair<unsigned int, unsigned int>(PSGIIA.id, PSGIIB.id));

						//check if this pair has not been checked for collision
						if (searchResult == collisionPairings.end())
						{
							//check for collision
							m.physicsComponentA = PSGIIA.physicsComponent;
							m.physicsComponentB = PSGIIB.physicsComponent;
							m.transformA = PSGIIA.transform;
							m.transformB = PSGIIB.transform;

							auto colliderA = PSGIIA.physicsComponent->GetCollider();
							auto colliderB = PSGIIB.physicsComponent->GetCollider();


							colliderA->CheckCollision(colliderB, m);

							if (m.isColliding)
							{
								colliderA->FillInManifold(colliderB, m);
								resultingManifolds.push_back(m);
							}

							collisionPairings.insert(std::pair<unsigned int, unsigned int>(PSGIIA.id, PSGIIB.id));

						}

					}
				}


			}
		}
		
		//-------------------------- Resolve collisions with sequential impulse --------------------------//

		//for each manifold in resulting manifolds 
		for (Manifold& manifold : resultingManifolds)
		{
			for (PhysicsContact& contact : manifold.contactData)
			{
				contact.PreCalculateEffectiveMass();
			}
		}
			//precalculate Effective Mass


		//-------------------------- Apply sequential impulse -------------------------------------------//

		Debug::Log("");
		Debug::Log(" --------- Apply sequential impulse --------------");
		Debug::Log("");
		for (int i = 0; i < 4; i++)
		{
			for (auto& manifold : resultingManifolds)
			{
				int iter = 0;

				Debug::Log(" --------- resolving {0} contacts in manifold --------------" , manifold.contactData.size());

				for (PhysicsContact& contact : manifold.contactData)
				{
					//Debug::Log("----resolving {0} contact", iter);
					contact.ResolveCollision(currentDt);

					//Debug::Log("****impulse sum after resolution {0} ",contact.impulseSum);
					iter++;
				}
			}
		}
		

		//while iter count is smaller than max iter count

			//resolve contacts

	}

	Vector3 PhysicsEngine::GetGravity()
	{
		return gravity;
	}

	void PhysicsEngine::SetGravity(Vector3 newGravity)
	{
		gravity = newGravity;

	}

	float PhysicsEngine::GetBaumgarteCoefficient()
	{
		return baumgarteCoeff;
	}

	void PhysicsEngine::SetDebugDrawer(std::shared_ptr<DebugDrawingManager> debugDrawer)
	{
		this->debugDrawer = debugDrawer;
	}

	void PhysicsEngine::ResetTickTimeRemaining()
	{
		tickTimeRemaining = 0.0f;
	}

	bool PhysicsEngine::Raycast(const Vector3& position, const Vector3& direction, HitInfo& hitInfo, std::shared_ptr<World> world, bool usePhysicsComponent)
	{
		hitInfo.RayHit = false;
		hitInfo.T = FLT_MAX;

		recursiveGameObjectRaytrace(position, direction, hitInfo, world, Mat4(),usePhysicsComponent);

		return hitInfo.RayHit;
	}

	void PhysicsEngine::recursiveRetrievePhysicsComponent(std::shared_ptr<GameObject> rootObj, std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, Mat4 transform,int id)
	{
		Mat4 rootObjTransform = rootObj->GetTransform()->GetLocalTransform() * transform;

		int colliderId = id;

		//all gameObjects have a physicsComponent but may not have coliders. we are only interested in those that do have colliders
		if (rootObj->GetPhysicsComponent()->GetColliderCount() > 0)
		{

			PhysicsSceneGraphIterationInfo psgii;
			psgii.physicsComponent = rootObj->GetPhysicsComponent();
			
			psgii.transform = rootObjTransform;
			psgii.id = colliderId;

			physicsComponents.push_back(psgii);

		}

		
		for (const auto& child : rootObj->GetChildren())
		{
			colliderId++;
			recursiveRetrievePhysicsComponent(child, physicsComponents, rootObjTransform, colliderId);
		}

	}

	void PhysicsEngine::BruteForceBroadPhase(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers)
	{
		PhysicsComponentContainer physicsComponentGroupContainer;

		for (const auto& psgii : physicsComponents)
		{
			physicsComponentGroupContainer.PSGIIContainer.push_back(psgii);
		}
		physicsComponentContainers.push_back(physicsComponentGroupContainer);

	}

	void PhysicsEngine::BroadPhaseOctreeOptimization(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers)
	{

		auto node = std::make_shared<OctreeNode>();

		float minSize = FLT_MAX;
		float maxSize = -FLT_MAX;

		Vector3 initialNodeAABBMin = Vector3(minSize, minSize, minSize);
		Vector3 initialNodeAABBMax = Vector3(maxSize, maxSize, maxSize);

		
		for (auto psgii : physicsComponents)
		{

			//---------------------------------- Fill node's IteratorInfoToAABBCollection ----------------------------------//

			PSGIIToAABB iterationInfoToAABB;
			auto x = iterationInfoToAABB.PSGII;
	
			Mat4 worldTransform = psgii.transform;

			iterationInfoToAABB.PSGII = psgii;

			//create AABB Box for all physicsComponents with colliders
			iterationInfoToAABB.AABB = psgii.physicsComponent->CreateAABBFromTransformedColliders(worldTransform);
			//world->GetDebugDrawingManager()->InstantiateAABBRepresentation(iterationInfoToAABB.AABB.get(), Vector3(1, 0, 0), world->GetTimeSystem()->GetAverageDeltaTime());

			
			node->IteratorInfoToAABBCollection.push_back(iterationInfoToAABB);

		
			//---------------------------------- update node's initialNodeAABBMin and initialNodeAABBMax ----------------------------------//

			Vector3 currentAABBMin = iterationInfoToAABB.AABB->position - iterationInfoToAABB.AABB->halfWidths;
			Vector3 currentAABBMax = iterationInfoToAABB.AABB->position + iterationInfoToAABB.AABB->halfWidths;


			//check if any component of  currentAABBMin is smaller than initialNodeAABBMin
			if (currentAABBMin.x < initialNodeAABBMin.x)
			{
				initialNodeAABBMin.x = currentAABBMin.x;
			}

			if (currentAABBMin.y < initialNodeAABBMin.y)
			{
				initialNodeAABBMin.y = currentAABBMin.y;
			}

			if (currentAABBMin.z < initialNodeAABBMin.z)
			{
				initialNodeAABBMin.z = currentAABBMin.z;
			}

			//check if any component of  currentAABBMax is bigger than initialNodeAABBMax
			if (currentAABBMax.x > initialNodeAABBMax.x)
			{
				initialNodeAABBMax.x = currentAABBMax.x;
			}

			if (currentAABBMax.y > initialNodeAABBMax.y)
			{
				initialNodeAABBMax.y = currentAABBMax.y;
			}

			if (currentAABBMax.z > initialNodeAABBMax.z)
			{
				initialNodeAABBMax.z = currentAABBMax.z;
			}

		}

		Vector3 initialPosition = (initialNodeAABBMax + initialNodeAABBMin) * 0.5f;
		Vector3 initialHalfWidth = (initialNodeAABBMax - initialNodeAABBMin) * 0.5f;

		node->Box = std::make_shared<AABBBox>(initialPosition, initialHalfWidth);

		//world->GetDebugDrawingManager()->InstantiateAABBRepresentation(node->Box.get(), Vector3(1, 0, 0), world->GetTimeSystem()->GetAverageDeltaTime());


		
		std::vector<std::shared_ptr<OctreeNode>> finalNodes;

		
		
		//------------------------------------------ recursively split the node ---------------------------------//
		recursiveOctreeSplit(node, finalNodes, 0, minObjectCount, maxDepthCount);


		//------------------------------------------ Fill the PhysicsComponentContainer with PSGIIs from the split nodes---------------------------------//
		for(const auto& node : finalNodes)
		{
			//world->GetDebugDrawingManager()->InstantiateAABBRepresentation(node->Box.get(), Vector3(0, 1, 0), world->GetTimeSystem()->GetAverageDeltaTime());

			PhysicsComponentContainer physicsComponentContainer;

			auto iteratorInfoToAABBCollectionnode = node->IteratorInfoToAABBCollection;

			for (auto  iteratorInfoToAABB : iteratorInfoToAABBCollectionnode)
			{
				physicsComponentContainer.PSGIIContainer.push_back(iteratorInfoToAABB.PSGII);
			}


			physicsComponentContainers.push_back(physicsComponentContainer);
		}

	}

	void PhysicsEngine::recursiveOctreeSplit(std::shared_ptr<OctreeNode> node, std::vector<std::shared_ptr<OctreeNode>>& finalNodes, int currentdepthCount, const int minObjectCount, const int maxDepthCount)
	{

		std::vector<std::shared_ptr<OctreeNode>> splitNodes;

		splitNodes = node->Split();

		int depthCount = currentdepthCount + 1;

		for (const auto& splitNode : splitNodes)
		{
			if (splitNode->IteratorInfoToAABBCollection.size() <= minObjectCount || depthCount >= maxDepthCount)
			{
				finalNodes.push_back(splitNode);

			}
			else
			{
				recursiveOctreeSplit(splitNode, finalNodes, depthCount, minObjectCount, maxDepthCount);
			}
			
		}

	}

	bool PhysicsEngine:: FindSeperatingAxisByProjectingMeshAandBToFaceNormals(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
		const Mat4 & transformA, const Mat4 & transformB, const Vector3& positionA, const Vector3& positionB,  float & seperationFound)
	{

		Vector3 vecAToB = positionB - positionA;

		//Test all axes parallel to face normals of meshB
		for (int index = 0; index < collisionMeshB->Indices.size(); index += 3)
		{
			int v0Index = collisionMeshB->Indices.at(index);


			glm::vec3 axis = transformB.ToGLM() * glm::vec4(collisionMeshB->Vertices.at(v0Index).normal.ToGLMVec3(), 0);
			axis = glm::normalize(axis);

			if (TestAxisDirection(axis, collisionMeshA, collisionMeshB, transformA, transformB, positionA, positionB, seperationFound))
			{
				return true;
			}

		}

		return false;

	}

	bool PhysicsEngine::FindSeperatingAxisByExtremePointProjection(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB, 
		const Mat4 & transformA, const Mat4 & transformB, const Vector3 & positionA, const Vector3 & positionB, int& index,float& seperationFound)
	{
		index = -1;
		float greatestSeperation = -FLT_MAX;

		for (int i = 0; i < collisionMeshA->Indices.size(); i += 3)
		{
			int v0Index = collisionMeshA->Indices.at(i);

			//get face normal
			glm::vec3 axis = transformA.ToGLM() * glm::vec4(collisionMeshA->Vertices.at(v0Index).normal.ToGLMVec3(), 0);


			//triangleAPos is the position of one on
			glm::vec3 triangleAPos = transformA.ToGLM() * glm::vec4(collisionMeshA->Vertices.at(v0Index).position.ToGLMVec3(), 1);

			unsigned int supportIndexB = -1;
			Vector3 supportPoint;
			PhysicsEngine::GetSupportPoint(collisionMeshB, transformB, positionB, -axis, supportIndexB, supportPoint);

			Vector3 extremePointDirection = supportPoint - triangleAPos;

			float seperation = Mathf::Dot(extremePointDirection, axis);

			if (seperation > greatestSeperation)
			{
				greatestSeperation = seperation;
				index = i;
			}
			
			if (seperation > 0)
			{
				
				//Debug::Log("Get Support Point Called {0} ", gsCount);
				return true;
			}


		}

		seperationFound = greatestSeperation;
		//Debug::Log("Get Support Point Called {0} ", gsCount);
		return false;
	}


	bool PhysicsEngine::FindSeparatingAxisByBruteForceEdgeToEdgeCheck(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
		const Mat4& transformA, const Mat4& transformB, const Vector3& positionA,  const Vector3& positionB, float& seperationFound)
	{

		std::vector<Vector3> axisesTested;


		Vector3 vecAToB = positionB - positionA;

		//for a each triangleA in mesh A
		for (int i = 0; i < collisionMeshA->Indices.size(); i += 3)
		{
			std::array<Vector3, 3> triangleA =
			{
				collisionMeshA->Vertices.at(collisionMeshA->Indices.at(i)).position,
				collisionMeshA->Vertices.at(collisionMeshA->Indices.at(i + 1)).position,
				collisionMeshA->Vertices.at(collisionMeshA->Indices.at(i + 2)).position,
			};
			//for a each triangleB in mesh B
			for (int j = 0; j < collisionMeshB->Indices.size(); j += 3)
			{
				std::array<Vector3, 3> triangleB =
				{
					collisionMeshB->Vertices.at(collisionMeshB->Indices.at(j)).position,
					collisionMeshB->Vertices.at(collisionMeshB->Indices.at(j + 1)).position,
					collisionMeshB->Vertices.at(collisionMeshB->Indices.at(j + 2)).position,
				};

				for (int k = 0; k < triangleA.size(); k++)
				{
					Vector3 startEdgeA = triangleA[k];
					Vector3 endEdgeA = k + 1 >= triangleA.size() ? triangleA[0] : triangleA[k + 1];
					//get one of the edges of triangleA
					Vector3 edgeAObjectSpace = endEdgeA - startEdgeA;
					glm::vec3 edgeA = transformA.ToGLM() * glm::vec4(edgeAObjectSpace.ToGLMVec3(), 0);

					//glm::vec

					for (int l = 0; l < triangleB.size(); l++)
					{
						Vector3 startEdgeB = triangleB[l];
						Vector3 endEdgeB = l + 1 >= triangleB.size() ? triangleB[0] : triangleB[l + 1];
						//get one of the edges of triangleB
						Vector3 edgeBObjectSpace = endEdgeB - startEdgeB;
						glm::vec3 edgeB = transformB.ToGLM() * glm::vec4(edgeBObjectSpace.ToGLMVec3(), 0);

						//the cross product between them is the new axis
						Vector3 axis = Mathf::Cross(edgeA, edgeB);

						bool isAxisAlreadyChecked = false;

						for (const Vector3& vec : axisesTested)
						{
							if (Mathf::FloatVectorCompare(axis, vec))
							{
								isAxisAlreadyChecked = true;
								break;
							}
						}

						if (Mathf::FloatCompare(axis.Length(), 0.0f) || isAxisAlreadyChecked) 
						{ 
							continue; 
						}

						axisesTested.push_back(axis);

						if (TestAxisDirection(axis, collisionMeshA, collisionMeshB, transformA, transformB, positionA, positionB, seperationFound))
						{
							return true;
						}


					}

				}

			}
		}

		return false;
	}

	bool PhysicsEngine::FindSeparatingAxisByGaussMapEdgeToEdgeCheck(ConvexCollider * colliderA, ConvexCollider * colliderB, 
		const Mat4 & transformA, const Mat4 & transformB, const Vector3 & positionA, const Vector3 & positionB, float & seperationFound, Vector3& seperatingNormal, Vector3& seperationPosition)
	{
		float greatestSeperation = -FLT_MAX;
		HalfEdgeEdge* seperationEdge = nullptr;
		
		for (auto edge : colliderA->GetEdges())
		{
			int j = 0;
			for (auto otherEdge : colliderB->GetEdges())
			{
				if (edge->isPairingEqualNormal() || otherEdge->isPairingEqualNormal()) { continue; }
				
				//check if both edges create a minkowski difference face
				if (AttemptBuildMinkowskiFace(edge, otherEdge, transformA, transformB,j))
				{

					glm::vec3 transformedEdgeA = transformA.ToGLM() * glm::vec4(edge->GetEdgeDirection().ToGLMVec3(), 0);
					glm::vec3 transformedEdgeB = transformB.ToGLM() * glm::vec4(otherEdge->GetEdgeDirection().ToGLMVec3(), 0);

					Vector3 normal = Mathf::Cross(transformedEdgeA, transformedEdgeB);

					if (normal.IsZeroVector()) { continue; }

					normal.Normalize();

					glm::vec3 EdgeAVertexPosition = transformA.ToGLM() * glm::vec4(edge->vert->position.ToGLMVec3(), 1);
					glm::vec3 EdgeBVertexPosition = transformB.ToGLM() * glm::vec4(otherEdge->vert->position.ToGLMVec3(), 1);

					Vector3 vertexEdgeAToCenterA = Vector3(EdgeAVertexPosition) - positionA;

					if (Mathf::Dot(normal, vertexEdgeAToCenterA) < 0)
					{
						normal = -normal;
					}

					float distance = Mathf::Dot(normal, EdgeBVertexPosition - EdgeAVertexPosition);

					if (distance > greatestSeperation)
					{
						seperationEdge = edge;
						seperatingNormal = normal;
						greatestSeperation = distance;
					}

					if (distance > 0)
					{
						return true;
					}

					
				}

			}
		}
		
		if (seperationEdge)
		{
			seperationPosition = transformA.ToGLM() * glm::vec4(seperationEdge->vert->position.ToGLMVec3(), 1);
		}
		
		seperationFound = greatestSeperation;
	
		return false;
	}

	void PhysicsEngine::recursiveGameObjectRaytrace(const Vector3& position, const Vector3& direction, HitInfo & hitInfo, std::shared_ptr<GameObject> gameObject, Mat4 parentTransform, bool isUsingPhysicsComponent)
	{
		//raycast mesh component of current object
		auto meshes = isUsingPhysicsComponent? 
			gameObject->GetPhysicsComponent()->GetPhysicsMeshes() : 
		gameObject->GetMeshComponent()->GetMeshes();

		Mat4 Transform = gameObject->GetTransform()->GetLocalTransform() * parentTransform;
		
		for (const auto& mesh : meshes)
		{
			
			rayToMeshIntersection(position, direction, hitInfo, mesh, Transform, gameObject);
		}
		
		//raytrace mesh component of children
		for (const auto& child : gameObject->GetChildren())
		{
			recursiveGameObjectRaytrace(position, direction, hitInfo, child, Transform, isUsingPhysicsComponent);
		}
	}

	void PhysicsEngine::rayToMeshIntersection(const Vector3& position, const Vector3& direction, HitInfo & hitInfo, std::shared_ptr<Mesh> mesh, Mat4 objectTransform, std::shared_ptr<GameObject> owner)
	{

		for (int index = 0; index < mesh->Indices.size(); index+=3)
		{

			int v0Index = mesh->Indices[index];
			int v1Index = mesh->Indices[index+1];
			int v2Index = mesh->Indices[index+2];

			HitInfo triangleHitInfo;
			triangleHitInfo.RayHit = false;

			RayTriangleIntersection(
				mesh->Vertices.at(v0Index).position,
				mesh->Vertices.at(v1Index).position,
				mesh->Vertices.at(v2Index).position,
				position,
				direction,
				objectTransform,
				triangleHitInfo,
				owner);

			if (triangleHitInfo.RayHit)
			{
				Vector3 raycasterToHitPoint = Vector3(position - triangleHitInfo.Position);
				float foundT = raycasterToHitPoint.Length();
				if (foundT < hitInfo.T)
				{
					hitInfo = triangleHitInfo;
					hitInfo.T = foundT;
				}
			}


		}

	}

	void PhysicsEngine::RayTriangleIntersection(Vector3 vec1, Vector3 vec2, Vector3 vec3, const Vector3& rayPosition, const Vector3& rayDirection, Mat4 objectTransform, HitInfo& hitInfo, std::shared_ptr<GameObject> owner)
	{
		
		//-----------find a point where the ray intersects the plane where the triangle lies-------//
		HitInfo Result;

		glm::vec4 rayOrigin(rayPosition.ToGLMVec3(), 1);
		glm::vec4 rayDir(rayDirection.Normalized().ToGLMVec3(), 0);

		//transform ray into the triangle's model space
		rayOrigin =  glm::inverse(objectTransform.ToGLM()) * rayOrigin;
		rayDir = glm::normalize(glm::inverse(objectTransform.ToGLM()) * rayDir);


		Vector3 a(vec1.ToGLMVec3());
		Vector3 b(vec2.ToGLMVec3());
		Vector3 c(vec3.ToGLMVec3());

		Vector3 ObjectSpaceNormal =  -Mathf::Cross(a - b, c - b).Normalized();

		float normalDotDirection = Mathf::Dot(ObjectSpaceNormal, Vector3(rayDir.x, rayDir.y, rayDir.z));

		float normalDotPoint = Mathf::Dot(a, ObjectSpaceNormal);

		float OriginDotNormal = Mathf::Dot(Vector3(rayOrigin.x, rayOrigin.y, rayOrigin.z), ObjectSpaceNormal);

		float t = (normalDotPoint - OriginDotNormal) / normalDotDirection;

		//check if triangle is behind ray
		if (t < 0)
		{
			Result.RayHit = false;
			Result.T = t;
			Result.GameObjectHit = owner;
			hitInfo = Result;
			return;
		}


		glm::vec3 P = rayOrigin + rayDir * t;

		//---------check if that point is inside the the triangle using barycentric coordinates--------//

		Vector3 p(P.x, P.y, P.z);

		Vector3 v0 = b - a;
		Vector3 v1 = c - a;
		Vector3 v2 = p - a;

		float d00 = Mathf::Dot(v0, v0);
		float d10 = Mathf::Dot(v1, v0);
		float d01 = Mathf::Dot(v0, v1);
		float d11 = Mathf::Dot(v1, v1);
		float d20 = Mathf::Dot(v2, v0);
		float d21 = Mathf::Dot(v2, v1);

		float denom = d00 * d11 - d01 * d10;

		// apply cramers rule
		float v = (d20* d11 - d21 * d10) / denom;

		//---- P is in triangle if u + v + w = 1 ------//
		if (v < 0 || v > 1)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		float w = (d00 * d21 - d01 * d20) / denom;

		if (w < 0 || w > 1)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		float u = 1 - w - v;

		if (u < 0 || u > 1)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		//------------------------- ray hits triangle--------------------------//
		auto transformedNormal = glm::transpose(glm::inverse(objectTransform.ToGLM())) * glm::vec4(ObjectSpaceNormal.ToGLMVec3(), 0);
		Result.Normal = glm::vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z);
		Result.Normal.Normalize();

		glm::vec4 finalPosition = objectTransform.ToGLM() * glm::vec4(p.x, p.y, p.z, 1.0);
		//Debug::Log("u:{0} , v:{1} , w:{2}", u, v, w);
		Result.Position = Vector3(finalPosition.x,finalPosition.y,finalPosition.z);
		Result.GameObjectHit = owner;
		Result.RayHit = true;
		Result.T = t;

		hitInfo = Result;

	}

	void PhysicsEngine::GetSupportPoint(std::shared_ptr<Mesh> mesh, const Mat4& meshTransform,  const Vector3& position, const Vector3& direction, unsigned int & index,Vector3& vertexWorldPosition)
	{
		float largestProjection = -FLT_MAX;
		index = -1;

		for (int i = 0; i < mesh->Vertices.size(); i++)
		{

			glm::vec3 objectSpaceVertexToPosition = (mesh->Vertices.at(i).position - position).ToGLMVec3();
			glm::vec3 worldSpaceVertexToPosition = meshTransform.ToGLM() * glm::vec4(objectSpaceVertexToPosition, 0);

			float dotResult = Mathf::Dot(worldSpaceVertexToPosition, direction);

			if (dotResult > largestProjection)
			{
				largestProjection = dotResult;
				index = i;
				
			}
		}

		vertexWorldPosition = meshTransform.ToGLM() * glm::vec4(mesh->Vertices.at(index).position.ToGLMVec3(),1);
	}

	float PhysicsEngine::FindVectorToPlaneInterpolation(const Vector3& startPoint, const Vector3& endPoint, const Vector3& planePosition,const Vector3& planeNormal)
	{
		return Mathf::Dot(planePosition - startPoint, planeNormal) / Mathf::Dot((endPoint - startPoint).Normalized(),planeNormal);
	}

	void PhysicsEngine::GetSupportPointMinMax(std::shared_ptr<Mesh> mesh, const Mat4 & meshTransform, const Vector3 & position, const Vector3 & direction,
		unsigned int & indexMin, Vector3 & vertexWorldPositionMin, unsigned int & indexMax, Vector3 & vertexWorldPositionMax)
	{
		float largestProjection = -FLT_MAX;
		float smallestProjection = FLT_MAX;

		indexMin = -1;
		indexMax = -1;

		for (int i = 0; i < mesh->Vertices.size(); i++)
		{

			glm::vec3 objectSpaceVertexToPosition = (mesh->Vertices.at(i).position - position).ToGLMVec3();
			glm::vec3 worldSpaceVertexToPosition = meshTransform.ToGLM() * glm::vec4(objectSpaceVertexToPosition, 0);

			float dotResult = Mathf::Dot(worldSpaceVertexToPosition, direction);

			if (dotResult > largestProjection)
			{
				largestProjection = dotResult;
				indexMax = i;

			}

			if (dotResult < smallestProjection)
			{
				smallestProjection = dotResult;
				indexMax = i;
			}


		}

		vertexWorldPositionMin = meshTransform.ToGLM() * glm::vec4(mesh->Vertices.at(indexMin).position.ToGLMVec3(), 1);
		vertexWorldPositionMax = meshTransform.ToGLM() * glm::vec4(mesh->Vertices.at(indexMax).position.ToGLMVec3(), 1);


	}

	bool PhysicsEngine::TestAxisDirection(const Vector3& axis,std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
		const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound)
	{
		Vector3 vecAToB = positionB - positionA;

		Vector3 directionA = Mathf::Dot(vecAToB, axis) > Mathf::Dot(-vecAToB, axis) ? (axis) : (-axis);
		Vector3 directionB = -directionA;

		unsigned int indexA = -1, indexB = -1;
		Vector3 supportPointA, supportPointB;

		PhysicsEngine::GetSupportPoint(collisionMeshA, transformA, positionA, directionA, indexA, supportPointA);
		PhysicsEngine::GetSupportPoint(collisionMeshB, transformB, positionB, directionB, indexB, supportPointB);

		Vector3 CenterAToSupportPointA = supportPointA - positionA;
		Vector3 CenterBToSupportPointB = supportPointB - positionB;


		//d is the distance between the centers of ColliderA and ColliderB, projected into 'axis'
		float d = Mathf::Abs(Mathf::Dot(axis, vecAToB));

		//ra is the distance from the center of ColliderA to the vertex nearest vertex to B, projected into 'axis'
		float ra = Mathf::Abs(Mathf::Dot(axis, CenterAToSupportPointA));

		//rb is the distance from the center of ColliderB to the vertex nearest vertex to A, projected into 'axis'
		float rb = Mathf::Abs(Mathf::Dot(axis, CenterBToSupportPointB));

		if (d > ra + rb)
		{
			seperationFound = d - ra + rb;

			return true;
		}
		return false;
	}

	void PhysicsEngine::recursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject)
	{
		MeshComponentList.push_back(gameObject->GetMeshComponent());

		for (auto const& child : gameObject->GetChildren())
		{
			recursiveGetMeshComponents(MeshComponentList, child);
		}
	}

	void PhysicsEngine::GetMinMaxPositionOfMeshes(Vector3 & min, Vector3 & max, std::vector< std::shared_ptr<Mesh>> meshes)
	{
		float minVal = FLT_MAX;
		float maxVal = -FLT_MAX;

		Vector3 minResult = Vector3(minVal,minVal,minVal);
		Vector3 maxResult = Vector3(maxVal,maxVal,maxVal);

		
		for (const auto& mesh : meshes)
		{
			auto vertices = mesh->Vertices;
			//TODO refactor with GetMinMaxPositionOfVerticess
			for (const auto& vertex : vertices)
			{
				Vector3 vertexPosition = vertex.position;

				//check for minResult
				if (vertexPosition.x < minResult.x)
				{
					minResult.x = vertexPosition.x;
				}

				if (vertexPosition.y < minResult.y)
				{
					minResult.y = vertexPosition.y;
				}

				if (vertexPosition.z < minResult.z)
				{
					minResult.z = vertexPosition.z;
				}

				//check for maxResult
				if (vertexPosition.x > maxResult.x)
				{
					maxResult.x = vertexPosition.x;
				}

				if (vertexPosition.y > maxResult.y)
				{
					maxResult.y = vertexPosition.y;
				}

				if (vertexPosition.z > maxResult.z)
				{
					maxResult.z = vertexPosition.z;
				}

			}

		}

		min = minResult;
		max = maxResult;
	}

	
	void PhysicsEngine::GetMinMaxPositionOfVertices(Vector3 & min, Vector3 & max, const std::vector<Vector3> vertices)
	{
		float minVal = FLT_MAX;
		float maxVal = -FLT_MAX;

		Vector3 minResult = Vector3(minVal, minVal, minVal);
		Vector3 maxResult = Vector3(maxVal, maxVal, maxVal);

		for (const auto& vertex : vertices)
		{
			//check for minResult
			if (vertex.x < minResult.x)
			{
				minResult.x = vertex.x;
			}

			if (vertex.y < minResult.y)
			{
				minResult.y = vertex.y;
			}

			if (vertex.z < minResult.z)
			{
				minResult.z = vertex.z;
			}



			//check for maxResult
			if (vertex.x > maxResult.x)
			{
				maxResult.x = vertex.x;
			}

			if (vertex.y > maxResult.y)
			{
				maxResult.y = vertex.y;
			}

			if (vertex.z > maxResult.z)
			{
				maxResult.z = vertex.z;
			}

		}

		min = minResult;
		max = maxResult;

	}

	bool PhysicsEngine::AttemptBuildMinkowskiFace(HalfEdgeEdge* edgeA, HalfEdgeEdge* edgeB, const Mat4& transformA, const Mat4& transformB,int j = -1)
	{
		Vector3 transformedNormalA1 = glm::vec3(transformA.ToGLM() * glm::vec4(edgeA->GetNormal()->ToGLMVec3(), 0));
		Vector3 transformedNormalA2 = glm::vec3(transformA.ToGLM() * glm::vec4(edgeA->GetPairingNormal()->ToGLMVec3(), 0));

		Vector3 transformedNormalB1 = -glm::vec3(transformB.ToGLM() * glm::vec4(edgeB->GetNormal()->ToGLMVec3(), 0));
		Vector3 transformedNormalB2 = -glm::vec3(transformB.ToGLM() * glm::vec4(edgeB->GetPairingNormal()->ToGLMVec3(), 0));

		return IsMinkowskiFace(transformedNormalA1, transformedNormalA2,transformedNormalB1, transformedNormalB2);
	}

	bool PhysicsEngine::IsMinkowskiFace(Vector3 & transformedNormalA1, Vector3& transformedNormalA2,
		Vector3& transformedNormalB1, Vector3& transformedNormalB2)
	{
		Vector3 edgeANormalPlane = Mathf::Cross(transformedNormalA1, transformedNormalA2);
		Vector3 edgeBNormalPlane = Mathf::Cross(transformedNormalB1, transformedNormalB2);

		//check if plane created from the cross products of the normals of edgeA divides the normals of edgeB
		float normalPlaneAToBNormalsResult = Mathf::Dot(transformedNormalB1, edgeANormalPlane)
			* Mathf::Dot(transformedNormalB2, edgeANormalPlane);

		//normals of B are found to be in the same side of the plane ,gauss Map arcs do not intersect
		if (normalPlaneAToBNormalsResult > 0 || Mathf::FloatCompare(normalPlaneAToBNormalsResult,0.0f)) { return false; }

		//check if plane created from the cross products of the normals of edgeB divides the normals of edgeA
		float normalPlaneBToANormalsResult = Mathf::Dot(transformedNormalA1, edgeBNormalPlane)
			* Mathf::Dot(transformedNormalA2, edgeBNormalPlane);

		//normals of A are found to be in the same side of the plane ,gauss Map arcs do not intersect
		if (normalPlaneBToANormalsResult > 0 || Mathf::FloatCompare(normalPlaneBToANormalsResult,0.0f)) { return false; }


		//check if normals are in the same hemisphere
		Vector3 edgeAEdgeBNormalPlane = Mathf::Cross(transformedNormalA2, transformedNormalB2);

		float edgeAEdgeBNormalPlaneToOtherNormals = Mathf::Dot(transformedNormalA1, edgeAEdgeBNormalPlane)
			* Mathf::Dot(transformedNormalB1, edgeAEdgeBNormalPlane);

		//normals of A are found to be on different sides of the plane, 
		//gauss Map arcs are in different sides of the hemisphere
		if (edgeAEdgeBNormalPlaneToOtherNormals < 0 || Mathf::FloatCompare(edgeAEdgeBNormalPlaneToOtherNormals,0.0f)) { return false; }

		return true;
	}

	void PhysicsEngine::SutherlandHodgmanClipping(ConvexCollider* referencePolyhedron, ConvexCollider* incidentPolyhedron, std::vector<Vector3>& contactPoints)
	{

		//DebugDrawingManager::drawingManagerStaticPtr->SetShouldDraw(false);
		//Debug::SetDebugState(false);
		//BenchmarkTimer SH("SutherlandHodgmanClipping");

		//Debug::Log("SH between {0} and {1} ", referencePolyhedron->GetPhysicsComponentContainer()->GetOwner()->name, incidentPolyhedron->GetPhysicsComponentContainer()->GetOwner()->name);
		//Debug::Log("jDebugAt {0} ", jDebugAt);

		auto faces = incidentPolyhedron->GetFaces();


		auto referenceMesh = referencePolyhedron->GetMesh();

		auto refPolyhedronPhysicsComponent = referencePolyhedron->GetPhysicsComponentContainer();
		const auto& refTransform = refPolyhedronPhysicsComponent->GetOwner()->GetTransform()->GetWorldTransform();

		auto refIncidentPhysicsComponent = incidentPolyhedron->GetPhysicsComponentContainer();
		const auto& incTransform = refIncidentPhysicsComponent->GetOwner()->GetTransform()->GetWorldTransform();

		auto referenceFaces = referencePolyhedron->GetFaces();
		auto incidentFaces = incidentPolyhedron->GetFaces();

		//DEBUG_PreviewPolyhedronEdges(incidentPolyhedron, incTransform);
		

		std::vector<Vector3> resultingClippingPoints;

		int k = 0;

		for (const auto& incidentFace : incidentFaces)
		{
			std::vector<Vector3> resultingPolygon;

			//--------------------------- populate list of Vector3s --------------------

			for (const HalfEdgeEdge* edge : incidentFace.EdgesOnFace)
			{
				Vector3 worldSpaceEdgePosition = incTransform.ToGLM() * glm::vec4(edge->GetPosition().ToGLMVec3(), 1);
				resultingPolygon.push_back(worldSpaceEdgePosition);
			}

			int l = 0;
			//-------------------- Split incident face with reference face -----------------------------------//
			for (const auto& face : referenceFaces)
			{
				glm::vec3 planePosition = refTransform.ToGLM() * glm::vec4(face.position.ToGLMVec3(), 1);
				glm::vec3 planeNormal = glm::transpose(glm::inverse(refTransform.ToGLM()))  * glm::vec4(face.normal.ToGLMVec3(), 0);

				auto inputPolygon = resultingPolygon;
				resultingPolygon.clear();

				int j = 0;

				for (int i = 0; i < inputPolygon.size(); i++)
				{
					Vector3 worldEdgePosition = inputPolygon.at(i);

					int nextPolygonIndex = (i + 1 >= inputPolygon.size()) ? 0 : i + 1;

					Vector3 worldNextEdgePosition = inputPolygon.at(nextPolygonIndex);


					bool isCurrentPointUnderPlane = !PhysicsEngine::IsPointAbovePlane(planeNormal, planePosition, worldEdgePosition);
					bool isNextPointUnderPlane = !PhysicsEngine::IsPointAbovePlane(planeNormal, planePosition, worldNextEdgePosition);

					const Vector3& pointUnderPlane = isCurrentPointUnderPlane ? worldEdgePosition : worldNextEdgePosition;
					const Vector3& pointAbovePlane = isCurrentPointUnderPlane ? worldNextEdgePosition : worldEdgePosition;


					if (isCurrentPointUnderPlane && isNextPointUnderPlane)
					{
						resultingPolygon.push_back(worldNextEdgePosition);
					}
					else if (!isCurrentPointUnderPlane && isNextPointUnderPlane)
					{
						Vector3 intersectionPoint;
						//intersection point
						if (PhysicsEngine::FindLineToPlaneIntersectionPoint(planeNormal, planePosition,
							pointUnderPlane, pointAbovePlane, intersectionPoint))
						{
							resultingPolygon.push_back(intersectionPoint);
						}

						resultingPolygon.push_back(worldNextEdgePosition);

					}
					else if (isCurrentPointUnderPlane && !isNextPointUnderPlane)
					{

						Vector3 intersectionPoint;
						//intersection point
						if (PhysicsEngine::FindLineToPlaneIntersectionPoint(planeNormal, planePosition,
							pointUnderPlane, pointAbovePlane, intersectionPoint))
						{
							resultingPolygon.push_back(intersectionPoint);
						}
					}
					/*else if (!isCurrentPointUnderPlane && !isNextPointUnderPlane)
					{

					}*/
					j++;
				}
				l++;
			}
			
			resultingClippingPoints.insert(resultingClippingPoints.end(), resultingPolygon.begin(), resultingPolygon.end());

			k++;
		}


		//--------------------------------- Ensure contact points are unique and put them in the 'contactPoints' collection --------------------------------//

		std::vector<Vector3> encounteredContactPoints;

		for (const Vector3& polygonClippingPoint : resultingClippingPoints)
		{
			bool isContactPointEncountered = false;

			for (const Vector3& encounteredContactPoint : encounteredContactPoints)
			{
				if (Mathf::FloatVectorCompare(encounteredContactPoint, polygonClippingPoint))
				{
					isContactPointEncountered = true;
					break;
				}

			}

			if (!isContactPointEncountered)
			{
				contactPoints.push_back(polygonClippingPoint);
				encounteredContactPoints.push_back(polygonClippingPoint);
			}
		
		}

	}

	void PhysicsEngine::DEBUG_PreviewPolyhedronEdges(ConvexCollider * collider,const Mat4& transform)
	{
		auto faces = collider->GetFaces();

		for (size_t i = 0; i < faces.size(); i++)
		{
			auto halfEdges = faces.at(i).EdgesOnFace;

			for (size_t j = 0; j < halfEdges.size(); j++)
			{
				if (i == iDebugAt && j == jDebugAt)
				{
					glm::vec3 worldSpaceEdgePosition = transform.ToGLM() * glm::vec4(halfEdges.at(j)->GetPosition().ToGLMVec3(), 1);
					glm::vec3 nextWorldSpaceEdgePosition = transform.ToGLM() * glm::vec4(halfEdges.at(j)->nextEdge->GetPosition().ToGLMVec3(), 1);

					DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
						worldSpaceEdgePosition, Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), Vector3(0, 1, 0), PXG_THIS_FRAME_ONLY);

					DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
						nextWorldSpaceEdgePosition, Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), Vector3(0, 0.5, 0), PXG_THIS_FRAME_ONLY);
				}

			}
		}
	}

	Vector3 PhysicsEngine::GetOrthographicCameraWorldPosition(float x, float y, float screenWidth, float screenHeight, std::shared_ptr<World> world)
	{
		auto camera = world->GetCamera();
		if (!camera) { return Vector3(); }

		auto transform = camera->GetOwner()->GetTransform();
		Vector3 position = transform->GetPosition();
		Vector3 up = transform->GetUp();
		Vector3 right = transform->GetRight();

		Vector3 topLeftPosition = position - (right * float(screenWidth / 2.0f)) + (up * screenHeight / 2.0f);

		topLeftPosition = topLeftPosition + right * (x / screenWidth)*screenWidth - up * (y / screenHeight)*screenHeight;

		return topLeftPosition;
	}
	float PhysicsEngine::PointDistanceToPlane(const Vector3 & planeNormal, const Vector3 & planePosition, const Vector3 & point)
	{
		return Mathf::Dot(point - planePosition, planeNormal);
	}
	bool PhysicsEngine::IsPointAbovePlane(const Vector3 & planeNormal, const Vector3 & planePosition, const Vector3 & point)
	{
		return PointDistanceToPlane(planeNormal,planePosition,point) > 0.0f;
	}
	bool PhysicsEngine::FindLineToPlaneIntersectionPoint(const Vector3 & planeNormal, const Vector3 & planePosition, const Vector3 & startPoint, const Vector3 & endPoint, Vector3 & intersectionPoint)
	{
		Vector3 direction = endPoint - startPoint;

		if(Mathf::FloatCompare(Mathf::Dot(direction, planeNormal),0.0f))
		{
			return false;
		}


		float t = PhysicsEngine::FindVectorToPlaneInterpolation(startPoint, endPoint, planePosition, planeNormal);

		intersectionPoint = startPoint + direction.Normalized() * t;

		return true;
	}


	/*
		//TODO make this an actual function for future testing
		for (size_t i = 0; i < faces.size(); i++)
		{
			auto halfEdges = faces.at(i).EdgesOnFace;

			for (size_t j = 0; j < halfEdges.size(); j++)
			{
				if (i == iDebugAt && j == jDebugAt)
				{
					glm::vec3 worldSpaceEdgePosition = incTransform.ToGLM() * glm::vec4(halfEdges.at(j)->GetPosition().ToGLMVec3(), 1);
					glm::vec3 nextWorldSpaceEdgePosition = incTransform.ToGLM() * glm::vec4(halfEdges.at(j)->nextEdge-> GetPosition().ToGLMVec3(), 1);

					DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
						worldSpaceEdgePosition, Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), Vector3(0, 1, 0), PXG_THIS_FRAME_ONLY);

					DebugDrawingManager::drawingManagerStaticPtr->InstantiateCube(
						nextWorldSpaceEdgePosition, Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), Vector3(0, 0.5, 0), PXG_THIS_FRAME_ONLY);
				}

			}
		}
	
	
	
	*/
}

