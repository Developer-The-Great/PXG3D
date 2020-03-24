#pragma once

#define PXG_THIS_FRAME_ONLY 0.0f
#include <memory>
#include <list>
#include "Vector3.h"
namespace PXG
{
	class DebugMeshObject;

	class Mesh;
	class World;

	enum class DebugPrimitive
	{
		LINE,
		CUBE,

	};

	class DebugDrawingManager
	{
	public:

		/**@brief Creates a line with a certain start and end with a certain color and lifetime
		 * @param [in] start: the start point of the line 
		 * @param [in] end: the end point of the line
		 * @param [in] color: the color that line will be rendered (e.g Vector3(1,0,0) would result in a red line)
		 * @param [in] lifetime: how long the line will exist(in seconds)
	    */ 
		void InstantiateLine(Vector3 start, Vector3 end, Vector3 color, float lifetime = PXG_THIS_FRAME_ONLY);

		/**@brief Creates a cube that has vertices that span from 'min' to 'max' at 'position' 
		 * @param [in] position: the start point of the line
		 * @param [in] min: the end point of the line, specified in object space
		 * @param [in] max: the color that line will be rendered, specified in object space
		 * @param [in] lifetime: how long the line will exist(in seconds)
		 * @param [in] vertexOffset: the offset in which the vertex positions of the cube will be shifted towards.
		*/
		void InstantiateCube(Vector3 position, Vector3 min, Vector3 max, Vector3 color, float lifetime = PXG_THIS_FRAME_ONLY, Vector3 vertexOffset = Vector3(0, 0, 0));

		void DrawDebugObjects();

		void DecreaseLifespan(float timepast);

		void RemoveDeadDebugMeshes();

		void SetWorld(std::weak_ptr<World> world);

		std::weak_ptr<World> GetWorld();
		

	private:
		
		std::weak_ptr<World> world;
		std::list<std::shared_ptr<DebugMeshObject>> debugMeshObjects;

	};
}


