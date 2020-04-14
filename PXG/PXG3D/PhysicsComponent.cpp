#include "PhysicsComponent.h"
#include "Mat4.h"
#include "ColorMaterial.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "CollisionCubeParams.h"
#include "PhysicsCollider.h"
#include "CubeCollider.h"
#include "ConvexCollider.h"
#include "SphereCollider.h"
#include "AABBBox.h"
#include <memory>

namespace PXG
{
	PhysicsComponent::PhysicsComponent() : Component()
	{
		physicsRenderingMaterial = std::make_shared<ColorMaterial>(Vector3(1,1,1));
	}

	void PhysicsComponent::SetPhysicsDrawingMaterial(std::shared_ptr<AbstractMaterial> newPhysicsDrawingMaterial)
	{
		physicsRenderingMaterial = newPhysicsDrawingMaterial;
	}

	void PhysicsComponent::Start()
	{

	}

	void PhysicsComponent::FixedUpdate(float tick)
	{
	}

	void PhysicsComponent::ConstructPhysicsRepresentationFromMeshComponent()
	{
		auto meshComponentMeshes = GetOwner()->GetMeshComponent()->GetMeshes();

		for (const auto& mesh : meshComponentMeshes)
		{
			std::shared_ptr<ConvexCollider> convexCollider = std::make_shared<ConvexCollider>();
			convexCollider->SetMesh(mesh);
			physicsColliders.push_back(convexCollider);

		}
	}


