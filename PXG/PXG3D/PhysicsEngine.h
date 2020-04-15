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
	struct OctreeNode;

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

		



		//------------------------------------------------- Collision Detection Algorithms ----------------------------------------//

		static std::function<void(const std::vector<PhysicsSceneGraphIterationInfo>&, std::vector<PhysicsComponentContainer>&)> CheckFaceNormalsOfMeshForSeperationAxis;

		/**@brief Finds the seperating axis between 2 meshes by iterating through the face normals of 'collisionMeshA' and
		projecting the support points of collisionMeshA and collisionMeshB and checking for colllision under the projection
		Returns a true if a seperation axis is found. The function will also seperation amount if a seperation axis if found.
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector 
		 coming from positionA to positionB, projected into the seperating Axis
		*/
		static bool FindSeperatingAxisByProjectingMeshAandBToFaceNormals(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound);

		/**@brief Attempts to find the seperating axis between 2 meshes by iterating through the face normals of 'collisionMeshA' and
		projecting the support point of collisionMeshB and projecting it into the normals to check for a positive value
		Returns true if a seperation axis is found. The function will also seperation amount if a seperation axis if found.
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector
		 coming from positionA to positionB, projected into the seperating Axis
		*/
		static bool FindSeperatingAxisByExtremePointProjection(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, int& index);

		/**@brief Attempts to find the seperating axis between 2 meshes by iterating through all of the edges of 'collisionMeshA' and 'collisionMeshB'
		and testing the vector that is perpendicular to both edges and using it as a potential seperating axis.
		Returns true if a seperation axis is found. The function will also seperation amount if a seperation axis if found.
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector
		 coming from positionA to positionB, projected into the seperating Axis
		*/
		static bool FindSeperatingAxisByBruteForceEdgeToEdgeCheck(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound);




		

		


		//------------------------------------------------- Collision Detection Helper functions  ----------------------------------------//

		/**@brief finds the furthest vertex in a certain direction of a mesh with a certain transform.
		 *@param [in] mesh : the given mesh that we would like to know its furthest point
		 *@param [in] meshTransform : the transform of the mesh
		 *@param [in] position : the world position of the mesh
		 *@param [in] direction : the direction in which we would measure the distance with
		 *@param [out] index : the index of the furthest vertex, will return -1 if such index is not found
		*/
		static void GetSupportPoint(std::shared_ptr<Mesh> mesh, const Mat4& meshTransform, const Vector3& position,const Vector3& direction, unsigned int & index, Vector3& vertexWorldPosition);


		static void GetSupportPointMinMax(std::shared_ptr<Mesh> mesh, const Mat4& meshTransform, const Vector3& position, const Vector3& direction
			, unsigned int & indexMin,  Vector3& vertexWorldPositionMin, unsigned int & indexMax,  Vector3& vertexWorldPositionMax);

		/**@brief: return true if the given axis is the seperation of the transformed meshes. Also returns a seperation amount if 
		the given axis is indeed the axis of seperation
		 *@param [in] axis : the axis that we would like to check for an axis of seperation
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector
		 coming from positionA to positionB, projected into the seperating Axis
		*/
		static bool TestAxisDirection(const Vector3& axis, std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound);

		/** @brief Finds the closest and furthest object space vertice position relative to the origin (0,0,0)
		* @param [out] min: the closest vertice from the origin
		* @param [out] max: the furthest vertice from the origin
		* @param [in] meshes: the mesh closest towards
		*/
		static void GetMinMaxPositionOfMeshes(Vector3& min, Vector3& max, std::vector< std::shared_ptr<Mesh>> meshes);

		/** @brief Given an std::vector of vertices,Finds the closest and furthest object space vertice position relative to the origin (0,0,0)
		* @param [out] min: the closest vertice from the origin
		* @param [out] max: the furthest vertice from the origin
		* @param [in] vertices: a vector containing the vertices that you would like to find the min-max of;
		*/
		static void GetMinMaxPositionOfVertices(Vector3& min, Vector3& max,const std::vector<Vector3> vertices);


	private:

		
		
		void recursiveRetrievePhysicsComponent(std::shared_ptr<GameObject> rootObj, std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, Mat4 transform);

		//----------------------------------------------- Broad Phase Optimization Algorithms --------------------------------------------------------------------------//

		std::function<void(const std::vector<PhysicsSceneGraphIterationInfo>&, std::vector<PhysicsComponentContainer>&)> OptimizeBroadPhase;


		/**@brief Places all physicsComponents in the vector of PhysicsSceneGraphIterationInfo into 1 physicsComponentContainer
		 *param [in] physicsComponents : a vector of PhysicsSceneGraphIterationInfo, containing all physicsComponents that have colliders
		 *param [out] physicsComponentContainers : a vector of PhysicsComponentContainer, ...........
		*/
		void BruteForceBroadPhase(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers);
		void BroadPhaseOctreeOptimization(const std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, std::vector<PhysicsComponentContainer>& physicsComponentContainers);


		//----------------------------------------------- Octree Helper Function --------------------------------------------------------------------------//

		void recursiveOctreeSplit(std::shared_ptr<OctreeNode> node, std::vector<std::shared_ptr<OctreeNode>>& finalNodes, int currentdepthCount, const int minObjectCount = 3, const int maxDepthCount = 6);


		//-------------------------------------------------- Raycasting -----------------------------------------------------------//

		//does a raytrace to all objects in the world by recursively going through the scene graph
		static void recursiveGameObjectRaytrace(const Vector3& position,const Vector3& direction, HitInfo& hitInfo, std::shared_ptr<GameObject> gameObject, Mat4 currentTransform, bool isUsingPhysicsComponent);

		static void rayToMeshIntersection(const Vector3& position, const Vector3& direction, HitInfo& hitInfo,std::shared_ptr<Mesh> mesh, Mat4 objectTransform,std::shared_ptr<GameObject> owner);

		static void recursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);

		const float tickTime = 0.02f;

		static double gravity;

		float tickTimeRemaining;

		std::shared_ptr<World> world;




	};

}


