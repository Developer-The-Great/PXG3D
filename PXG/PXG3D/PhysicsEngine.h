#pragma once
#include <memory>
#include <vector>
#include <functional>

namespace PXG
{
	struct Vector3;
	struct HitInfo;
	class GameObject;
	class PhysicsComponent;
	class MeshComponent;
	struct PhysicsSceneGraphIterationInfo;
	struct PhysicsComponentContainer;
	class World;
	class Mesh;
	struct Mat4;

	class PhysicsEngine
	{
	public:

		PhysicsEngine();

		void AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent);

		void SetWorld(std::shared_ptr<World> world);

		float GetTickRate() const;

		float GetCurrentTickRemaining() const;

		void SetTickRemaining(float tick);

		bool IsTicking();

		float DecreaseRemainingTickTime();

		void CheckCollisions();







		static double GetGravity();

		static void SetGravity(double newGravity);

		/**@brief Casts a ray with a certain position and direction and returns true if it hits a mesh
		* @param [in] position: the start point of the raycast
		* @param [in] direction: the direction where the ray will be cast
		* @param [out] hitInfo: a struct that will contain information about the object hit (position,normals,a pointer to the object,etc)
		* @param [in] world: a pointer to the world that the raycast will take place
		* @param [in] usePhysicsComponent: a bool that indicates if you want to do a geometry raycast or use the physics component mesh instead
	    */
		static bool Raycast(const Vector3& position,const Vector3& direction,HitInfo& hitInfo,std::shared_ptr<World> world,bool usePhysicsComponent = true);


		/** @brief Finds the closest and furthest vertice position relative to the origin (0,0,0)
		* @param [out] min: the closest vertice from the origin
		* @param [out] max: the furthest vertice from the origin
		* @param [in] meshes: the mesh closest towards 
		*/
		static void GetMinMaxPositionOfMeshes(Vector3& min, Vector3& max, std::vector< std::shared_ptr<Mesh>> meshes);

		//returns the world position of pixel (i,j) in a camera with a screen width of screenWidth and a screen height of screenHeight.
		static Vector3 GetOrthographicCameraWorldPosition(float i, float j, float screenWidth, float screenHeight, std::shared_ptr<World> world);

		/**@brief checks if triangle with vertices v1,v2,and v3 intersects with a ray coming from rayPosition and rayDirection
		 *@param [in] v1,v2,v3 : the vertices of the triangle
		 *@param [in] rayPosition : the origin of the ray
		 *@param [in] rayDirection : the direction of the ray
		 *@param [in] objectTransform : the transform the the triangle
		 *@param [out] hitInfo : a struct that outputs the hit information
		 *@param [in] owner : the GameObject that owns the triangle, if it exist
		*/
		static void RayTriangleIntersection(Vector3 v1, Vector3 v2, Vector3 v3, const Vector3& rayPosition, const Vector3& rayDirection,
			Mat4 objectTransform, HitInfo& hitInfo, std::shared_ptr<GameObject> owner);

		/**@brief finds the furthest vertex in a certain direction of a mesh with a certain transform.
		 *@param [in] mesh : the given mesh that we would like to know its furthest point
		 *@param [in] meshTransform : the transform of the mesh
		 *@param [in] position : the world position of the mesh
		 *@param [in] direction : the direction in which we would measure the distance with
		 *@param [out] index : the index of the furthest vertex, will return -1 if such index is not found
		*/
		static void GetSupportPoint(std::shared_ptr<Mesh> mesh, Mat4 meshTransform, Vector3 position, Vector3 direction, unsigned int & index, Vector3& vertexWorldPosition);







	private:

		
		
		void recursiveRetrievePhysicsComponent(std::shared_ptr<GameObject> rootObj, std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, Mat4 transform);

		static std::function<void(const std::vector<PhysicsSceneGraphIterationInfo>&, std::vector<PhysicsComponentContainer>&)> OptimizeBroadPhase ;

		/**@brief Places all physicsComponents in the vector of PhysicsSceneGraphIterationInfo into 1 physicsComponentContainer
		 *param [in] physicsComponents : a vector of PhysicsSceneGraphIterationInfo, containing all physicsComponents that have colliders
		 *param [out] physicsComponentContainers : a vector of PhysicsComponentContainer, ...........
		*/
		static void BruteForceBroadPhase(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers);
		static void BroadPhaseOctreeOptimization(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers);




		//does a raytrace to all objects in the world by recursively goinf through the scene graph
		static void recursiveGameObjectRaytrace(const Vector3& position,const Vector3& direction, HitInfo& hitInfo, std::shared_ptr<GameObject> gameObject, Mat4 currentTransform, bool isUsingPhysicsComponent);

		static void rayToMeshIntersection(const Vector3& position, const Vector3& direction, HitInfo& hitInfo,std::shared_ptr<Mesh> mesh, Mat4 objectTransform,std::shared_ptr<GameObject> owner);

		static void recursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);


		

		const float tickTime = 0.02f;

		static double gravity;

		float tickTimeRemaining;

		std::shared_ptr<World> world;




	};

}


