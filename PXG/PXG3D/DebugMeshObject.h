#pragma once
#include <memory>

#define PXG_THIS_FRAME_ONLY 0.0f

namespace PXG
{
	class AbstractMaterial;
	struct Vector3;
	class Mesh;
	struct Mat4;


	enum class DebugPrimitive
	{
		LINE,
		CUBE,

	};


	class DebugMeshObject
	{
	public:

		void Draw(Mat4 parentTransform, Mat4 view, Mat4 projection);



	private:

		DebugMeshObject(std::shared_ptr<Mesh> mesh, Mat4 transform, float lifespan = PXG_THIS_FRAME_ONLY);

		std::shared_ptr<AbstractMaterial> material;



			
	};
}


