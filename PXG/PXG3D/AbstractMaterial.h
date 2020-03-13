#pragma once
#include <memory>

namespace PXG
{
	class World;
	struct Mat4;
	class Shader;

	class AbstractMaterial
	{

	public:
		AbstractMaterial() = default;
		virtual ~AbstractMaterial() = default;

		virtual Shader* GetShader() = 0;

		virtual void SendUniforms(std::weak_ptr<World> _world,Mat4 model,Mat4 view,Mat4 projection) = 0;

	};
}
