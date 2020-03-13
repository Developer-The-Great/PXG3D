#pragma once
#include "Component.h"
#include <memory>
#include <vector>

namespace PXG
{
	struct Mat4;
	class AbstractMaterial;
	class Mesh;
	struct CollisionCubeParams;
	//struct Vector3;

	class PhysicsComponent : public Component
	{
	public:
		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		/**@brief creates a mesh by directly copying the meshes in MeshComponent
	     * Note: Should only be used for objects with a low triangle count.
	   */
		void ConstructPhysicsRepresentationFromMeshComponent();

	   /**@brief creates a mesh that will encompass all the vertices of the meshes in MeshComponent
	    * @param [in] CubeParams: a set of parameters that you would like to use instead of using the min/max vertex
	    * Note: not all parameters of CubeParams need to be specified, If a certain parameter is not specified,
	    * the function will instead use the default min max of the mesh (From PhysicsEngine::GetMinMaxPositionOfMeshes).
	   */
		void ConstructCollisionCube(const CollisionCubeParams& CubeParams);
		void ConstructCollisionCube();
		
		PhysicsComponent();

		void SetPhysicsDrawingMaterial(std::shared_ptr<AbstractMaterial> newPhysicsDrawingMaterial);

		void DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection);

		std::vector<std::shared_ptr<Mesh>> GetPhysicsMeshes();

	private:
		std::shared_ptr<AbstractMaterial> physicsRenderingMaterial;

		std::vector<std::shared_ptr<Mesh>> meshes;

	};
}


