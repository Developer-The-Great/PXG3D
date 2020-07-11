#include "ForceAdderTestComponent.h"

#include "GameObject.h"
#include "Rigidbody.h"
#include "Input.h"
#include "KeyCode.h"

namespace PXG
{
	ForceAdderTestComponent::ForceAdderTestComponent(Vector3 forceDirection = Vector3(0,5,0))
	{
		this->forceDirection = forceDirection;
	}

	void ForceAdderTestComponent::FixedUpdate(float tick)
	{




	}

	void ForceAdderTestComponent::Update()
	{
		//Debug::Log("!rb {0} " + !rb);
		if (!rb) { return; }

		if (Input::GetKeyDown(KeyCode::F))
		{
			Debug::Log("Add force {0} " + forceDirection.ToString());

			rb->AddForce(forceDirection);
		}

		if (Input::GetKeyDown(KeyCode::T))
		{
			Debug::Log("Add force at {0} ");
			
			rb->AddForceAt(forceDirection, owner.lock()->GetTransform()->GetPosition() + Vector3(0.5, 0, 0));
		}



	}

	void ForceAdderTestComponent::SetOwner(std::shared_ptr<GameObject> owner)
	{
		Component::SetOwner(owner);

		if (owner)
		{
			rb = owner->GetComponent<Rigidbody>();
		}
		else
		{
			Debug::Log(Verbosity::Error, "owner is null!");
		}

	}

}
