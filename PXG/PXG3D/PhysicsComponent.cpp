#include "PhysicsComponent.h"
#include "Mat4.h"
#include "ColorMaterial.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Mesh.h"
#include "PhysicsEngine.h"
#include "CollisionCubeParams.h"

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
		meshes = meshComponentMeshes;
	}

	//TODO this looks pretty similar to how mesh component draws things, find a way to refactor it
	void PhysicsComponent::DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection)
	{

		auto owner = GetOwner();
		//set uniforms for material

		Mat4 currentTransform = owner->GetTransform()->GetLocalTransform() * parentTransform;

		if (physicsRenderingMaterial)
		{
			physicsRenderingMaterial->SendUniforms(owner->GetWorld(), currentTransform, view, projection);

			for (auto const& mesh : meshes)
			{
				mesh->Draw(physicsRenderingMaterial->GetShader());
			}
		}


		for (std::shared_ptr<GameObject> const& child : owner->GetChildren())
		{
			child->GetPhysicsComponent()->DrawPhysicsRepresentation(currentTransform, view, projection);
		}



	}
	std::vector<std::shared_ptr<Mesh>> PhysicsComponent::GetPhysicsMeshes()
	{
		return meshes;
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

		//---------------------Create top and bottom face--------------------------//
		Vector3 topNormal = Vector3(0.0, 1.0, 0.0);
		Vector3 bottomNormal = Vector3(0.0, -1.0, 0.0);

		{
			//top face
			Vertex topFaceTriangle1V0;
			topFaceTriangle1V0.position = upperV0;
			topFaceTriangle1V0.normal = topNormal;

			Vertex topFaceTriangle1V1;
			topFaceTriangle1V1.position = upperV1;
			topFaceTriangle1V1.normal = topNormal;

			Vertex topFaceTriangle1V2;
			topFaceTriangle1V2.position = upperV3;
			topFaceTriangle1V2.normal = topNormal;

			Vertex topFaceTriangle2V0;
			topFaceTriangle2V0.position = upperV0;
			topFaceTriangle2V0.normal = topNormal;

			Vertex topFaceTriangle2V1;
			topFaceTriangle2V1.position = upperV3;
			topFaceTriangle2V1.normal = topNormal;

			Vertex topFaceTriangle2V2;
			topFaceTriangle2V2.position = upperV2;
			topFaceTriangle2V2.normal = topNormal;

			//top face push_back
			collisionCube->Vertices.push_back(topFaceTriangle1V0);
			collisionCube->Vertices.push_back(topFaceTriangle1V1);
			collisionCube->Vertices.push_back(topFaceTriangle1V2);

			collisionCube->Vertices.push_back(topFaceTriangle2V0);
			collisionCube->Vertices.push_back(topFaceTriangle2V1);
			collisionCube->Vertices.push_back(topFaceTriangle2V2);
		}

		{
			//bottom face
			Vertex bottomFaceTriangle1V0;
			bottomFaceTriangle1V0.position = lowerV0;
			bottomFaceTriangle1V0.normal = bottomNormal;

			Vertex bottomFaceTriangle1V1;
			bottomFaceTriangle1V1.position = lowerV1;
			bottomFaceTriangle1V1.normal = bottomNormal;

			Vertex bottomFaceTriangle1V2;
			bottomFaceTriangle1V2.position = lowerV3;
			bottomFaceTriangle1V2.normal = bottomNormal;

			Vertex bottomFaceTriangle2V0;
			bottomFaceTriangle2V0.position = lowerV0;
			bottomFaceTriangle2V0.normal = bottomNormal;

			Vertex bottomFaceTriangle2V1;
			bottomFaceTriangle2V1.position = lowerV3;
			bottomFaceTriangle2V1.normal = bottomNormal;

			Vertex bottomFaceTriangle2V2;
			bottomFaceTriangle2V2.position = lowerV2;
			bottomFaceTriangle2V2.normal = bottomNormal;

			//bottom face push_back
			collisionCube->Vertices.push_back(bottomFaceTriangle1V0);
			collisionCube->Vertices.push_back(bottomFaceTriangle1V1);
			collisionCube->Vertices.push_back(bottomFaceTriangle1V2);

			collisionCube->Vertices.push_back(bottomFaceTriangle2V0);
			collisionCube->Vertices.push_back(bottomFaceTriangle2V1);
			collisionCube->Vertices.push_back(bottomFaceTriangle2V2);

		}

		//---------------------Create forward and back face-----------------------------//
		Vector3 forwardNormal = Vector3(0.0, 0.0, 1.0);
		Vector3 backNormal = Vector3(-0.0, -0.0, -1.0);


		{
			//forward face
			Vertex forwardFaceTriangle1V0;
			forwardFaceTriangle1V0.position = upperV3;
			forwardFaceTriangle1V0.normal = forwardNormal;

			Vertex forwardFaceTriangle1V1;
			forwardFaceTriangle1V1.position = lowerV3;
			forwardFaceTriangle1V1.normal = forwardNormal;

			Vertex forwardFaceTriangle1V2;
			forwardFaceTriangle1V2.position = upperV2;
			forwardFaceTriangle1V2.normal = forwardNormal;

			Vertex forwardFaceTriangle2V0;
			forwardFaceTriangle2V0.position = upperV2;
			forwardFaceTriangle2V0.normal = forwardNormal;

			Vertex forwardFaceTriangle2V1;
			forwardFaceTriangle2V1.position = lowerV3;
			forwardFaceTriangle2V1.normal = forwardNormal;

			Vertex forwardFaceTriangle2V2;
			forwardFaceTriangle2V2.position = lowerV2;
			forwardFaceTriangle2V2.normal = forwardNormal;

			//forward face push_back
			collisionCube->Vertices.push_back(forwardFaceTriangle1V0);
			collisionCube->Vertices.push_back(forwardFaceTriangle1V1);
			collisionCube->Vertices.push_back(forwardFaceTriangle1V2);

			collisionCube->Vertices.push_back(forwardFaceTriangle2V0);
			collisionCube->Vertices.push_back(forwardFaceTriangle2V1);
			collisionCube->Vertices.push_back(forwardFaceTriangle2V2);

		}

		{
			//backward face 
			Vertex backwardFaceTriangle1V0;
			backwardFaceTriangle1V0.position = upperV0;
			backwardFaceTriangle1V0.normal = backNormal;

			Vertex backwardFaceTriangle1V1;
			backwardFaceTriangle1V1.position = upperV1;
			backwardFaceTriangle1V1.normal = backNormal;

			Vertex backwardFaceTriangle1V2;
			backwardFaceTriangle1V2.position = lowerV1;
			backwardFaceTriangle1V2.normal = backNormal;

			Vertex backwardFaceTriangle2V0;
			backwardFaceTriangle2V0.position = upperV0;
			backwardFaceTriangle2V0.normal = backNormal;

			Vertex backwardFaceTriangle2V1;
			backwardFaceTriangle2V1.position = lowerV0;
			backwardFaceTriangle2V1.normal = backNormal;

			Vertex backwardFaceTriangle2V2;
			backwardFaceTriangle2V2.position = lowerV1;
			backwardFaceTriangle2V2.normal = backNormal;

			collisionCube->Vertices.push_back(backwardFaceTriangle1V0);
			collisionCube->Vertices.push_back(backwardFaceTriangle1V1);
			collisionCube->Vertices.push_back(backwardFaceTriangle1V2);

			collisionCube->Vertices.push_back(backwardFaceTriangle2V0);
			collisionCube->Vertices.push_back(backwardFaceTriangle2V1);
			collisionCube->Vertices.push_back(backwardFaceTriangle2V2);


		}

		//---------------------Create right and left face--------------------------//
		Vector3 rightNormal = Vector3(1.0, 0.0, 0.0);
		Vector3 leftNormal = Vector3(-1.0, -0.0, 0.0);

		{
			//right face 
			Vertex rightFaceTriangle1V0;
			rightFaceTriangle1V0.position = upperV1;
			rightFaceTriangle1V0.normal = rightNormal;

			Vertex rightFaceTriangle1V1;
			rightFaceTriangle1V1.position = lowerV3;
			rightFaceTriangle1V1.normal = rightNormal;

			Vertex rightFaceTriangle1V2;
			rightFaceTriangle1V2.position = upperV3;
			rightFaceTriangle1V2.normal = rightNormal;

			Vertex rightFaceTriangle2V0;
			rightFaceTriangle2V0.position = upperV1;
			rightFaceTriangle2V0.normal = rightNormal;

			Vertex rightFaceTriangle2V1;
			rightFaceTriangle2V1.position = lowerV1;
			rightFaceTriangle2V1.normal = rightNormal;

			Vertex rightFaceTriangle2V2;
			rightFaceTriangle2V2.position = lowerV3;
			rightFaceTriangle2V2.normal = rightNormal;

			collisionCube->Vertices.push_back(rightFaceTriangle1V0);
			collisionCube->Vertices.push_back(rightFaceTriangle1V1);
			collisionCube->Vertices.push_back(rightFaceTriangle1V2);

			collisionCube->Vertices.push_back(rightFaceTriangle2V0);
			collisionCube->Vertices.push_back(rightFaceTriangle2V1);
			collisionCube->Vertices.push_back(rightFaceTriangle2V2);
		}

		{
			//left face 
			Vertex leftFaceTriangle1V0;
			leftFaceTriangle1V0.position = upperV0;
			leftFaceTriangle1V0.normal = leftNormal;

			Vertex leftFaceTriangle1V1;
			leftFaceTriangle1V1.position = upperV2;
			leftFaceTriangle1V1.normal = leftNormal;

			Vertex leftFaceTriangle1V2;
			leftFaceTriangle1V2.position = lowerV2;
			leftFaceTriangle1V2.normal = leftNormal;

			Vertex leftFaceTriangle2V0;
			leftFaceTriangle2V0.position = upperV0;
			leftFaceTriangle2V0.normal = leftNormal;

			Vertex leftFaceTriangle2V1;
			leftFaceTriangle2V1.position = lowerV0;
			leftFaceTriangle2V1.normal = leftNormal;

			Vertex leftFaceTriangle2V2;
			leftFaceTriangle2V2.position = lowerV2;
			leftFaceTriangle2V2.normal = leftNormal;

			collisionCube->Vertices.push_back(leftFaceTriangle1V0);
			collisionCube->Vertices.push_back(leftFaceTriangle1V1);
			collisionCube->Vertices.push_back(leftFaceTriangle1V2);

			collisionCube->Vertices.push_back(leftFaceTriangle2V0);
			collisionCube->Vertices.push_back(leftFaceTriangle2V1);
			collisionCube->Vertices.push_back(leftFaceTriangle2V2);
		}

		
		//set indices
		for (int i = 0; i < collisionCube->Vertices.size(); i++)
		{
			collisionCube->Indices.push_back(i);
		}

		collisionCube->SetupMesh();
		meshes.push_back(collisionCube);
	}
	void PhysicsComponent::ConstructCollisionCube()
	{
		//is empty
		CollisionCubeParams params;
		ConstructCollisionCube(params);
	}
}

