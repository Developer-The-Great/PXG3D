#pragma once
#include "Component.h"
#include "Mat4.h"

namespace PXG
{


	class CameraComponent : public Component, public std::enable_shared_from_this<CameraComponent>
	{
	public:

		CameraComponent();

		~CameraComponent() override;

		Mat4 GetView() const;
		Mat4 GetProjection() const;

		virtual void SetOwner(std::shared_ptr<GameObject> owner) override;

		int GetUseCount() const;


	private:

		Mat4 projection;

	};

}

