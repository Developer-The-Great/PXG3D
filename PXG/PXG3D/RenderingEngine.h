#pragma once
#include <memory>
#include "MeshComponent.h"
#include "font.h"

namespace PXG
{
	class World;
	class Canvas;
	class RenderingEngine
	{
	public:

		RenderingEngine();

		void AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent);

		void RenderCurrentlySetWorld() const;

		void RenderCanvas() const;

		//void EnableDepthTest()
		void SetWorld(std::shared_ptr<World> world);

		void SetCanvas(std::shared_ptr<Canvas> canvas);

		FontRenderer& GetFontRenderer()
		{
			if (!fontRenderer) fontRenderer = std::make_shared<FontRenderer>(true);
			return *fontRenderer;
		}

	private:
		std::shared_ptr<FontRenderer> fontRenderer;
		std::shared_ptr<World> world = nullptr;
		std::shared_ptr<Canvas> canvas = nullptr;

	};
}


