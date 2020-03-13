#pragma once
#include <memory>


namespace PXG
{

	class GameObject;
	class Transform;

	class Component
	{
	public:

		virtual void Start() {}

		virtual void FixedUpdate(float tick) {}

		Component() = default;

		virtual ~Component() = default;

		virtual void SetOwner(std::shared_ptr<GameObject> owner);

		Transform* GetOwnerTransform();

		std::shared_ptr<GameObject> GetOwner() const;
		void SetActive(bool condition) { isActive = condition; };
		bool IsActive() { return isActive; };
	protected:

		std::weak_ptr<GameObject> owner;
		bool isActive = true;
	};
}



