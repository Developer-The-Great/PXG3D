#pragma once
#include "Vector3.h"

namespace PXG
{
	struct DepthPenetrationInfo
	{
		Vector3 seperationPlanePosition;
		Vector3 seperationPlaneNormal;
		float penetration;
		bool isColliderARef;

		DepthPenetrationInfo()
		{

		}

		DepthPenetrationInfo(Vector3& seperationPlanePosition,Vector3& seperationPlaneNormal,float penetration, bool isColliderARef)
		{
			this->seperationPlanePosition = seperationPlanePosition;
			this->seperationPlaneNormal = seperationPlaneNormal;
			this->penetration = penetration;
			this->isColliderARef = isColliderARef;
		}

		bool operator< ( DepthPenetrationInfo const& b)
		{ 
			return this->penetration < (b.penetration);
		}

	};
}
