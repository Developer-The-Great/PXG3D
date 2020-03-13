#include "CameraRotator.h"
#include "Input.h"
#include "GameObject.h"
#include "KeyCode.h"

void PXG::CameraRotator::Start()
{
	oldX = Input::GetMouseX();
	oldY = Input::GetMouseY();
}

void PXG::CameraRotator::FixedUpdate(float tick)
{

	if (Input::GetKeyDown(KeyCode::MiddleMouse))
	{
		rotateUsed = !rotateUsed;
	}
	
	float currentX = Input::GetMouseX();
	float currentY = Input::GetMouseY();

	if (rotateUsed)
	{
		float diffX = currentX - oldX;
		float diffY = currentY - oldY;

		if (Mathf::Abs(diffX))
		{
			GetOwner()->GetTransform()->rotate(Vector3(0, -1, 0), diffX * speedMultiplier * tick);
		}

		if (Mathf::Abs(diffY))
		{
			GetOwner()->GetTransform()->rotate(-GetOwner()->GetTransform()->GetRight(), diffY * speedMultiplier  * tick);
		}
	}

	oldX = Input::GetMouseX();
	oldY = Input::GetMouseY();


}

void PXG::CameraRotator::SetRotateUsed(bool newRotateUsedState)
{
	rotateUsed = newRotateUsedState;
}
