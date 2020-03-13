#pragma once
#include "Component.h"
#include "Vector3.h"

namespace PXG
{
	class Shader;

	enum class LightType
	{
		POINT_LIGHT,
		SPOTLIGHT,
		DIRECTIONAL_LIGHT
	};

	class LightComponent : public Component, public std::enable_shared_from_this<LightComponent>
	{

	public:

		LightComponent(Vector3 ambient = Vector3(0.05f, 0.05f, 0.05f),Vector3 diffuse = Vector3(0.7f, 0.7f, 0.7f),Vector3 specular = Vector3(1.0f, 1.0f, 1.0f));

		virtual void SetOwner(std::shared_ptr<GameObject> owner) override;

		void SendUniforms(Shader* shaderProgram, std::vector<int>& lightTypeCount);

		static void SetIsNotUsed(Shader * shaderProgram, int lightType, int lightMax);

		void SetLightType(LightType newType);


		void SetIntensity(float intensity);

	private:

		void SendPointUniform(Shader * shaderProgram, std::vector<int>& lightTypeCount) const;
		void SendSpotUniform(Shader * shaderProgram, std::vector<int>& lightTypeCount) const;
		void SendDirectionalUniform(Shader* shaderProgram, std::vector<int>& lightTypeCount) const;

		glm::vec3 direction = glm::vec3(0, -1, 0);
		float cutoffAngle = glm::cos(glm::radians(30.0f));
		float outercutoffAngle = glm::cos(glm::radians(33.0f));
		float intensity = 20.0f;

		LightType _lightType = LightType::POINT_LIGHT;


		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;

	};
}


