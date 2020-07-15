#pragma once
#include "Component.h"
#include "Vector3.h"

namespace PXG
{
	class Transform;


	class Rigidbody : public Component
	{
	public:

		Rigidbody();

		void AddForce(Vector3 force);

		void AddForceAt(Vector3 force, Vector3 worldPosition);
		
		void AddImpulse(Vector3 velocity);

		void Integrate(float dt);

		void SetInertia(glm::mat3 inertiaTensor);

		void SetMass(float newMass);

		Vector3 GetVelocity() const;

		Vector3 GetAcceleration() const;

	private:

		bool isAsleep = false;

		float inverseMass;


		Vector3 velocity;
		Vector3 acceleration;

		Vector3 angularAcceleration;
		Vector3 angularVelocity;

		Vector3 forceAccumulator;
		Vector3 torqueAccumulator;

		void resetAccumulators();

		void semiImplicitEulerIntegration(Transform * transform,float dt);
	
		//TODO please create an actual wrapper for mat3 at some point...
		glm::mat3 inverseInertiaTensor = glm::inverse(glm::mat3(1.0f/12.0f));

	};

}

