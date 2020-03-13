#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "UIObject.h"

typedef std::shared_ptr<PXG::GameObject> GameObj;
typedef std::shared_ptr<PXG::UIObject> UIObj;

namespace PXG
{
	class World;
	class Canvas;
	class Game
	{


	public:

		Game();

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void FixedUpdate(float tick) = 0;

		std::shared_ptr < World>  GetWorld() const;
		std::shared_ptr < Canvas>  GetCanvas() const;

		void SetCamera();

		virtual void Initialize();

		GameObj Instantiate() const;
		GameObj InstantiateUIObject() const;
	protected:
		std::shared_ptr < World> world;
		std::shared_ptr < Canvas> canvas;
	};


}
