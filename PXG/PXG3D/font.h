/*
 Copyright 2018-2020 Raphael Baier
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included
	in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
 */


#pragma once

#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <glm/mat4x2.hpp>
#include <spdlog/fmt/bundled/format.h>
#include "Shader.h"

namespace PXG {

	class Shader;

	const std::string FONT_EXCEPTION_TAG = "Fonts";

class Font
{
public:
	Font(std::string filepath, int pensize = 16);

	struct Character
	{
		GLuint		TextureID;
		glm::ivec2	Size;
		glm::ivec2	Bearing;
		GLuint		Advance;
	};

private:
	friend class FontRenderer;
	static FT_Library* m_library;
	FT_Face* m_face = nullptr;
	std::map<char, Character> m_characters;

};

class FontRenderer
{
public:

	typedef std::multimap<GLuint, std::shared_ptr<GLfloat>> render_queue;

	FontRenderer(bool provideShader = true)
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		if (provideShader) use_default_shader();
	}
	~FontRenderer()
	{
		if (m_is_default_shader)
			delete m_shader;
	}


	void select_shader(Shader* sh);
	void use_default_shader();

	void text(Font* fnt, std::string tx, float scale,glm::vec2 position);


	template <typename ... Args>
	void text(Font* fnt, std::string tx, float scale,glm::vec2 position, Args&& ... args);

	render_queue save_queue() const;
	void restore_queue(render_queue* queue);
	void add_queue(render_queue* queue);

	void delete_queue();


	void draw(const glm::mat4& projection, glm::vec3 color);
	void draw_text(glm::vec3 color, std::string text, glm::vec2 position, GLfloat scale, Font* fnt) const;

private:
	Shader* m_shader;
	bool m_is_default_shader = false;


	GLuint m_VBO, m_VAO;

	void text_fmt_impl(Font* fnt, std::string tx, float scale,glm::vec2 position, fmt::format_args args)
	{
		text(fnt, fmt::vformat(tx, args), scale, std::forward<glm::vec2>(position));
	}
	render_queue m_renderQueue;

};

template <typename ... Args>
void FontRenderer::text(Font* fnt, std::string text, float scale, glm::vec2 position,
	Args&&... args)
{
	text_fmt_impl(fnt, text, scale, std::forward<glm::vec2>(position), fmt::make_format_args(args...));
}


}
