#pragma once
#include "GameObject.h"
#include <list>

namespace PXG
{
	class CameraComponent;
	class LightComponent;

	class World : public GameObject
	{

	public:

		World();

		void SetCamera(std::shared_ptr<CameraComponent> cameraComponent);
		std::shared_ptr<CameraComponent> GetCamera() const;

		void AddLight(std::shared_ptr<LightComponent> light);
		std::list<std::shared_ptr<LightComponent>>  GetLights() const;

		int GetLightCount() const;

		static bool IsDrawPhysicsComponentMeshNeeded();
		//TODO [LOW PRIORITY] also make a better name for this
		static void SetDrawPhysicsComponentMeshDraw(bool newPhysicsComponentDrawState);

		static void SetPhysicsComponentDrawActive(bool newState);

	protected:

	private:
		//TODO [LOW PRIORITY] make a better name for this
		static bool needToDrawPhysicsComponentMesh;

		std::shared_ptr<CameraComponent> cameraComponent;

		std::list<std::shared_ptr<LightComponent>> lights;

	};
}


