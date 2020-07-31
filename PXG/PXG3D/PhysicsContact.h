#pragma once
#include "Vector3.h"
#include "Rigidbody.h";
#include "PhysicsEngine.h"

namespace PXG
{
	struct PhysicsContact
	{
		Vector3 worldContactPositionA;
		Vector3 worldContactPositionB;

		Vector3 colliderAWorldPosition;
		Vector3 colliderBWorldPosition;

		Vector3 collisionNormal;

		float impulseSum;

		float penetrationDepth;

		float inverseEffectiveMass;
		float baumgartePositionalConstraint;


		std::shared_ptr<Rigidbody> rigidbodyA = nullptr;
		std::shared_ptr<Rigidbody> rigidbodyB = nullptr;


		void PreCalculateEffectiveMass()
		{
			//if both rigidbody pointers are empty there's no point to doing this calculation
			if (IsBothRigidbodyPtrEmpty()) { return; }

			Vector3 ra = worldContactPositionA - colliderAWorldPosition;
			Vector3 rb = worldContactPositionB - colliderBWorldPosition;

			Vector3 wa = rigidbodyA ? rigidbodyA->GetAngularVelocity() : Vector3();
			Vector3 wb = rigidbodyB ? rigidbodyB->GetAngularVelocity() : Vector3();

			//(-ra x wa)
			glm::vec3 raInverseCrossNormal =  Mathf::Cross(-ra, collisionNormal).ToGLMVec3();
			//(rb x wb)
			glm::vec3 rbCrossNormal =  Mathf::Cross(rb, collisionNormal).ToGLMVec3();

			//---------------- Calculate M^-1 * J^T, denoted as effective mass -------------------------------//

			glm::vec3 x, y, z, w;

			//calculate (MA)^-1  * -n , denoted as x 
			glm::mat3 MaInverse = rigidbodyA ? glm::mat3(rigidbodyA->GetInverseMass()) : glm::mat3(1.0);
			x = MaInverse * -collisionNormal.ToGLMVec3();

			//calculate (IA)^-1 * (-ra x wa) , denoted as y 
			glm::mat3 IaInverse = rigidbodyA ? rigidbodyA->GetInverseInertiaTensor() : glm::inverse(glm::mat3(1.0f / 12.0f));
			y = IaInverse * raInverseCrossNormal;

			//calculate (MB)^-1 * n , denoted as z 
			glm::mat3 MbInverse = rigidbodyB ? glm::mat3(rigidbodyB->GetInverseMass()) : glm::mat3(1.0);
			z = MbInverse * collisionNormal.ToGLMVec3();

			//calculate (IB)^-1 * (rb x wb) , denoted as w 
			glm::mat3 IbInverse = rigidbodyB ? rigidbodyB->GetInverseInertiaTensor() : glm::inverse(glm::mat3(1.0f / 12.0f));
			w = IbInverse * rbCrossNormal;
			
			//calculate -n . x 
			float inverseNormalDotX = Mathf::Dot(-collisionNormal, x);

			//calculate (-ra x wa) . y
			float raInverseCrossWaDotY = Mathf::Dot(raInverseCrossNormal, y);

			//calculate n . z 
			float NormalDotZ = Mathf::Dot(collisionNormal, z);

			//calculate (rb x wb) . w
			float rbCrossWbDotW = Mathf::Dot(rbCrossNormal, w);

			//calculate inverse effective mass by dividing 1 by the sum up result of dot products 
			inverseEffectiveMass = 1.0f /(inverseNormalDotX + raInverseCrossWaDotY + NormalDotZ + rbCrossWbDotW);

			baumgartePositionalConstraint = -PhysicsEngine::GetBaumgarteCoefficient() * penetrationDepth;

		}

