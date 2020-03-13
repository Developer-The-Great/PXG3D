#include "CameraComponent.h"
#include "GameObject.h"
#include <memory>

#include "ScreenSize.h"
#include "World.h"

namespace PXG
{
	CameraComponent::CameraComponent()
	{
		bool hasOwner = !owner.expired();

		projection = glm::perspective(glm::radians(60.0f), PXG::ScreenSize::WIDTH / PXG::ScreenSize::HEIGHT, 0.1f, 100000.0f);

		//projection = glm::ortho(-ScreenSize::WIDTH/2, ScreenSize::WIDTH / 2, -ScreenSize::HEIGHT / 2, ScreenSize::HEIGHT / 2, 0.000f, 100000000.0f);
	}
	CameraComponent::~CameraComponent()
	{
		Debug::Log("CAM DESTROYED");
	}
	Mat4 CameraComponent::GetView() const
	{
		bool hasOwner = !(owner.expired());
		if (hasOwner)
		{
			auto ownerPtr = owner.lock();

			auto transform = ownerPtr->GetTransform();

			return glm::inverse(transform->GetWorldTransform().ToGLM());
		}

		return Mat4();
	}
	Mat4 CameraComponent::GetProjection() const
	{
		return projection;
	}
	void CameraComponent::SetOwner(std::shared_ptr<GameObject> owner)
	{
		Component::SetOwner(owner);

		if (auto worldPtr = owner->GetWorld().lock())
		{
			worldPtr->SetCamera(shared_from_this());
		}
	}
	int CameraComponent::GetUseCount() const
	{
		return owner.use_count();
	}
}