	void PhysicsComponent::DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection)
	{

		auto owner = GetOwner();
		//set uniforms for material

		Mat4 currentTransform = owner->GetTransform()->GetLocalTransform() * parentTransform;

		if (physicsRenderingMaterial)
		{
			physicsRenderingMaterial->SendUniforms(owner->GetWorld(), currentTransform, view, projection);

			for (auto const& physicsCollider : physicsColliders)
			{
				physicsCollider->GetMesh()->Draw(physicsRenderingMaterial->GetShader());
			}
		}


		for (std::shared_ptr<GameObject> const& child : owner->GetChildren())
		{
			child->GetPhysicsComponent()->DrawPhysicsRepresentation(currentTransform, view, projection);
		}



	}

	std::shared_ptr<AABBBox> PhysicsComponent::CreateAABBFromTransformedColliders(Mat4 & transform)
	{
		Vector3 objectSpaceMin, objectSpaceMax;

		PhysicsEngine::GetMinMaxPositionOfMeshes(objectSpaceMin, objectSpaceMax, GetPhysicsMeshes());

		Vector3 forward = Vector3(0, 0, objectSpaceMax.z - objectSpaceMax.z);
		Vector3 Up = Vector3(0, objectSpaceMax.y - objectSpaceMax.y,0);
		Vector3 Right = Vector3(objectSpaceMax.x - objectSpaceMax.x, 0, 0);

		glm::vec3 min = transform.ToGLM() * glm::vec4(objectSpaceMin.ToGLMVec3(), 1);
		glm::vec3 minPlusForward = transform.ToGLM() * glm::vec4((objectSpaceMin + forward).ToGLMVec3(), 1);
		glm::vec3 minPlusUp = transform.ToGLM() * glm::vec4((objectSpaceMin + Up).ToGLMVec3(), 1);
		glm::vec3 minPlusRight = transform.ToGLM() * glm::vec4((objectSpaceMin + Right).ToGLMVec3(), 1);
		
		glm::vec3 max = transform.ToGLM() * glm::vec4(objectSpaceMax.ToGLMVec3(), 0);
		glm::vec3 maxMinusBackward = transform.ToGLM() * glm::vec4((objectSpaceMin - forward).ToGLMVec3(), 1); 
		glm::vec3 maxMinusDown = transform.ToGLM() * glm::vec4((objectSpaceMin - Up).ToGLMVec3(), 1);
		glm::vec3 minMinusRight = transform.ToGLM() * glm::vec4((objectSpaceMin - Right).ToGLMVec3(), 1);

		std::vector<Vector3> verticesContainer;

		verticesContainer.push_back(min);
		verticesContainer.push_back(minPlusForward);
		verticesContainer.push_back(minPlusUp);
		verticesContainer.push_back(minPlusRight);

		verticesContainer.push_back(max);
		verticesContainer.push_back(maxMinusBackward);
		verticesContainer.push_back(maxMinusDown);
		verticesContainer.push_back(minMinusRight);

		Vector3 worldSpaceMin, worldSpaceMax;

		PhysicsEngine::GetMinMaxPositionOfVertices(worldSpaceMin, worldSpaceMax, verticesContainer);

		Vector3 halfWidth = (worldSpaceMax - worldSpaceMin) * 0.5f;

		return std::make_shared<AABBBox>(glm::vec3(transform.Matrix[3]), halfWidth);

	}

	void PhysicsComponent::SetIsTrigger(bool newTriggerState)
	{
		isTrigger = newTriggerState;
	}

	bool PhysicsComponent::IsTrigger() const
	{
		return isTrigger;
	}

	std::vector<std::shared_ptr<Mesh>> PhysicsComponent::GetPhysicsMeshes() const
	{
		std::vector<std::shared_ptr<Mesh>> meshes;


		for (const auto& collider : physicsColliders)
		{
			meshes.push_back(collider->GetMesh());
		}


		return meshes;
	}

	std::shared_ptr<PhysicsCollider> PhysicsComponent::GetCollider(int i)
	{
		return physicsColliders.at(i);
	}

	int PhysicsComponent::GetColliderCount() const
	{
		return physicsColliders.size();
	}

	void PhysicsComponent::ConstructCollisionCube(const CollisionCubeParams& CubeParams)
	{
		std::shared_ptr<Mesh> collisionCube = std::make_shared<Mesh>();
		
		auto meshComponentMeshes = GetOwner()->GetMeshComponent()->GetMeshes();

		//get min and max vertices
		Vector3 Min, Max;

		PhysicsEngine::GetMinMaxPositionOfMeshes(Min, Max, meshComponentMeshes);

		//get all 36 points that will be used to make vertices

		//---------- If a Parameter was specified, use that param. Use Max - Min if its not specified--------------//

		Vector3 xAdd = Mathf::FloatCompare(CubeParams.widthFromMin,0.0f)? 
			Vector3(Max.x - Min.x, 0, 0) : 
			Vector3(CubeParams.widthFromMin, 0, 0);

		Vector3 yAdd = Mathf::FloatCompare(CubeParams.heightFromMin, 0.0f) ? 
			Vector3(0,Max.y - Min.y, 0) :
			Vector3(0, CubeParams.heightFromMin, 0);
			
		Vector3 zAdd = Mathf::FloatCompare(CubeParams.breadthFromMin, 0.0f) ?
			Vector3(0, 0, Max.z - Min.z) :
			Vector3(0, 0, CubeParams.breadthFromMin);


		Vector3 lowerV0 = Min + CubeParams.offset;
		Vector3 lowerV1 = lowerV0 + xAdd;
		Vector3 lowerV2 = lowerV0 + zAdd;
		Vector3 lowerV3 = lowerV0 + xAdd + zAdd;

		Vector3 upperV0 = Min + yAdd;
		Vector3 upperV1 = upperV0 + xAdd;
		Vector3 upperV2 = upperV0 + zAdd;
		Vector3 upperV3 = upperV0 + xAdd + zAdd;

		auto cubeCollider = std::make_shared<CubeCollider>();
		cubeCollider->SetMesh(Mesh::InstantiateCubeMesh(lowerV0, lowerV1, lowerV2, lowerV3, upperV0, upperV1, upperV2, upperV3));


		physicsColliders.push_back(cubeCollider);

	}
	void PhysicsComponent::ConstructCollisionCube()
	{
		//is empty
		CollisionCubeParams params;
		ConstructCollisionCube(params);
	}
}

