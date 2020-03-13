#pragma once
#include "GLMHeaders.h"
#include "AxisAngle.h"
#include "Mathf.h"

namespace PXG
{
	struct  Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		Quaternion(float qw, float qx, float qy, float qz)
		{
			w = qw;
			x = qx;
			y = qy;
			z = qz;
		}

		Quaternion()
		{
			w = 1;
			x = 0;
			y = 0;
			z = 0;
		}

		Quaternion(glm::quat Quat)
		{
			x = Quat.x;
			y = Quat.y;
			z = Quat.z;
			w = Quat.w;
		}

		Quaternion operator/(const float scalar) const
		{
			float newW = w / scalar;
			float newX = x / scalar;
			float newY = y / scalar;
			float newZ = z / scalar;

			return Quaternion(newW,newX,newY,newZ);
		}

		Quaternion operator*(const float& scalar) const
		{
			float newW = w * scalar;
			float newX = x * scalar;
			float newY = y * scalar;
			float newZ = z * scalar;
			return Quaternion(newW, newX, newY, newZ);
		}

		Quaternion operator*(const Quaternion& otherQuat) const
		{
			return Quaternion(glm::quat(w, x, y, z) * glm::quat(otherQuat.w, otherQuat.x, otherQuat.y, otherQuat.z));
		}

		Quaternion& operator=(const glm::quat& quat)
		{
			w = quat.w;
			x = quat.x;
			y = quat.y;
			z = quat.z;

			return *this;
		}

		float Length() const
		{
			return glm::length(glm::quat(w, x, y, z));
		}

		Quaternion Normalized() const
		{
			return glm::normalize(glm::quat(w, x, y, z));
		}

		AxisAngle ToAxisAngle() const
		{

			AxisAngle Result;
			Result.angle = 2 * Mathf::ACos(w);
			Result.x = x / Mathf::Sqrt(1 - w * w);
			Result.y = y / Mathf::Sqrt(1 - w * w);
			Result.z = z / Mathf::Sqrt(1 - w * w);

			return Result;
		}

		std::string ToString() const
		{
			return glm::to_string(glm::quat(w, x, y, z));
		}

		glm::quat ToGLMQuat() const
		{
			return glm::quat(w, x, y, z);
		}

		static Quaternion GetIdentity()
		{
			return Quaternion();
		}

	};
}
