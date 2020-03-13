#include "TextComponent.h"
namespace PXG
{
	FontRenderer::render_queue render_queue;

	void TextComponent::setString(std::string  string)
	{
		this->Text = string;
		updateText();
	}

	void TextComponent::setFontSize(int size)
	{
		CharSize = size;
		updateText();
	}
	void TextComponent::setRelativePosition(Vector2 position)
	{
		this->offSet = Vector2(position.x - ScreenSize::WIDTH / 2, position.y - ScreenSize::HEIGHT / 2);
		Vector3 pScale = this->GetOwnerTransform()->getScale();
		offSet.x = offSet.x - pScale.x + 100;
		offSet.y = offSet.y - pScale.z + 100;
		updateText();
	}
	void TextComponent::FixedUpdate(float tick)
	{
		if (this->active)
		{
			updateText();
		}
	}
	void TextComponent::SetFont(Font * font)
	{
		this->font = font;
		updateText();
	}
	void TextComponent::InitText(FontRenderer * fRender)
	{

		this->frenderer = fRender;
		updateText();
		render_queue = frenderer->save_queue();

	}

	void TextComponent::isActive(bool active)
	{
		this->active = active;
	}

	void TextComponent::setStatic(bool isStatic)
	{
		this->isStatic = isStatic;
	}

	void TextComponent::updatePos()
	{
		Vector3 vec3 = GetOwnerTransform()->GetLocalPosition();
		Vector2 newPos = offSet + vec3.discardZ(vec3);
		position = newPos;
	}

	void TextComponent::updateText()
	{
		if (frenderer)
		{
			Vector2 newPos = position;
			if (GetOwner() && !isStatic)
			{
				updatePos();
			}
			frenderer->text(font, Text, CharSize, position.ToGLMVec2());
		}
	}
}