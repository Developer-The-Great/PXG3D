#pragma once
#include "World.h"
#include <list>

#include "Vector2.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Game.h"
#include "FileConfig.h"
#include "Subject.h"
#include "Subscriber.h"
namespace PXG
{
	class Canvas : public World
	{
	public:
		Canvas();

		GameObj createCanvasObject(Vector2 position, Vector2 dimensions,
										std::string name, std::string textureName);
		GameObj createEmptyCanvasObject();
	};
}


