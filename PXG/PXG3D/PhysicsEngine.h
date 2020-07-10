#pragma once
#include <memory>
#include <vector>
#include <functional>

namespace PXG
{

	struct HalfEdgeEdge;
	struct Vector3;
	struct HitInfo;
	class GameObject;
	class PhysicsComponent;
	class MeshComponent;
	class ConvexCollider;

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

		/**@brief Gets all the rigidbodies in the world and calls the Integrate function on them
		 *@param [in] dt : a float representing the amount of time we would like to integrate
		*/
		void Integrate(float dt);

		//TODO documentation for this
		void IncrementTickCount();

		void ResetTickCount();


		/**@brief Sets the World where the physics Engine would 'act' in
		 *@param [in] world : a shared_ptr to the world where we would like the physics engine to act in.
		*/
		void SetWorld(std::shared_ptr<World> world);

		/**@brief Returns the time step of the physics engine
		*/
		float GetTickRate() const;

		/**@bried Returns the tick time remaining
		*/
		float GetTickTimeRemaining() const;

		/**@brief Sets the amount of time that the physics engine needs to update
		 *@param [in]: a float representing the amount of time that the physics engine needs to update
		*/
		void AccumulateTickTime(float tick);

		/**@brief return true if 'tickTimeRemaining' is bigger than zero
		*/
		bool IsTicking();

		/**@brief return the amount of 'tickTimeRemaining' after it has been decreased by a single time step
		*/
		float DecreaseRemainingTickTime();

		/**@brief Checks if each GameObject with a non-empty PhysicsComponent collides with other gameObjects with a 
		non-empty PhysicsComponent
		*/
		void CheckCollisions();

		/**@brief returns the gravity the physicsEngine uses*/
		static double GetGravity();

		/**@brief sets the gravity the physicsEngine uses
		 *@param [in]: a double representing new gravity used
		*/
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
		Returns true if a seperation axis is found. The function will also output the seperation amount if a seperation axis if found.
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector
		 coming from positionA to positionB, projected into the seperating Axis
		*/
		static bool FindSeparatingAxisByBruteForceEdgeToEdgeCheck(std::shared_ptr<Mesh> collisionMeshA, std::shared_ptr<Mesh> collisionMeshB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound);

		/**@brief Attempts to find the seperating axis between 2 meshes by iterating through only the edges that create a valid seperating axis.
		Returns true id a seperating axis is found.The function will also output the seperation amount if a seperation axis if found.
		 *@param [in] collisionMeshA,collisionMeshB : the meshes that will be projected into the face normals of 'collisionMeshA'
		 *@param [in] transformA,transformB : the world transforms of collisionMeshA and collisionMeshB
		 *@param [in] positionA,positionB : the world position of collisionMeshA and collisionMeshB
		 *@param [out] seperationFound: the result of substracting the supportPoint of A and B projected into the seperating Axis from the vector
		*/
		static bool FindSeparatingAxisByGaussMapEdgeToEdgeCheck(ConvexCollider * colliderA, ConvexCollider * colliderB,
			const Mat4& transformA, const Mat4& transformB, const Vector3& positionA, const Vector3& positionB, float& seperationFound,
			HalfEdgeEdge * firstEdge,HalfEdgeEdge *secondEdge);


		//------------------------------------------------- Collision Detection Helper functions  ----------------------------------------//

		/**@brief finds the furthest vertex in a certain direction of a mesh with a certain transform.
		 *@param [in] mesh : the given mesh that we would like to know its furthest point
		 *@param [in] meshTransform : the transform of the mesh
		 *@param [in] position : the world position of the mesh
		 *@param [in] direction : the direction in which we would measure the distance with
		 *@param [out] index : an integer representing the index of the furthest vertex, will return -1 if such index is not found
		 *@param [out] vertexWorldPosition : a Vector3 representing the world position of the given index
		*/
		static void GetSupportPoint(std::shared_ptr<Mesh> mesh, const Mat4& meshTransform, const Vector3& position,const Vector3& direction, unsigned int & index, Vector3& vertexWorldPosition);

		/**@brief finds the 2 vertices that represent furthest vertex in a given direction and the direction opposite to it.
		 **@param [in] mesh : the given mesh that we would like to know its furthest point
		 *@param [in] meshTransform : the transform of the mesh
		 *@param [in] position : the world position of the mesh
		 *@param [in] direction : the direction in which we would measure the distance with
		 *@param [out] indexMin : the index of the furthest vertex in the direction parrallel to the Vector3 'direction', will return -1 if such index is not found
		 *@param [out] vertexWorldPositionMin : a Vector3 representing the world position of the vertex at index 'indexMin'
		 *@param [out] indexMax : the index of the furthest vertex in the direction opposite to the Vector3 'direction', will return -1 if such index is not found
		 *@param [out] vertexWorldPositionMax : a Vector3 representing the world position of the vertex at index 'indexMax'
		*/
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

		/** @brief returns true if given 2 Half Edges and their respective world transform, create a Minkoswski Face
		 *@param [in] edgeA: a HalfEdgeEdge from the first Mesh
		 *@param [in] edgeB: a HalfEdgeEdge another Mesh
		 *param [in] transformA: the world transform of the first Mesh
		 *@param [in] transformB: the world transform of the second Mesh
		*/
		static bool AttemptBuildMinkowskiFace(HalfEdgeEdge* edgeA, HalfEdgeEdge* edgeB,const Mat4& transformA,const Mat4& transformB);

		/** @bried returns true if given 2 arcs of a sphere, one represented by transformedNormalA1 and transformedA2,
		and another arc represented by transformedNormalB1 and transformedNormalB2 are intersecting
		 *@param [in] transformedA1: a Vector3 representing the start of the first arc
		 *@param [in] transformedA2: a Vector3 representing the end of the first arc
		 *@param [in] transformedB1: a Vector3 representing the start of the second arc
		 *@param [in] transofrmedB2: a Vector3 representing the end of the second arc
		*/
		static bool IsMinkowskiFace(Vector3 & transformedNormalA1, Vector3 & transformedNormalA2,
			Vector3 & transformedNormalB1, Vector3 & transformedNormalB2);



	private:
		/**
		*/
		void recursiveRetrievePhysicsComponent(std::shared_ptr<GameObject> rootObj, std::vector<PhysicsSceneGraphIterationInfo>& physicsComponents, Mat4 transform,int id = 0);

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

		static void recursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);

		const float tickTime = 0.02f;

		const int maxTickCountPerFrame = 3;
		int currentFrameTickCount = 0;

		static double gravity;

		float tickTimeRemaining;

		std::shared_ptr<World> world;

		const int minObjectCount = 4;
		const int maxDepthCount = 5;


	};

}


