#pragma once
#include "GLMHeaders.h"
#include "Debug.h"
#include <string>
#include "Vector2.h"
#include "Mathf.h"
#include <math.h>
namespace PXG
{
	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3(float v3x, float v3y, float v3z)
		{
			x = v3x;
			y = v3y;
			z = v3z;
		}

		Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3(glm::vec3 GLMVec3)
		{
			x = GLMVec3.x;
			y = GLMVec3.y;
			z = GLMVec3.z;
		}

		Vector3 Normalized() const
		{
			return Vector3(glm::normalize(glm::vec3(x, y, z)));
		}

		Vector3& Normalize()
		{
			*this = Normalized();

			return *this;
		}

		Vector3 operator+ (const Vector3 &vec3) const
		{
			float newX = x + vec3.x;
			float newY = y + vec3.y;
			float newZ = z + vec3.z;

			return Vector3(newX, newY, newZ);
		}



		Vector3 operator- (const Vector3 &vec3) const
		{
			float newX = x - vec3.x;
			float newY = y - vec3.y;
			float newZ = z - vec3.z;

			return Vector3(newX, newY, newZ);
		}

		static friend Vector3 operator-(const Vector3& lhs)
		{
			return { -lhs.x,-lhs.y,-lhs.z };
		}
		static friend Vector3 operator-(Vector3&& lhs)
		{
			return { -lhs.x,-lhs.y,-lhs.z };
		}
		Vector3 operator* (float scalar) const
		{
			float newX = x * scalar;
			float newY = y * scalar;
			float newZ = z * scalar;

			return Vector3(newX, newY, newZ);
		}

		Vector3 operator/ (float scalar) const
		{
			float newX = x / scalar;
			float newY = y / scalar;
			float newZ = z / scalar;

			return Vector3(newX, newY, newZ);
		}

		Vector3& operator= (const Vector3 &vec3)
		{

			x = vec3.x;
			y = vec3.y;
			z = vec3.z;

			return *this;
		}

		Vector3& operator= (const glm::vec3& vec3)
		{
			x = vec3.x;
			y = vec3.y;
			z = vec3.z;

			return *this;
		}

		float& operator[](std::size_t location)
		{
			switch (location)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default:throw std::out_of_range("Vector3 subscript out of range!");
			}
		}

		float operator[](std::size_t location) const
		{
			switch (location)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default:throw std::out_of_range("Vector3 subscript out of range!");
			}
		}

		friend bool operator==(const Vector3& lhs, const Vector3& rhs)
		{
			return
				lhs.x == rhs.x &&
				lhs.y == rhs.y &&
				lhs.z == rhs.z;
		}

		friend bool operator!=(const Vector3& lhs, const Vector3& rhs)
		{
			return !(lhs == rhs);
		}

		static float Distance(Vector3 a, Vector3 b)
		{
			float length = 0;
			length = Mathf::Sqrt((a.x - b.x)*(a.x*b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z));
			return length;
		}

		float Length() const
		{
			return glm::length(glm::vec3(x, y, z));
		}
		float getAngle(Vector3 a, Vector3 b)
		{
			float angle = 0;
			float lengthA = a.Length();
			float lengthB = b.Length();
			float dot = Mathf::Dot(a, b);
			angle = atan2(b.z - a.z, b.x - a.x);
			angle = angle * 180 / glm::pi<float>()*2;
			return angle;
		}
		std::string ToString() const
		{
			return glm::to_string(glm::vec3(x, y, z));
		}

		glm::vec3 ToGLMVec3() const
		{
			return glm::vec3(x, y, z);
		}
		Vector2 discardZ(Vector3 vec3)
		{
			return Vector2(vec3.x, vec3.y);
		}
		Vector2 discardY(Vector3 vec3)
		{
			return Vector2(vec3.x, vec3.z);
		}
	};
}