		void ResolveCollision(float dt)
		{
			//if both rigidbody pointers are empty there's no point to doing this calculation
			if (IsBothRigidbodyPtrEmpty()) { return; }

			Vector3 ra = worldContactPositionA - colliderAWorldPosition;
			Vector3 rb = worldContactPositionB - colliderBWorldPosition;

			Vector3 va = rigidbodyA ? rigidbodyA->GetVelocity() : Vector3();
			Vector3 vb = rigidbodyB ? rigidbodyB->GetVelocity() : Vector3();

			Vector3 wa = rigidbodyA ? rigidbodyA->GetAngularVelocity() : Vector3();
			Vector3 wb = rigidbodyB ? rigidbodyB->GetAngularVelocity() : Vector3();




			//(-ra x wa)
			glm::vec3 raInverseCrossNormal = Mathf::Cross(-ra, collisionNormal).ToGLMVec3() ;
			//(rb x wb)
			glm::vec3 rbCrossNormal =  Mathf::Cross(rb, collisionNormal).ToGLMVec3() ;


			//calculate - J.V
				//calculate -n . va
				float JVx = Mathf::Dot(-collisionNormal, va);

				//calculate (-ra x wa) . wa
				float JVy = Mathf::Dot(raInverseCrossNormal, wa);

				//calculate n * vb
				float JVz = Mathf::Dot(collisionNormal, vb);

				//calculate (rb x wb) . wb
				float JVw = Mathf::Dot(rbCrossNormal, wb);

			float inverseJV = -(JVx + JVy + JVz + JVw) + baumgartePositionalConstraint/dt;

			Debug::Log("----------inverse jv calculation-----------------");
			Debug::Log("JVx {0} , -collisionNormal {1}  dot va {2}",JVx, (-collisionNormal).ToString(),va.ToString());
			Debug::Log("JVy {0} ra {1} wa {2}", JVy,ra.ToString(),wa.ToString());
			Debug::Log("JVz {0} ", JVz);
			Debug::Log("JVw {0} ", JVw);

			Debug::Log("baumgartePositionalConstraint/dt {0} ", baumgartePositionalConstraint / dt);
			Debug::Log("inverseJV {0} ", inverseJV);

			// calculate - J.V * inverseEffectiveMass,denoted as lambda
			float lambda = inverseJV * inverseEffectiveMass ;

			//make sure total impulse sum does not 

			float oldTotalLambda = impulseSum;

			impulseSum += lambda;
			if (impulseSum < 0.0f)
			{
				impulseSum = 0.0f;
			}

			Debug::Log("raw lambda {0} ", lambda);

			lambda = impulseSum - oldTotalLambda;

			
			Debug::Log("impulseSum {0} ", impulseSum);
			Debug::Log("actual lambda applied {0} ", lambda);

			//apply linear impulse 
			Vector3 linearImpulse = collisionNormal * lambda;

			Vector3 torqueDirectionA = Mathf::Cross(-ra, collisionNormal);
			Vector3 torqueDirectionB = Mathf::Cross(rb, collisionNormal);

			Vector3 angularImpulseA = torqueDirectionA * lambda;
			Vector3 angularImpulseB = torqueDirectionB * lambda;

			/*Debug::Log("-JV {0} ", -(JVx + JVy + JVz + JVw));
			Debug::Log("baumgarte {0}", baumgartePositionalConstraint / dt);*/

			if (rigidbodyA)
			{
				

				//Debug::Log("add vel {0} to bodyA ", (-linearImpulse * rigidbodyA->GetInverseMass()).ToString());
				rigidbodyA->AddVelocity(-linearImpulse * rigidbodyA->GetInverseMass());
				Vector3 addedAngularVelocity = rigidbodyA->GetInverseInertiaTensor() * angularImpulseA.ToGLMVec3();
				rigidbodyA->AddAngularVelocity(addedAngularVelocity);

				/*Debug::Log("ra length: {0} rb length: {1}  ", ra.Length(),rb.Length());
				Debug::Log("add angvel {0} to bodyA ", addedAngularVelocity.ToString());*/
			}

			/*Debug::Log("inverseEffectiveMass {0} ", (inverseEffectiveMass));
			Debug::Log("lambda {0} ", (lambda));*/
			
			if (rigidbodyB)
			{
				/*Debug::Log("bodyB mass {0} ", (rigidbodyB->GetInverseMass()));
				Debug::Log("add vel {0} to bodyB ", (linearImpulse * rigidbodyB->GetInverseMass()).ToString());*/
				rigidbodyB->AddVelocity(linearImpulse * rigidbodyB->GetInverseMass());
				Vector3 addedAngularVelocity = rigidbodyB->GetInverseInertiaTensor() * angularImpulseB.ToGLMVec3();
				rigidbodyB->AddAngularVelocity(addedAngularVelocity);

				//Debug::Log("add angvel {0} to bodyB ", addedAngularVelocity.ToString());
			}

		}

		bool IsBothRigidbodyPtrEmpty()
		{
			return !(rigidbodyA || rigidbodyB);
		}

	};

}
