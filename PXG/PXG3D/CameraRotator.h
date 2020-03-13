#pragma once
#include "Component.h"

namespace PXG
{
	class CameraRotator : public Component
	{
	public:

		virtual void Start() override;
		virtual void FixedUpdate(float tick) override;
		virtual void SetRotateUsed(bool newRotateUsedState);

	private:

		bool rotateUsed = false;

		float oldX = 0;
		float oldY = 0;

		float speedMultiplier = 5.0f;


	};

}


