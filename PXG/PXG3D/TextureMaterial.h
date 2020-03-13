#pragma once
#include "AbstractMaterial.h"

namespace PXG
{
	class TextureMaterial : public AbstractMaterial
	{
	public:

		TextureMaterial();

		~TextureMaterial();

		virtual Shader* GetShader() override;

		virtual void SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection) override;

	private:

		static void initalizeTextureMaterial();

		static Shader* shader;

	};
}


