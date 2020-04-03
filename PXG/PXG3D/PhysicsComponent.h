#pragma once
#include "Component.h"
#include <memory>
#include <vector>

namespace PXG
{
	struct Mat4;
	class AbstractMaterial;
	class Mesh;
	class PhysicsCollider;
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

	    /**@brief creates a cube that will encompass all the vertices of the meshes in MeshComponent
	     * @param [in] CubeParams: a set of parameters that you would like to use instead of using the min/max vertex
	     * Note: not all parameters of CubeParams need to be specified, If a certain parameter is not specified,
	     * the function will instead use the default min max of the mesh (From PhysicsEngine::GetMinMaxPositionOfMeshes).
	   */
		void ConstructCollisionCube(const CollisionCubeParams& CubeParams);
		void ConstructCollisionCube();
		
		PhysicsComponent();

	    /**@brief Sets the material that will be used when the physics representation is drawn.
		 * @param [in] newPhysicsDrawingMaterial: the material that will be used to render the mesh
	   */
		void SetPhysicsDrawingMaterial(std::shared_ptr<AbstractMaterial> newPhysicsDrawingMaterial);

	   /**@brief Draws the mesh in the physics Component
		* @param [in] parentTransform: the transform of the GameObject that this GameObject is parented to
		* @param [in] view: the view matrix of the camera that is currently used to render the world
		* @param [in] projection: the projection matrix of the camera that is currently used to render the world
	   */ 
		void DrawPhysicsRepresentation(Mat4 parentTransform, Mat4 view, Mat4 projection);

		void SetIsTrigger(bool newTriggerState);

		bool IsTrigger() const;

		std::vector<std::shared_ptr<Mesh>> GetPhysicsMeshes();

		std::shared_ptr<PhysicsCollider> GetCollider(int i = 0);

		int GetColliderCount() const;

	private:
		std::shared_ptr<AbstractMaterial> physicsRenderingMaterial;

		std::vector <std::shared_ptr<PhysicsCollider>> physicsColliders;

		bool isTrigger = false;


	};
}


