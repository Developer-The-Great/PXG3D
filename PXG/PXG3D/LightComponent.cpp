#include "LightComponent.h"
#include "Shader.h"
#include <vector>
#include "GameObject.h"
#include "World.h"
namespace PXG
{

	LightComponent::LightComponent(Vector3 ambient, Vector3 diffuse, Vector3 specular) : Component()
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

	}
	void LightComponent::SetOwner(std::shared_ptr<GameObject> owner)
	{
		Component::SetOwner(owner);

		if (auto worldPtr = owner->GetWorld().lock())
		{
			worldPtr->AddLight(shared_from_this());
		}
	}
	void LightComponent::SendUniforms(Shader * shaderProgram, std::vector<int>& lightTypeCount)
	{
		switch (_lightType)
		{
		case LightType::POINT_LIGHT:
			SendPointUniform(shaderProgram, lightTypeCount);

			break;
		case LightType::SPOTLIGHT:
			SendSpotUniform(shaderProgram, lightTypeCount);

			break;
		case LightType::DIRECTIONAL_LIGHT:
			SendDirectionalUniform(shaderProgram, lightTypeCount);

			break;
		default:
			break;
		}
	}
	void LightComponent::SetIsNotUsed(Shader * shaderProgram, int lightType, int lightMax)
	{
		std::string index = std::to_string(lightMax);

		std::string arrayName = "";

		switch ((LightType)lightType)
		{
		case LightType::SPOTLIGHT:
			arrayName = "spotLights";
			break;

		case LightType::POINT_LIGHT:
			arrayName = "pointLights";
			break;

		case LightType::DIRECTIONAL_LIGHT:
			arrayName = "dirLights";
			break;

		}

		shaderProgram->SetInt(arrayName + "[" + index + "].isLightUsed",false);
	}

	void LightComponent::SetLightType(LightType newType)
	{
		_lightType = newType;
	}

	void LightComponent::SetIntensity(float intensity)
	{
		this->intensity = intensity;
	}

	void LightComponent::SendPointUniform(Shader * shaderProgram, std::vector<int>& lightTypeCount) const
	{
		Vector3 position = GetOwner()->GetTransform()->GetPosition();

		int lightCount = lightTypeCount.at((int)LightType::POINT_LIGHT);
		std::string index = std::to_string(lightCount);

		shaderProgram->SetVec3("pointLights[" + index + "].position",position);
		
		shaderProgram->SetVec3("pointLights[" + index + "].ambient", ambient);

		shaderProgram->SetVec3("pointLights[" + index + "].diffuse", diffuse);

		shaderProgram->SetVec3("pointLights[" + index + "].specular",specular);

		shaderProgram->SetFloat("pointLights[" + index + "].intensity", intensity);

		shaderProgram->SetInt("pointLights[" + index + "].isLightUsed", true);


		lightTypeCount.at((int)LightType::POINT_LIGHT)++;

	}
	void LightComponent::SendSpotUniform(Shader * shaderProgram, std::vector<int>& lightTypeCount) const
	{
		Vector3 position = GetOwner()->GetTransform()->GetPosition();

		int lightCount = lightTypeCount.at((int)LightType::SPOTLIGHT);

		std::string index = std::to_string(lightCount);

		shaderProgram->SetVec3("spotLights[" + index + "].direction", direction);

		shaderProgram->SetVec3("spotLights[" + index + "].position", position);

		shaderProgram->SetVec3("spotLights[" + index + "].ambient", ambient);

		shaderProgram->SetVec3("spotLights[" + index + "].diffuse", diffuse);

		shaderProgram->SetVec3("spotLights[" + index + "].specular", specular);

		shaderProgram->SetFloat("spotLights[" + index + "].cutoff", cutoffAngle);

		shaderProgram->SetFloat("spotLights[" + index + "].outerCutoff", outercutoffAngle);

		shaderProgram->SetFloat("spotLights[" + index + "].intensity", intensity);

		shaderProgram->SetInt("spotLights[" + index + "].isLightUsed", true);


		lightTypeCount.at((int)LightType::SPOTLIGHT)++;


	}

	void LightComponent::SendDirectionalUniform(Shader * shaderProgram, std::vector<int>& lightTypeCount) const
	{
		int lightCount = lightTypeCount.at((int)LightType::DIRECTIONAL_LIGHT);
		std::string index = std::to_string(lightCount);

		shaderProgram->SetVec3("dirLights[" + index + "].direction", direction);

		shaderProgram->SetVec3("dirLights[" + index + "].ambient", ambient);

		shaderProgram->SetVec3("dirLights[" + index + "].diffuse", diffuse);
	
		shaderProgram->SetVec3("dirLights[" + index + "].specular", specular);

		shaderProgram->SetInt("dirLights[" + index + "].isLightUsed", true);


		lightTypeCount.at((int)LightType::DIRECTIONAL_LIGHT)++;

	}
}

