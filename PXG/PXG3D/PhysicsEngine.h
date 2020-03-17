#pragma once
#include <memory>
#include <vector>



namespace PXG
{
	struct Vector3;
	struct HitInfo;
	class GameObject;
	class PhysicsComponent;
	class MeshComponent;
	class World;
	class Mesh;
	struct Mat4;

	class PhysicsEngine
	{
	public:

		void AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent);

		void SetWorld(std::shared_ptr<World> world);

		float GetTickRate() const;

		

		float GetCurrentTickRemaining() const;

		void SetTickRemaining(float tick);

		bool IsTicking();

		float DecreaseRemainingTickTime();

		static double GetGravity();

		static void SetGravity(double newGravity);

		/**@brief Casts a ray with a certain position and direction and returns true if it hits a mesh
		* @param [in] position: the start point of the raycast
		* @param [in] direction: the direction where it will raycast
		* @param [out] hitInfo: a struct that will contain information about the object hit (position,normals,a pointer to the object,etc)
		* @param [in] world: a pointer to the world that the raycast will take place
		* @param [in] usePhysicsComponent: a bool that indicates if you want to do a geometry raycast or use the physics component mesh instead
	    */
		static bool Raycast(const Vector3& position,const Vector3& direction,HitInfo& hitInfo,std::shared_ptr<World> world,bool usePhysicsComponent = true);


		//finds 2 Vectors that encompass all of the vertices of the meshes given
		//Min---------------
		// |	O		   |
		// |			   |
		// |			  O|
		// |O			   |
		// ----------------Max 
		//['O' == vertex]
		static void GetMinMaxPositionOfMeshes(Vector3& min, Vector3& max, std::vector< std::shared_ptr<Mesh>> meshes);

		//returns the world position of pixel (i,j) in a camera with a screen width of screenWidth and a screen height of screenHeight.
		static Vector3 GetOrthographicCameraWorldPosition(float i, float j, float screenWidth, float screenHeight, std::shared_ptr<World> world);

		//detects if a ray that has an origin and a direction is intersecting with a triangle with vertices v1,v2,and v3

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

	private:


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


