#pragma once
#include "Component.h"
#include "Vector3.h"

namespace PXG
{


	class RotatorComponent : public Component
	{
	public:

		RotatorComponent(Vector3 axis,float speed = 1.0f);

		virtual void FixedUpdate(float tick) override;

	private:

		Vector3 RotationAxis;
		float speed;

	};
}


