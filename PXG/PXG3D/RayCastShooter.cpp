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

		if (!Input::GetKeyUp(KeyCode::LeftMouse)) return;

		auto transform = GetOwnerTransform();


		Vector3 forward = transform->GetForward();

		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		Vector3 PixelPosition = Utils::GetProjectionCameraScreenToWorld(x,y,GetOwner()->GetWorld().lock());
		Vector3 ObjectPosition = transform->GetPosition();

		HitInfo hit;

		PhysicsEngine::Raycast(ObjectPosition, (PixelPosition - ObjectPosition).Normalized(), hit, GetOwner()->GetWorld().lock(), true);
		if(hit.RayHit)
		{
			Debug::Log("Hit");

			auto world = GetOwner()->GetWorld().lock();

			if (world)
			{
				if (auto debugDrawer = world->GetDebugDrawingManager())
				{
					Debug::Log("hit at {0}", hit.Position.ToString());
					debugDrawer->InstantiateLine(ObjectPosition,hit.Position, Vector3(1, 0, 0), 5.0f);
					debugDrawer->InstantiateCube(hit.Position, Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0, 1, 0), 5.0f);
				}
			}

			//transform->SetLocalPosition(hit.Position);
		}
		else
		{
			Debug::Log("Miss");
		}

		
		


	}
}

