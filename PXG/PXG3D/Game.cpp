#include "Game.h"
#include "Canvas.h"
#include "World.h"
#include "PhysicsComponent.h"
namespace PXG
{
	Game::Game()
	{
		world = std::make_shared<World>();
		canvas = std::make_shared<Canvas>();

	}

	std::shared_ptr<World> Game::GetWorld() const
	{
		return world;
	}

	std::shared_ptr<Canvas> Game::GetCanvas() const
	{
		return canvas;
	}

	void Game::SetCamera()
	{
	}

	void Game::Initialize()
	{
	}


	GameObj Game::Instantiate() const
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		newGameObject->InitializeComponentOwners();
		newGameObject->SetWorld(world);
		return newGameObject;
	}
	GameObj Game::InstantiateUIObject() const
	{
		GameObj newGameObject = std::make_shared<GameObject>();

		newGameObject->SetWorld(canvas);
		return newGameObject;
	}
}

