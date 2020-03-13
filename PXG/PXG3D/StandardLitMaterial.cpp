#include "StandardLitMaterial.h"
#include "Vector3.h"
#include "Shader.h"
#include "Mat4.h"
#include "LightComponent.h"
#include "World.h"
#include "CameraComponent.h"
namespace PXG
{
	Shader * StandardLitMaterial::shader = nullptr;

	StandardLitMaterial::StandardLitMaterial(Vector3 ambient, Vector3 diffuse, Vector3 speculars, float shininess)
	{
		this->shininess = shininess;
		initalizeStandardLitMaterial();
	}
	StandardLitMaterial::~StandardLitMaterial()
	{
		Debug::Log("standard lit dest");
	}
	Shader * StandardLitMaterial::GetShader()
	{
		return shader;
	}
	void StandardLitMaterial::SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection)
	{
		shader->Use();

		if (_world.expired()) { return; }


		auto WorldPtr = _world.lock();

		int index = 0;
		std::vector<int> lightCounts;
		lightCounts.clear();

		int nrLightTypes = 3;

		for (int i = 0; i < nrLightTypes; i++)
		{
			lightCounts.push_back(0);
		}

		//reset all lights
		for (int i = 0; i < nrLightTypes; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				LightComponent::SetIsNotUsed(shader, i, j);
			}
		}


		for (auto const& light : WorldPtr->GetLights())
		{
			light->SendUniforms(shader, lightCounts);
		}

		//send MVP
		shader->SetMat4("MVP", model * view * projection);

		//send Model
		shader->SetMat4("model", model );
		//send shininess
		shader->SetFloat("material.shininess", shininess);

		//send camera position
		shader->SetVec3("eyePos", WorldPtr->GetCamera()->GetOwnerTransform()->GetPosition());


	}
	void StandardLitMaterial::initalizeStandardLitMaterial()
	{
		if (!shader)
		{
			char vertexFilename[64];
			char fragmentFilename[64];

			//TODO Change  "assets/shaders/" :Config::PXG_SHADER_PATH
			//There is an error associated with it

			strncpy_s(vertexFilename, "assets/shaders/", sizeof(vertexFilename));
			strncat_s(vertexFilename, "PXGStandard.vert.glsl", sizeof(vertexFilename));

			strncpy_s(fragmentFilename, "assets/shaders/", sizeof(fragmentFilename));
			strncat_s(fragmentFilename, "PXGStandard.frag.glsl", sizeof(fragmentFilename));

			shader = new Shader(vertexFilename, fragmentFilename, "light shader");

		}
	}
}

