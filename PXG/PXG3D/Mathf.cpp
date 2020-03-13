#include "Mathf.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <iostream>


namespace PXG
{
	const float Mathf::Deg2Rad = 0.0174532f;
	const float Mathf::Epsilon = FLT_EPSILON;
	const float Mathf::Rad2Deg = 57.295779f;

	//TODO replace GLM functions with custom functions

	bool Mathf::FloatCompare(float a, float b)
	{
		return glm::abs(a - b) < Epsilon;
	}

	bool Mathf::FloatVectorCompare(Vector3 a, Vector3 b)
	{
		return FloatCompare(a.x, b.x) && FloatCompare(a.y, b.y) && FloatCompare(a.z, b.z);
	}

	float Mathf::Abs(float x)
	{
		return glm::abs(x);
	}

	float Mathf::ACos(float x)
	{
		return glm::acos(x);
	}

	float Mathf::Clamp(float value, float min, float max)
	{
		if (value < min) { return min; }
		if (value > max) { return max; }

		return value;
	}

	float Mathf::Cos(float radians)
	{
		return glm::cos(radians);
	}

	Vector3 Mathf::Cross(Vector3 a, Vector3 b)
	{
		return glm::cross(glm::vec3(a.x, a.y, a.z), glm::vec3(b.x, b.y, b.z));
	}

	float Mathf::Dot(Vector3 a, Vector3 b)
	{
		return glm::dot(glm::vec3(a.x, a.y, a.z), glm::vec3(b.x, b.y, b.z));
	}

	float Mathf::Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	Vector3 Mathf::Lerp(Vector3 a, Vector3 b, float t)
	{
		return a * (1 - t) + b * t;
	}

	Vector3 Mathf::Slerp(Vector3 a, Vector3 b, float t)
	{
		return Vector3();
	}

	Quaternion Mathf::Slerp(Quaternion a, Quaternion b, float t)
	{
		glm::quat A(a.w, a.x, a.y, a.z);
		glm::quat B(b.y, b.x, b.y, b.z);

		return glm::slerp(A, B, t);
	}

	float Mathf::Sqrt(float x)
	{
		return glm::sqrt(x);
	}

	Quaternion Mathf::ToQuaternion(AxisAngle axisAngle)
	{
			Vector3 axis(axisAngle.x, axisAngle.y, axisAngle.z);
			float angle = axisAngle.angle;
			axis.Normalize();

			float qx = axis.x * Mathf::Sin(glm::radians(angle / 2.0f));
			float qy = axis.y * Mathf::Sin(glm::radians(angle / 2.0f));
			float qz = axis.z * Mathf::Sin(glm::radians(angle / 2.0f));
			float qw = Mathf::Cos(glm::radians(angle / 2.0f));

			Quaternion result = Quaternion(qw, qx, qy, qz).Normalized();

			return result;

	}

	//TODO implement quat angle difference
	float Mathf::GetQuaternionAngleDifference(Quaternion a, Quaternion b)
	{
		//get axis angle representation of a and b

		//get vector c, a vector perpendicular to the forward of a and b

		//construct 2 new axis angle orientations. one orientation (A') where  c is forward and a is up and another one (B')
		//where c is forward and b is up

		//return the angle difference between A' and B'
		return -1.0f;
	}

	Quaternion Mathf::LookRot(Vector3 Forward, Vector3 Up)
	{

		Vector3 right = Cross(Up, Forward).Normalized();

		glm::mat3 GLMMat3(right.ToGLMVec3(), Up.ToGLMVec3(), Forward.ToGLMVec3());

		Quaternion Result = glm::normalize(glm::toQuat(GLMMat3));

		return Result;
	}

	float Mathf::Map(float value, float min1, float max1, float min2, float max2)
	{
		return min2 + ((value - min1) / (max1 - min1)) * (max2 - min2);
	}

	float Mathf::Pow(float value, float pow)
	{
		return glm::pow(value, pow);
	}

	Vector3 Mathf::RotateVectorAroundAxis(Vector3 toRotate, Vector3 axis, float angle)
	{
		//TODO needs unit test
		glm::quat Quat = glm::quat(0, toRotate.ToGLMVec3());

		float halfSinValue = 0.5f * Sin(glm::radians(angle));
		float halfCosValue = 0.5f * Cos(glm::radians(angle));

		glm::quat rotatationquat = glm::quat(halfCosValue, axis.ToGLMVec3() * halfSinValue);

		Quat = rotatationquat * Quat * glm::conjugate(rotatationquat);

		Quat = glm::normalize(Quat);

		Vector3 result(Quat.x, Quat.y, Quat.z);

		return result;

	}

	float Mathf::Sin(float radians)
	{
		return glm::sin(radians);
	}

}
