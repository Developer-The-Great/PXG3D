#pragma once
#include "HalfEdgeEdge.h"
#include "Vector3.h"

namespace PXG
{
	struct ClippingIterInfo
	{
		HalfEdgeEdge * edge;
		std::shared_ptr<std::array<Vector3, 2>> clippingPoints;


		ClippingIterInfo(HalfEdgeEdge * edge, std::shared_ptr<std::array<Vector3, 2>> clippinPoints)
		{
			this->edge = edge;
			this->clippingPoints = clippingPoints;

		}
	};
}

