#pragma once
#include "Component.h"

namespace PXG
{
	class CameraMover :public Component
	{
	public:

		CameraMover(float speed = 0.001f);

		void Update() override;

		void FixedUpdate(float tick) override;

		void SetSpeed(float newSpeed);

	private:

		float speed;

	};
}


