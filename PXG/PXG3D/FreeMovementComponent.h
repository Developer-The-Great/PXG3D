#pragma once
#include "Component.h"

namespace PXG
{
	class FreeMovementComponent :public Component
	{
	public:

		virtual void FixedUpdate(float tick) override;
	};

}

