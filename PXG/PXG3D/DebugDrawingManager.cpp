#include "DebugDrawingManager.h"

#include "Mesh.h"
#include "AbstractMaterial.h"
#include "ColorMaterial.h"
#include "DebugMeshObject.h"
#include "World.h"
#include "CameraComponent.h"
#include "Mesh.h"
#include "RasterizationMode.h"
namespace PXG
{
	void DebugDrawingManager::InstantiateLine(Vector3 start, Vector3 end, Vector3 color, float lifetime)
	{

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> emptyTextures;

		Vertex startVertex;
		startVertex.position = start;
		startVertex.normal = start;
		startVertex.UV = Vector2(0, 0).ToGLMVec2();

		vertices.push_back(startVertex);

		Vertex endVertex;
		endVertex.position = end;
		endVertex.normal = end;
		startVertex.UV = Vector2(0, 0).ToGLMVec2();

		vertices.push_back(endVertex);

		indices.push_back(0);
		indices.push_back(1);


		std::shared_ptr<Mesh> lineMesh = std::make_shared<Mesh>(indices, vertices, emptyTextures);
		std::shared_ptr<AbstractMaterial> abstractMaterial = std::make_shared<ColorMaterial>(color);


		debugMeshObjects.push_back(std::make_shared<DebugMeshObject>(lineMesh, abstractMaterial, Mat4(), lifetime, PrimitiveDrawingMode::LINE));

	}



	void DebugDrawingManager::InstantiateCube(Vector3 position, Vector3 min, Vector3 max, Vector3 color, float lifetime, Vector3 vertexOffset)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> emptyTextures;

		Vector3 xAdd = Vector3(max.x - min.x, 0, 0);
			
		Vector3 yAdd = Vector3(0, max.y - min.y, 0);
			
		Vector3 zAdd = Vector3(0, 0, max.z - min.z);
			

		Vector3 lowerV0 = min + vertexOffset;
		Vector3 lowerV1 = lowerV0 + xAdd;
		Vector3 lowerV2 = lowerV0 + zAdd;
		Vector3 lowerV3 = lowerV0 + xAdd + zAdd;

		Vector3 upperV0 = min + vertexOffset + yAdd;
		Vector3 upperV1 = upperV0 + xAdd;
		Vector3 upperV2 = upperV0 + zAdd;
		Vector3 upperV3 = upperV0 + xAdd + zAdd;



		std::shared_ptr<Mesh> cube =  Mesh::InstantiateCubeMesh(lowerV0, lowerV1, lowerV2, lowerV3, upperV0, upperV1, upperV2, upperV3);
		std::shared_ptr<AbstractMaterial> abstractMaterial = std::make_shared<ColorMaterial>(color);
		
		glm::mat4 mat4Position(
			glm::vec4(1.0f, 0, 0, 0),
			glm::vec4(0, 1.0f, 0, 0),
			glm::vec4(0, 0, 1.0f, 0),
			glm::vec4(position.ToGLMVec3(), 1.0f)
		);
		
		debugMeshObjects.push_back(std::make_shared<DebugMeshObject>(cube, abstractMaterial, mat4Position, lifetime,PrimitiveDrawingMode::TRIANGLE));

	}

	void DebugDrawingManager::DrawDebugObjects()
	{
		Mesh::SetRasterizationMode(RasterizationMode::LINE);
		

		if (auto worldPtr = world.lock())
		{
			auto camera = worldPtr->GetCamera();
			if (camera)
			{
				for (auto const& debugObject : debugMeshObjects)
				{
					Mesh::SetPrimitiveDrawingMode(debugObject->GetDrawMode());
					debugObject->Draw(world, Mat4(), camera->GetView(), camera->GetProjection());
				}
			}
			
		}

		
	}

	void DebugDrawingManager::DecreaseLifespan(float timepast)
	{
		for (auto const& debugObject : debugMeshObjects)
		{
			debugObject->DecreaseLifeSpan(timepast);
		}

	}

	void DebugDrawingManager::RemoveDeadDebugMeshes()
	{
		std::vector<std::shared_ptr<DebugMeshObject>> deadDebugMeshes;

		for (auto const& debugObject : debugMeshObjects)
		{
			if (debugObject->IsLifeSpanOver())
			{
				deadDebugMeshes.push_back(debugObject);
			}
		}

		for (auto const& debugObject : deadDebugMeshes)
		{
			debugMeshObjects.remove(debugObject);


		}




	}

	

	void DebugDrawingManager::SetWorld(std::weak_ptr<World> world)
	{
		this->world = world;
	}
	std::weak_ptr<World> DebugDrawingManager::GetWorld()
	{
		return world;
	}
}

