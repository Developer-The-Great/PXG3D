#include "Rigidbody.h"
#include "GameObject.h"


namespace PXG
{
	Rigidbody::Rigidbody()
	{
		inverseMass = 0.1f;
	}
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

		Debug::Log("isAsleep {0}  Mathf::FloatCompare(0.0f,inverseMass) {1} ", isAsleep, Mathf::FloatCompare(0.0f, inverseMass));

		auto transform = GetOwner()->GetTransform();

		semiImplicitEulerIntegration(transform, dt);
	}

	void Rigidbody::SetInertia(glm::mat3 inertiaTensor)
	{
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


	void Rigidbody::resetAccumulators()
	{
		forceAccumulator = Vector3();
		torqueAccumulator = Vector3();
	}

	void PXG::Rigidbody::semiImplicitEulerIntegration(Transform * transform, float dt)
	{
		//add force accumulation to acceleration
		forceAccumulator = forceAccumulator + Vector3(0, -9, 0);

		Vector3 newAcceleration = forceAccumulator * inverseMass;

		acceleration =  newAcceleration;

		velocity = velocity + (newAcceleration + Vector3(0, -9, 0)) * dt;

		Debug::Log("velocity {0} ", (velocity * dt).ToString());

		transform->translate(velocity * dt);

		//add torque accumulation to angular acceleration
		Vector3 newAngularAcceleration = inverseInertiaTensor * torqueAccumulator.ToGLMVec3();

		angularAcceleration =  newAngularAcceleration;

		angularVelocity = angularVelocity + newAngularAcceleration * dt;


		Quaternion quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);
		transform->rotate(quat * 0.5f * dt);

		resetAccumulators();

	}

}


