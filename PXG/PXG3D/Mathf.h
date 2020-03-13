#pragma once
#include <float.h>




namespace PXG
{
	struct Quaternion;
	struct Vector3;
	struct AxisAngle;

	//Contains commonly used mathematical functions. Also contains 3D math related functions
	class Mathf
	{
	public:

		static const float Deg2Rad;
		static const float Epsilon;
		static const float Rad2Deg;

		//returns the absolute value of an int
		static float Abs(float x);
		
		static float ACos(float x);

		static float Clamp(float value, float min, float max);

		static float Cos(float radians);
		//returns the cross of a and b
		static Vector3 Cross(Vector3 a, Vector3 b);
		//returns the dot product of a and b
		static float Dot(Vector3 a, Vector3 b);
		//compares 2 floats
		static bool FloatCompare(float a, float b);

		static bool FloatVectorCompare(Vector3 a, Vector3 b);

		static float GetQuaternionAngleDifference(Quaternion a, Quaternion b);

		static float Lerp(float a, float b, float t);

		//returns a Vector3 that is linearly interpolated between a and b with an interpolant of t
		static Vector3 Lerp(Vector3 a, Vector3 b, float t);

		//Warning : Assumes that given forward and up are orthonormal
		static Quaternion LookRot(Vector3 Forward, Vector3 Up);

		//maps value that initially has a range of min1 and max to having a range of min2 to max2
		static float Map(float value, float min1, float max1, float min2, float max2);
		//returns value to the power of pow
		static float Pow(float value, float pow);
		//rotate a vector around a given axis and angle
		static Vector3 RotateVectorAroundAxis(Vector3 toRotate, Vector3 axis, float angle);
		//returns the sine value of radians
		static float Sin(float radians);

		static Vector3 Slerp(Vector3 a, Vector3 b, float t);
		//spherically interpolates a and b with an interpolant of t 
		static Quaternion Slerp(Quaternion a, Quaternion b, float t);

		static float Sqrt(float x);

		static Quaternion ToQuaternion(AxisAngle axisAngle);


	};

}
