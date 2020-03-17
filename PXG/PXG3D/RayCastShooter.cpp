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
		Vector3 ObjectPosition = transform->GetLocalPosition();

		HitInfo hit;

		PhysicsEngine::Raycast(ObjectPosition, (PixelPosition - ObjectPosition).Normalized(), hit, GetOwner()->GetWorld().lock(), true);
		if(hit.RayHit)
		{
			Debug::Log("Hit");

			//transform->SetLocalPosition(hit.Position);
		}
		else
		{
			Debug::Log("Miss");
		}

		
		


	}
}

