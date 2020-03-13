#pragma once
#include "Game.h"

namespace PXG
{
	class PXGGame : public Game
	{
	public :

		PXGGame();

		virtual void Start() override;

		virtual void Update() override;

		virtual void FixedUpdate(float tick)override;

		virtual void Initialize() override;


		GameObj MakeChild(const std::string& name);

		Font* font;
		FontRenderer* frender;
	};

}

