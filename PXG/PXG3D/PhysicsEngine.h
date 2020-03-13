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

		//cast a ray from a position to a direction. It can either use the meshComponent or the physicsComponent
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
		static Vector3 GetOrthographicCameraWorldPosition(float i, float j, float screenWidth, float screenHeight, std::shared_ptr<GameObject> object);

	private:
		//does a raytrace to all objects in the world by recursively goinf through the scene graph
		static void recursiveGameObjectRaytrace(const Vector3& position,const Vector3& direction, HitInfo& hitInfo, std::shared_ptr<GameObject> gameObject, Mat4 currentTransform, bool isUsingPhysicsComponent);

		static void rayToMeshIntersection(const Vector3& position, const Vector3& direction, HitInfo& hitInfo,std::shared_ptr<Mesh> mesh, Mat4 objectTransform,std::shared_ptr<GameObject> owner);

		static void recursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);

		//detects if a ray that has an origin and a direction is intersecting with a triangle with vertices v1,v2,and v3
		static void rayTriangleIntersection(Vector3 v1,Vector3 v2,Vector3 v3 , const Vector3& rayPosition, const Vector3& rayDirection, Mat4 objectTransform, HitInfo& hitInfo, std::shared_ptr<GameObject> owner);

		const float tickTime = 0.02f;

		static double gravity;

		float tickTimeRemaining;

		std::shared_ptr<World> world;


	};

}


