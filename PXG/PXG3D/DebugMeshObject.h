#pragma once
#include <memory>
#include "Mat4.h"


namespace PXG
{

	
	class AbstractMaterial;
	class Mesh;
	class World;
	enum class PrimitiveDrawingMode;


	


	class DebugMeshObject
	{
	public:

		DebugMeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<AbstractMaterial> material, Mat4 transform, float lifespan,PrimitiveDrawingMode drawMode);

		~DebugMeshObject();

		void Draw(std::weak_ptr<World> world, Mat4 parentTransform, Mat4 view, Mat4 projection);

		bool IsLifeSpanOver() const;

		void DecreaseLifeSpan(float tick);

		PrimitiveDrawingMode GetDrawMode() const;

		//static std::shared_ptr<DebugMeshObject> InstantiateMeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<AbstractMaterial> material, Mat4 transform, float lifespan);


	private:

		

		std::shared_ptr<AbstractMaterial> material;
		std::shared_ptr<Mesh> mesh;

		Mat4 transform;

		float lifespan;

		PrimitiveDrawingMode drawMode;
			
	};
}


