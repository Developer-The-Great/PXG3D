#include "Rigidbody.h"
#include "GameObject.h"


namespace PXG
{

	Rigidbody::Rigidbody()
	{
		inverseMass = 1.0f;
	}

	void Rigidbody::AddForce(Vector3 force)
	{
		forceAccumulator = forceAccumulator + force;
		Debug::Log(" forceAccumulator {0} " + forceAccumulator.ToString());
		Debug::Log("");
	}

	void Rigidbody::AddForceAt(Vector3 force, Vector3 worldPosition)
	{
		auto transform = GetOwner()->GetTransform();

		Vector3 torque = Mathf::Cross(worldPosition - transform->GetPosition(), force);

		forceAccumulator = forceAccumulator + force;
		torqueAccumulator = torqueAccumulator + torque;

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

	void Rigidbody::SetInertia(glm::mat3 inertiaTensor)
	{
	}

	void Rigidbody::SetMass(float newMass)
	{
		if (Mathf::FloatCompare(newMass, 0.0f))
		{
			inverseMass = 0.0f;
		}
		else
		{
			inverseMass = 1.0f / newMass;
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
		Vector3 newAcceleration = forceAccumulator * inverseMass;
		
		acceleration =  newAcceleration;
		
		velocity = velocity + (newAcceleration + PhysicsEngine::GetGravity()) * dt;

		transform->translate(velocity * dt);

		//add torque accumulation to angular acceleration
		Vector3 newAngularAcceleration = inverseInertiaTensor * torqueAccumulator.ToGLMVec3();


		angularAcceleration =  newAngularAcceleration;

		//Debug::Log("(inverseInertiaTensor {0} ", (glm::to_string(inverseInertiaTensor)));

		angularVelocity = angularVelocity + (newAngularAcceleration ) * dt;

		angularVelocity = angularVelocity * (0.999f);

		Quaternion quat;


		if (!angularVelocity.IsZeroVector())
		{
			float angle = angularVelocity.Length();

			Vector3 axis = angularVelocity / angle * Mathf::Sin(0.5f * angle * dt);

			quat.w = Mathf::Cos(0.5 * angle * dt);
			quat.x = axis.x;
			quat.y = axis.y;
			quat.z = axis.z;

			if (Mathf::FloatCompare(quat.Length(), 0.0))
			{
				quat = Quaternion();
			}
			
			transform->rotate(( quat ));

		}
		

		resetAccumulators();

	}

}


