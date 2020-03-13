#include "RayCastShooter.h"
#include "PhysicsEngine.h"
#include "Debug.h"
#include "Transform.h"
#include "GameObject.h"
#include "HitInfo.h"
#include "Input.h"
#include "CameraComponent.h"
#include "KeyCode.h"

#include "ScreenSize.h"

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

		HitInfo info;


		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		Vector3 position = PhysicsEngine::GetOrthographicCameraWorldPosition(x, y, ScreenSize::WIDTH, ScreenSize::HEIGHT, GetOwner());
		if (isActive)
		{
			PhysicsEngine::Raycast(position, forward, info, GetOwner()->GetWorld().lock());

			if (info.RayHit)
			{
				Debug::Log("raycast hit {0} ", info.GameObjectHit->name);
				Debug::Log("normal found ", info.Normal.ToString());
				lastHit = info;
				notify(ON_RAYCAST_HIT);

				

			}
			else {
				Debug::Log("oof ");
			}
		}
	}
}

