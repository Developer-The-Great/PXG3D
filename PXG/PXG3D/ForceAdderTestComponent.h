#pragma once
#include "Component.h"
#include "Vector3.h"
#include <memory>

namespace PXG
{
	class Rigidbody;

	class ForceAdderTestComponent :public Component
	{
	public:

		ForceAdderTestComponent(Vector3 forceDirection);


		void FixedUpdate(float tick) override;

		void Update() override;

		void SetOwner(std::shared_ptr<GameObject> owner) override;

	private:

		
		std::shared_ptr<Rigidbody> rb;


		Vector3 forceDirection;
		Vector3 torquePoint;
		



	};
}


