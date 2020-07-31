#pragma once
#include "Mesh.h"

namespace PXG
{
	struct HalfEdgeEdge
	{
		Vertex* vert = nullptr;
		HalfEdgeEdge * pairingEdge = nullptr;
		HalfEdgeEdge * nextEdge = nullptr;
		int index;
		

		HalfEdgeEdge(int uniqueIndex)
		{
			index = uniqueIndex;
		}

		/*public HalfEdgeEdge()
		{

		}*/
		int GetNextIndex()
		{
			return nextEdge->index;
		}

		Vector3 GetEdgeDirection()
		{
			return GetNextEdgePosition() - GetPosition();
		}

		Vector3 GetPosition() const
		{
			return vert->position;
		}

		Vector3 GetNextEdgePosition()
		{
			return nextEdge->vert->position;
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

		bool isPairingEqualNormal()
		{
			return Mathf::FloatVectorCompare(*GetNormal(), *GetPairingNormal());
		}

	};
}

