#include "CameraMover.h"
#include "GameObject.h"
#include "Input.h"
#include "KeyCode.h"

namespace PXG
{
	CameraMover::CameraMover(float speed)
	{
		SetSpeed(speed);
	}
	void CameraMover::Update()
	{
		auto transform = GetOwner()->GetTransform();

		//Debug::Log("FixedUpdate(float tick)");
		if (transform)
		{

			if (Input::GetKey(KeyCode::W))
			{
				Debug::Log("speed {0} ",speed);
				Vector3 forward = transform->GetForward();
				transform->translate(forward * speed );
			}

			if (Input::GetKey(KeyCode::A))
			{
				Vector3 left = transform->GetRight() * -1;
				transform->translate(left * speed );
			}

			if (Input::GetKey(KeyCode::S))
			{
				Vector3 backward = transform->GetForward() * -1;
				transform->translate(backward * speed );
			}

			if (Input::GetKey(KeyCode::D))
			{
				Vector3 right = transform->GetRight();
				transform->translate(right * speed );
			}


			//if(Input::GetKey(KeyCode::scr))
		/*	if (Input::GetMouseWheelScroll() > 0.1f)
			{
				speed += 1.0f;
			}

			if (Input::GetMouseWheelScroll() < -0.1f)
			{
				speed += -1.0f;
			}*/




		}
	}
	void CameraMover::FixedUpdate(float tick)
	{
		

	}
	void CameraMover::SetSpeed(float newSpeed)
	{
		speed = newSpeed;
	}
}
