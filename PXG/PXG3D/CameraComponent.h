#pragma once
#include "Component.h"
#include "Mat4.h"

namespace PXG
{


	class CameraComponent : public Component, public std::enable_shared_from_this<CameraComponent>
	{
	public:

		CameraComponent(float FOV = 60.0f, float screenWidth = 1280.0f,float screenHeight = 720.0f);

		~CameraComponent() override;

		Mat4 GetView() const;
		Mat4 GetProjection() const;

		//TODO implement SetFOV
		void SetFOVY(float degrees);
		

		float GetFOVYDegrees() const;
		float GetFOVXDegrees() const;

		float GetScreenWidth() const;
		float GetScreenHeight() const;



		virtual void SetOwner(std::shared_ptr<GameObject> owner) override;

		int GetUseCount() const;


	private:

		void updateProjection();


		float fovy;
		float screenWidth;
		float screenHeight;

		Mat4 projection;

	};

}

