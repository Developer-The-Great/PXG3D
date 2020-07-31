#pragma once
#include "Vector3.h"

namespace PXG
{
	struct ContactPointPenetrationInfo
	{
		Vector3* contactPoint;
		float penetration;

		ContactPointPenetrationInfo(Vector3* contactPointPtr, float penetration)
		{
			this->contactPoint = contactPointPtr;
			this->penetration = penetration;
		}

		bool operator<(const ContactPointPenetrationInfo& other) const
		{
			return penetration < other.penetration;

		}
		


	};
}
