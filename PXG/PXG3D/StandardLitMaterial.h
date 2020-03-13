#pragma once
#include "AbstractMaterial.h"
#include "Vector3.h"
namespace PXG
{
	class StandardLitMaterial :public AbstractMaterial
	{
	public:
		StandardLitMaterial(Vector3 ambient = Vector3(1.0,1.0,1.0), Vector3 diffuse = Vector3(1.0, 1.0, 1.0),
							Vector3 speculars = Vector3(1.0, 1.0, 1.0), float shininess = 32.0f);
		~StandardLitMaterial();

		virtual Shader* GetShader() override;
		virtual void SendUniforms(std::weak_ptr<World> _world, Mat4 model, Mat4 view, Mat4 projection) override;

	private:
		static Shader* shader;
		static void initalizeStandardLitMaterial();

		float shininess;
	};
}


