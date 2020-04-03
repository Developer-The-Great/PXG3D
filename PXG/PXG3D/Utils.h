#pragma once
#define OUT
#define SCREEN_WIDTH 
#include <memory>

namespace PXG
{
	class World;
	struct Vector3;

	class Utils
	{
	public:

		/**@brief : Gets the world position of a pixel at (i,j) in a projection camera that has an aspect ration of screenWidth:screenHeight
		* @param [in] i : the location of the pixel in the x axis
		* @paran [in] j : the location of the pixel in the y axis
		* @paran [in] world : the world where this calculation will take place
		*/
		static Vector3 GetProjectionCameraScreenToWorld(float i, float j, std::shared_ptr<World> world);

		//returns the world position of pixel (i,j) in a camera with a screen width of screenWidth and a screen height of screenHeight.
		static Vector3 GetOrthographicCameraWorldPosition(float i, float j, float screenWidth, float screenHeight, std::shared_ptr<World> world);



	};



}