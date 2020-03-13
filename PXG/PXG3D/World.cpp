#include "World.h"
#include "CameraComponent.h"

namespace PXG
{
	bool World::needToDrawPhysicsComponentMesh = true;

	World::World():GameObject()
	{

	}
	void World::SetCamera(std::shared_ptr<CameraComponent>  cameraComponent)
	{
		this->cameraComponent = cameraComponent;
	}

	std::shared_ptr<CameraComponent> World::GetCamera() const
	{

		return cameraComponent;
	}
	void World::AddLight(std::shared_ptr<LightComponent> light)
	{
		lights.emplace_front(light);
	}
	std::list<std::shared_ptr<LightComponent>> World::GetLights() const
	{
		return lights;
	}
	int World::GetLightCount() const
	{
		return lights.size();
	}
	bool World::IsDrawPhysicsComponentMeshNeeded()
	{
		return needToDrawPhysicsComponentMesh;
	}
	void World::SetDrawPhysicsComponentMeshDraw(bool newPhysicsComponentDrawState)
	{
		needToDrawPhysicsComponentMesh = newPhysicsComponentDrawState;
	}
	void World::SetPhysicsComponentDrawActive(bool newState)
	{
		needToDrawPhysicsComponentMesh = newState;
	}
}

