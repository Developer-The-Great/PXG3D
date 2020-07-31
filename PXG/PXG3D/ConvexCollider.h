#pragma once
#include "PhysicsCollider.h"
#include "HalfEdgeEdge.h"
#include "PhysicsMeshFace.h"
#include <map>
namespace PXG
{

	typedef std::pair<unsigned int, unsigned int> edgeVertexIndexPair;
	typedef std::map<edgeVertexIndexPair, HalfEdgeEdge*> VertexIndexToHalfEdgePtr;

	class ConvexCollider : public PhysicsCollider 
	{
	public:

		ConvexCollider() = default;

		~ConvexCollider();

		/*virtual bool BoundingVolumeCheck(std::shared_ptr<PhysicsCollider> otherPhysicsCollider) override;

		virtual bool BoundingVolumeCheckWith(std::shared_ptr<SphereCollider> sphereCollider) override;
		virtual bool BoundingVolumeCheckWith(std::shared_ptr<ConvexCollider> convexCollider) override;*/

		virtual void CheckCollision(std::shared_ptr<PhysicsCollider> physicsCollider,Manifold& manifold) override;


		virtual void CheckCollisionWith(SphereCollider* sphereCollider, Manifold& manifold) override;
		virtual void CheckCollisionWith(ConvexCollider* convexCollider, Manifold& manifold) override;


		virtual void FillInManifold(std::shared_ptr<PhysicsCollider> otherPhysicsCollider, Manifold& manifold) override;

		virtual void FillInManifoldWith(ConvexCollider* convexCollider, Manifold& manifold) override;
		virtual void FillInManifoldWith(SphereCollider* sphereCollider, Manifold& manifold)  override;
		
		void SetMesh(std::shared_ptr<Mesh> mesh) override;

		const std::vector<HalfEdgeEdge*>& GetEdges();

		const std::vector< PhysicsMeshFace>& GetFaces();
		

		
	private:

		void initializeColliderMesh();

		void initializeHalfEdgeList();

		void initializeFaceList();

		
		//VertexIndexToHalfEdgePtr vertexIndexToHalfEdge;
		std::vector<HalfEdgeEdge*> edges;
		std::vector<PhysicsMeshFace> polyhedronFaces;

	};

}

