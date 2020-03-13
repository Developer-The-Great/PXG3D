#pragma once
#include "GLMHeaders.h"
#include "Debug.h"
#include <string>

namespace PXG
{
	struct Vector2
	{
		float x;
		float y;

		Vector2(float v2x, float v2y)
		{
			x = v2x;
			y = v2y;
		}

		Vector2()
		{
			x = 0;
			y = 0;
		}

		Vector2(glm::vec2 GLMVec2)
		{
			x = GLMVec2.x;
			y = GLMVec2.y;
		}

		Vector2 Normalized() const
		{
			return Vector2(glm::normalize(glm::vec2(x, y)));
		}

		Vector2& Normalize()
		{
			*this = Normalized();

			return *this;
		}

		Vector2 operator+ (const Vector2 &vec2) const
		{
			float newX = x + vec2.x;
			float newY = y + vec2.y;

			return Vector2(newX, newY);
		}

		Vector2 operator*(const int &scaler)
		{
			float newX = x *= scaler;
			float newY = y *= scaler;
			return Vector2(newX, newY);
		}

		Vector2 operator- (const Vector2 &vec2) const
		{
			float newX = x - vec2.x;
			float newY = y - vec2.y;

			return Vector2(newX, newY);
		}

		Vector2 operator* (const float &scalar) const
		{
			float newX = x * scalar;
			float newY = y * scalar;

			return Vector2(newX, newY);
		}

		Vector2 operator/ (const float &scalar) const
		{
			float newX = x / scalar;
			float newY = y / scalar;

			return Vector2(newX, newY);
		}

		Vector2& operator= (const Vector2 &vec2)
		{

			x = vec2.x;
			y = vec2.y;

			return *this;
		}

		Vector2& operator= (const glm::vec2& vec2)
		{
			x = vec2.x;
			y = vec2.y;

			return *this;
		}

		float& operator[](std::size_t location)
		{
			switch (location)
			{
			case 0: return x;
			case 1: return y;
			default:throw std::out_of_range("Vector2 subscript out of range!");
			}
		}

		float operator[](std::size_t location) const
		{
			switch (location)
			{
			case 0: return x;
			case 1: return y;
			default:throw std::out_of_range("Vector2 subscript out of range!");
			}
		}

		float Length() const
		{
			return glm::length(glm::vec2(x, y));
		}

		std::string ToString() const
		{
			return glm::to_string(glm::vec2(x, y));
		}

		glm::vec2 ToGLMVec2() const
		{
			return glm::vec2(x, y);
		}

	};
}


