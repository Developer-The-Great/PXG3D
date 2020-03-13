#pragma once
#include "GLMHeaders.h"
#include <string>
#include "Mathf.h"
#include "Vector3.h"
namespace PXG
{
	struct Quaternion;

	struct AxisAngle
	{
		float angle;
		float x;
		float y;
		float z;

		AxisAngle()
		{
			angle = 0;
			x = 0;
			y = 0;
			z = 1;
		}

		AxisAngle(float aAngle, float ax, float ay, float az)
		{
			angle = aAngle;
			x = ax;
			y = ay;
			z = az;
		}

		Quaternion ToQuaternion() const;

		std::string ToString() const
		{
			return "AxisAngle(Angle: " + std::to_string(angle) + " ,X: " + std::to_string(x) + " ,Y: " + std::to_string(y) + " ,Z: " + std::to_string(z);
		}


	};

}

