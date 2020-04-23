#pragma once
#include "Mesh.h"

namespace PXG
{
	struct HalfEdgeEdge
	{
		Vertex* vert = nullptr;
		HalfEdgeEdge * pairingEdge = nullptr;
		HalfEdgeEdge * nextEdge = nullptr;

		Vector3 GetEdgeDirection()
		{
			return nextEdge->vert->position - vert->position;
		}

		Vector3* GetNormal()
		{
			if (vert)
			{
				return &vert->normal;
			}
			return nullptr;
		}

		Vector3* GetPairingNormal()
		{
			if (pairingEdge)
			{

				return pairingEdge->GetNormal();
			}
			return nullptr;
		}

	};
}

