#pragma once
#include "AbstractMaterial.h"
#include "Vector3.h"
#include "Shader.h"

namespace PXG
{

	class ColorMaterial :public AbstractMaterial
	{
	public:

		ColorMaterial(Vector3 color  = Vector3(1,0,0));

		virtual Shader* GetShader() override;

		virtual void SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection) override;

	private:

		static void initalizeColorMaterial();

		static Shader* shader;

		Vector3 color;


	};
}


