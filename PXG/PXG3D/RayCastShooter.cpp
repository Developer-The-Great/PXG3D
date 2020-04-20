#include "RayCastShooter.h"
#include "PhysicsEngine.h"
#include "Debug.h"
#include "Transform.h"
#include "GameObject.h"
#include "HitInfo.h"
#include "Input.h"
#include "CameraComponent.h"
#include "KeyCode.h"
#include "HitInfo.h"
#include "ScreenSize.h"
#include "World.h"
#include "DebugDrawingManager.h"

namespace PXG
{
	void RayCastShooter::Start()
	{
	}

	


	void RayCastShooter::FixedUpdate(float tick)
	{
	}

	void RayCastShooter::Update()
	{

		if (possessedGameObject)
		{
			if (Input::GetKey(KeyCode::KeyLeft))
			{
				possessedGameObject->GetTransform()->translate(Vector3(-1, 0, 0) * speed);
			}

			if (Input::GetKey(KeyCode::KeyRight))
			{
				possessedGameObject->GetTransform()->translate(Vector3(1, 0, 0) * speed);
			}

			if (Input::GetKey(KeyCode::KeyUp))
			{
				possessedGameObject->GetTransform()->translate(Vector3(0, 0, -1) * speed);
			}

			if (Input::GetKey(KeyCode::KeyDown))
			{
				possessedGameObject->GetTransform()->translate(Vector3(0, 0, 1) * speed);
			}

			if (Input::GetKey(KeyCode::KP2))
			{
				possessedGameObject->GetTransform()->translate(Vector3(0, 1, 0) * speed);
			}

			if (Input::GetKey(KeyCode::KP0))
			{
				possessedGameObject->GetTransform()->translate(Vector3(0, -1, 0) * speed);
			}

			if (Input::GetKey(KeyCode::X))
			{
				possessedGameObject->GetTransform()->rotate(Vector3(1, 0, 0), 1.0f);
			}

			if (Input::GetKey(KeyCode::Y))
			{
				possessedGameObject->GetTransform()->rotate(Vector3(0, 1, 0), 1.0f);
			}

			if (Input::GetKey(KeyCode::Z))
			{
				possessedGameObject->GetTransform()->rotate(Vector3(0, 0, 1), 1.0f);
			}
			

		}



		if (!Input::GetKeyUp(KeyCode::LeftMouse)) return;

		auto transform = GetOwnerTransform();

		Vector3 forward = transform->GetForward();

		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		Vector3 PixelPosition = Utils::GetProjectionCameraScreenToWorld(x, y, GetOwner()->GetWorld().lock());
		Vector3 ObjectPosition = transform->GetPosition();

		HitInfo hit;
		Debug::Log("---------------------------raycast start----------------");
		PhysicsEngine::Raycast(ObjectPosition, (PixelPosition - ObjectPosition).Normalized(), hit, GetOwner()->GetWorld().lock(), true);
		
		if (hit.RayHit)
		{
			

			auto world = GetOwner()->GetWorld().lock();

			if (world)
			{
				if (auto debugDrawer = world->GetDebugDrawingManager())
				{
					Debug::Log("hit at {0}", hit.Position.ToString());
					Debug::Log("hit normal {0}", hit.Normal.ToString());
					debugDrawer->InstantiateLine(ObjectPosition, hit.Position, Vector3(1, 0, 0), 2.0f);
					debugDrawer->InstantiateCube(hit.Position, Vector3(0, 0, 0), Vector3(0.1, 0.1, 0.1), Vector3(0, 1, 0), 2.0f);

					Debug::Log("Now possessing GameObject with name: {0}", hit.GameObjectHit->name);
					possessedGameObject = hit.GameObjectHit;

					

				}
			}


		}
		else
		{
			Debug::Log("Miss");
		}
		Debug::Log("---------------raycast end--------------------");
		//----------------------------- Control Possesssed GameObject------------------------------------//

		





	}

}

