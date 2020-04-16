#include "DebugMeshObject.h"
#include "Vector3.h"
#include "AbstractMaterial.h"
#include "Mesh.h"
#include "World.h"

namespace PXG
{
	void DebugMeshObject::Draw(std::weak_ptr<World> world,Mat4 parentTransform, Mat4 view, Mat4 projection)
	{

		if (!material)  return; 

		material->SendUniforms(world,parentTransform * transform, view, projection);

		mesh->Draw(material->GetShader());

	}

	bool DebugMeshObject::IsLifeSpanOver() const
	{
		return lifespan <= 0.0f;
	}

	void DebugMeshObject::DecreaseLifeSpan(float tick)
	{
		lifespan -= tick;
	}

	PrimitiveDrawingMode DebugMeshObject::GetDrawMode() const
	{
		return drawMode;
	}

	DebugMeshObject::DebugMeshObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<AbstractMaterial> material, Mat4 transform, float lifespan, PrimitiveDrawingMode drawMode )
	{
		this->mesh = mesh;
		this->material = material;
		this->lifespan = lifespan;
		this->transform.operator=(transform);
		this->drawMode = drawMode;

	}
	DebugMeshObject::~DebugMeshObject()
	{

	}
}
