#pragma once
#include "HalfEdgeEdge.h"
#include "Vector3.h"

namespace PXG
{
	struct ClippingIterInfo
	{
		static const int precision = 4;

		Vector3 edgeWorldPosition;
		Vector3 nextEdgeWorldPosition;
		bool isEdgeIntersected;

		ClippingIterInfo(Vector3 edgeWorldPosition, Vector3 nextEdgeWorldPosition)
		{
			this->edgeWorldPosition = edgeWorldPosition;
			this->nextEdgeWorldPosition = nextEdgeWorldPosition;
			isEdgeIntersected = false;
		}

		/*bool operator<(const Test& t) const
		{
			return (this->id < t.id);
		}*/
	};
}

