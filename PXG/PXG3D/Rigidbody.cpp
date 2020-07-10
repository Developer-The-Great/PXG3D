#include "Rigidbody.h"
#include "GameObject.h"


namespace PXG
{
	void Rigidbody::AddForce(Vector3 force)
	{
	}
	void Rigidbody::AddForceAt(Vector3 force, Vector3 worldPosition)
	{
	}
	void Rigidbody::AddImpulse(Vector3 velocity)
	{
	}
	void PXG::Rigidbody::Integrate(float dt)
	{
		if (isAsleep || Mathf::FloatCompare(0.0f,inverseMass)) { return; }

		auto transform = GetOwner()->GetTransform();

		semiImplicitEulerIntegration(transform, dt);

	}

	void Rigidbody::SetMass(float newMass)
	{
		if (Mathf::FloatCompare(newMass, 0.0f))
		{
			inverseMass = 0.0f;
		}


	}

	Vector3 Rigidbody::GetVelocity() const
	{
		return velocity;
	}

	Vector3 Rigidbody::GetAcceleration() const
	{
		return acceleration;
	}


	void PXG::Rigidbody::semiImplicitEulerIntegration(Transform * transform, float dt)
	{
		//add force accumulation to acceleration
		Vector3 newAcceleration = forceAccumulator * inverseMass;

		//TODO make an operator overload for += 
		acceleration = acceleration + newAcceleration;

		velocity = velocity + acceleration;

		transform->translate(velocity);

		//add torque accumulation to angular acceleration



		//add angular acceleration to angular velocity

		//add angular velocity to acceleration 

		//reset accumulators

	}

}


