#include "Canvas.h"
#include "TextureMaterial.h"
namespace PXG
{

	Canvas::Canvas() {}

	GameObj Canvas::createCanvasObject(Vector2 position, Vector2 dimensions, std::string name, std::string textureName)
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		newGameObject->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "plane.obj");
		//child->GetMeshComponent()->AddTextureToMeshAt(Texture{ config::PXG_INDEPENDENT_TEXTURES_PATH + tile["texture"].get<std::string>(),TextureType::DIFFUSE }, 0);

		newGameObject->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());
		newGameObject->GetMeshComponent()->AddTextureToMeshAt(Texture
			{ config::PXG_INDEPENDENT_TEXTURES_PATH + textureName, TextureType::DIFFUSE }, 0);
		
		newGameObject->GetTransform()->Scale(Vector3(dimensions.x, 0, dimensions.y));
		newGameObject->GetTransform()->SetLocalPosition(Vector3(position.x, position.y, -100));
		newGameObject->GetTransform()->rotate(glm::vec3(1, 0, 0), 90);
		this->AddToChildren(newGameObject);
		return newGameObject;
	}

	GameObj Canvas::createEmptyCanvasObject()
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		this->AddToChildren(newGameObject);
		return newGameObject;
	}
}

