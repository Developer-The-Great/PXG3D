#include "Utils.h"
#include "Vector3.h"
#include "World.h"

#include "CameraComponent.h"
namespace PXG
{
	Vector3 Utils::GetProjectionCameraScreenToWorld(float i, float j, std::shared_ptr<World> world)
	{
		auto camera = world->GetCamera();

		if (!camera) 
		{ 
			Debug::Log(Verbosity::Error, "World does not have a reference to camera!");
			return Vector3(); 
		}

		auto transform = camera->GetOwner()->GetTransform();

		Vector3 cameraForward = transform->GetForward();
		Vector3 cameraRight = transform->GetRight();
		Vector3 cameraUp = transform->GetUp();

		float betaMax = Mathf::Tan(Mathf::Deg2Rad * camera->GetFOVYDegrees() / 2.0f);
		float alphaMax = betaMax * camera->GetScreenWidth() / camera->GetScreenHeight(); 
		
		
		float halfScreenWidth = camera->GetScreenWidth() / 2.0f;
		float halfScreenHeight = camera->GetScreenHeight() / 2.0f;

		float kAlpha = (i - halfScreenWidth) / halfScreenWidth;
		float kBeta = ( halfScreenHeight - j) / halfScreenHeight;

		Debug::Log("kAlpha {0} ", kAlpha);
		Debug::Log("kBeta {0} ", kBeta);

		float alpha = alphaMax * kAlpha;
		float beta = betaMax * kBeta;



		Debug::Log("transform->GetPosition() {0} ", transform->GetPosition().ToString());

		Vector3 result = transform->GetPosition() + (cameraRight * alpha + cameraUp * beta + cameraForward);

		return result;
	}
}

