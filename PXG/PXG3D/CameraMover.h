#pragma once
#include "Component.h"

namespace PXG
{
	class CameraMover :public Component
	{
	public:

		CameraMover(float speed = 3.0f);

		void FixedUpdate(float tick) override;

		void SetSpeed(float newSpeed);

	private:

		float speed;

	};
}


