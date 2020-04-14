#pragma once
#include "GameObject.h"
#include <list>

namespace PXG
{
	class CameraComponent;
	class DebugDrawingManager;
	class LightComponent;
	class Time;

	class World : public GameObject
	{

	public:

		World();

		void SetCamera(std::shared_ptr<CameraComponent> cameraComponent);
		std::shared_ptr<CameraComponent> GetCamera() const;

		void AddLight(std::shared_ptr<LightComponent> light);
		std::list<std::shared_ptr<LightComponent>>  GetLights() const;

		int GetLightCount() const;

		std::shared_ptr<DebugDrawingManager> GetDebugDrawingManager();

		void SetDebugDrawingManager(std::shared_ptr<DebugDrawingManager> debugDrawingManager);

		static bool IsDrawPhysicsComponentMeshNeeded();

		//TODO [LOW PRIORITY] also make a better name for thi

		static void SetPhysicsComponentDrawActive(bool newState);

		std::shared_ptr<Time> GetTimeSystem();
		void SetTimeSystem(std::shared_ptr<Time> newTimeSystem);

	protected:

	private:

		std::shared_ptr<DebugDrawingManager> debugDrawingManager;

		//TODO [LOW PRIORITY] make a better name for this
		static bool needToDrawPhysicsComponentMesh;

		std::shared_ptr<CameraComponent> cameraComponent;

		std::list<std::shared_ptr<LightComponent>> lights;

		std::shared_ptr<Time> time;


	};
}


