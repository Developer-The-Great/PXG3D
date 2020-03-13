
#include "ColorMaterial.h"
#include "Mat4.h"

#include "Debug.h"

namespace PXG
{
	Shader * ColorMaterial::shader = nullptr;

	ColorMaterial::ColorMaterial(Vector3 color)
	{
		Debug::Log("Color material Init");
		initalizeColorMaterial();
		this->color = color;
	}
	Shader * ColorMaterial::GetShader()
	{
		return shader;
	}
	void ColorMaterial::SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection)
	{
		shader->Use();

		shader->SetMat4("MVP",  model * view * projection);
		shader->SetVec3("diffuseColor", color);

	}
	void ColorMaterial::initalizeColorMaterial()
	{
		if (!shader)
		{
			char vertexFilename[64];
			char fragmentFilename[64];

			//TODO Change  "assets/shaders/" :Config::PXG_SHADER_PATH
			//There is an error associated with it

			strncpy_s(vertexFilename, "assets/shaders/", sizeof(vertexFilename));
			strncat_s(vertexFilename, "PXGColor.vert.glsl", sizeof(vertexFilename));

			strncpy_s(fragmentFilename, "assets/shaders/", sizeof(fragmentFilename));
			strncat_s(fragmentFilename, "PXGColor.frag.glsl", sizeof(fragmentFilename));

			shader = new Shader(vertexFilename, fragmentFilename,"color");

		}


	}
}