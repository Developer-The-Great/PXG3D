#include "TextureMaterial.h"
#include "Shader.h"
#include "Mat4.h"

namespace PXG
{
	Shader* TextureMaterial::shader = nullptr;

	TextureMaterial::TextureMaterial()
	{
		initalizeTextureMaterial();
	}

	TextureMaterial::~TextureMaterial()
	{
		//Debug::Log("Texture Material Destroyed");
	}

	Shader * TextureMaterial::GetShader()
	{
		return shader;
	}

	void TextureMaterial::SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection)
	{
		shader->Use();

		shader->SetMat4("MVP", model * view * projection);
		//texture is sent in the mesh

	}

	void TextureMaterial::initalizeTextureMaterial()
	{
		if (!shader)
		{
			char vertexFilename[64];
			char fragmentFilename[64];

			//TODO Change  "assets/shaders/" :Config::PXG_SHADER_PATH
			//There is an error associated with it

			strncpy_s(vertexFilename, "assets/shaders/", sizeof(vertexFilename));
			strncat_s(vertexFilename, "PXGTexture.vert.glsl", sizeof(vertexFilename));

			strncpy_s(fragmentFilename, "assets/shaders/", sizeof(fragmentFilename));
			strncat_s(fragmentFilename, "PXGTexture.frag.glsl", sizeof(fragmentFilename));

			shader = new Shader(vertexFilename, fragmentFilename);

		}
	}

}
