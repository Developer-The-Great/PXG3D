#include "Component.h"
#include "GameObject.h"

namespace PXG
{
	void Component::SetOwner(std::shared_ptr<GameObject> owner)
	{
		this->owner = owner;
	}



	std::shared_ptr<GameObject> Component::GetOwner() const
	{

		if (owner.expired()) { return nullptr; }
		return owner.lock();
	}

	Transform * Component::GetOwnerTransform()
	{
		auto ownerPtr = GetOwner();

		if (ownerPtr)
		{
			return ownerPtr->GetTransform();
		}
		return nullptr;
	}

}
