#pragma once
#include "Vector3.h"
#include "HalfEdgeEdge.h"

namespace PXG
{
	struct PhysicsMeshFace
	{
		Vector3 normal;
		Vector3 position;

		std::vector<HalfEdgeEdge*> EdgesOnFace;

		void AddEdge(HalfEdgeEdge* newEdge)
		{
			EdgesOnFace.push_back(newEdge);
		}

		PhysicsMeshFace(HalfEdgeEdge* startingEdge)
		{
			EdgesOnFace.push_back(startingEdge);
			normal = *startingEdge->GetNormal();
			position = startingEdge->GetPosition();

		}



	};


}
