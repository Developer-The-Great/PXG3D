#pragma once
#include "Component.h"
#include<string>
#include "Vector2.h"
#include "Vector3.h"
#include "Game.h"
#include "Transform.h"
#include "font.h"
#include "ScreenSize.h"

namespace PXG
{
	class TextComponent :public Component, public std::enable_shared_from_this<TextComponent>
	{
	public:

		void setString(std::string string);
		void setFontSize(int size);
		void setRelativePosition(Vector2 position);
		void FixedUpdate(float tick) override;
		void SetFont(Font* font);
		void InitText(FontRenderer* fRender);
		void isActive(bool active);
		void setStatic(bool isStatic);
	private:
		void updatePos();
		void updateText();
		Vector2 position = Vector2(-400, -300);
		Vector2 offSet = Vector2(0, 0);
		std::string Text;
		int CharSize=1;
		Font* font;
		FontRenderer* frenderer;
		bool active = true;
		bool isStatic = false;
	};


}