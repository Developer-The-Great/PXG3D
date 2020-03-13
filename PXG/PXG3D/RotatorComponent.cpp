#include "RotatorComponent.h"
#include "Transform.h"
namespace PXG
{

	RotatorComponent::RotatorComponent(Vector3 axis, float speed):Component()
	{
		this->speed = speed;
		this->RotationAxis = axis;


	}
	void RotatorComponent::FixedUpdate(float tick)
	{
		auto transform = GetOwnerTransform();

		if (transform)
		{
			transform->rotate(RotationAxis, speed);

		}

	}
}

