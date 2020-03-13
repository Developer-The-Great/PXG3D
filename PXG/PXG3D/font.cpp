#include "font.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include "Debug.h"
#include "Shader.h"
#include "Vector3.h"
#include "Mat4.h"


namespace PXG {


	const char* fragment_shader = R"(
	#version 330 core
	in vec2 TexCoords;
	out vec4 color;

	uniform sampler2D text;
	uniform vec3 textColor;

	void main()
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
		color = vec4(textColor, 1.0) * sampled;


	})";


	const char* vertex_shader = R"(
	#version 330 core
	layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
	out vec2 TexCoords;

	uniform mat4 projection;

	void main()
	{
	    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	    TexCoords = vertex.zw;
	})";


class font_error : std::runtime_error
{
public: font_error(std::string message) :std::runtime_error(message + "@" + PXG::FONT_EXCEPTION_TAG) {}
};


PXG::Font::Font(std::string  filepath, int pensize)
{
	//check if the ft library is initialized
	if (m_library == nullptr)
	{
		m_library = new FT_Library;
		const FT_Error error = FT_Init_FreeType(m_library);
		if (error)
		{
#undef FTERRORS_H
#define FT_ERRORDEF( e,v,s ) case e: throw font_error(s);
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H
		}
	}

	//constructed the font
	m_face = new FT_Face;

	const FT_Error error = FT_New_Face(*m_library, filepath.c_str(), 0, m_face);


	//const FT_Error error = FT_New_Memory_Face(*m_library, font_data.data(), font_data.size(), 0, m_face);

	if (error)
	{
#undef FTERRORS_H
#define FT_ERRORDEF( e,v,s ) case e: throw font_error(s);
#define FT_ERROR_START_LIST switch(error) {
#define FT_ERROR_END_LIST }
#include FT_ERRORS_H
	}

	//set the size of the font

	FT_Set_Pixel_Sizes(*m_face, pensize, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	//generate the charset
	for (GLubyte c = 0; c < 128; c++)
	{
		FT_Error err = 0;
		if ((err = FT_Load_Char(*m_face, c, FT_LOAD_RENDER)) != 0)
		{
			Debug::Log(Verbosity::Warning, "defective font char: {0} was not loaded", c);
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			(*m_face)->glyph->bitmap.width,
			(*m_face)->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			(*m_face)->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2((*m_face)->glyph->bitmap.width, (*m_face)->glyph->bitmap.rows),
			glm::ivec2((*m_face)->glyph->bitmap_left, (*m_face)->glyph->bitmap_top),
			static_cast<GLuint>((*m_face)->glyph->advance.x)
		};
		m_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	FT_Done_Face(*m_face);
	delete m_face;
}

//use a different shader for the font renderer
void FontRenderer::select_shader(Shader* sh)
{
	if (m_shader && m_is_default_shader) delete m_shader;
	m_shader = sh;
	m_is_default_shader = false;
}

//use default shader for rendering (see graphics/impl)
void FontRenderer::use_default_shader()
{
	m_is_default_shader = true;

	m_shader = new Shader(vertex_shader,fragment_shader,"__internal_font_shader",true);
}


//add text to the render queue
void FontRenderer::text(Font* fnt, std::string tx, float scale, glm::vec2 position)
{
	//iterate over each char in the map
	for (std::string::const_iterator c = tx.begin(); c != tx.end(); ++c)
	{
		//get the corresponing font character
		const auto ch = fnt->m_characters[*c];

		//calculate position of char
		const GLfloat xpos = position.x + ch.Bearing.x * scale;
		const GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		//calculate width and height of char
		const GLfloat w = ch.Size.x * scale;
		const GLfloat h = ch.Size.y * scale;

		//prepare vertex buffers
		std::shared_ptr<GLfloat> vertices = std::shared_ptr<GLfloat>(new GLfloat[6 * 4]{
				xpos,     ypos + h,   0.0, 0.0 ,
				xpos,     ypos,       0.0, 1.0 ,
				xpos + w, ypos,       1.0, 1.0 ,

				xpos,     ypos + h,   0.0, 0.0 ,
				xpos + w, ypos,       1.0, 1.0 ,
				xpos + w, ypos + h,   1.0, 0.0
			}, [](const GLfloat* f) { delete[] f; });

		//inset the buffers into the render_queue
		m_renderQueue.insert(std::make_pair(ch.TextureID, vertices));

		//advance by one
		position.x += (ch.Advance >> 6)* scale;
	}
}

//save the render_queue for reuse (the draw will clear it)
FontRenderer::render_queue FontRenderer::save_queue() const
{
	return m_renderQueue;
}

//load a render_queue and replace the old one
void FontRenderer::restore_queue(render_queue* queue)
{
	m_renderQueue = *queue;
}

//load a render_queue and add to the existing one
void FontRenderer::add_queue(render_queue* queue)
{
	m_renderQueue.insert(queue->begin(), queue->end());
}

//clear the render_queue prematurely
void FontRenderer::delete_queue()
{
	m_renderQueue.clear();
}

//draw the render_queue
void FontRenderer::draw(const glm::mat4& projection, glm::vec3 color)
{


	if (!m_shader) throw font_error("provided shader is invalid !");

	//bind the shader
	m_shader->Use();

	//upload projection
	m_shader->SetMat4("projection", Mat4(projection));

	//upload text color
	m_shader->SetVec3("textColor", Vector3(color));

	//bind active texture unit and Vertex Arrays
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	//iterate over each position in the map (note that the map automagically groups the same characters)
	decltype(m_renderQueue.equal_range(0)) range;
	for (auto i = m_renderQueue.begin(); i != m_renderQueue.end(); i = range.second)
	{
		range = m_renderQueue.equal_range(i->first);

		//bind the texture for this specific character
		glBindTexture(GL_TEXTURE_2D, i->first);


		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		//get number of Chars in range
		std::size_t range_size = m_renderQueue.count(i->first);

		//orphan buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4 * range_size, NULL, GL_DYNAMIC_DRAW);

		//get buffer into physical space
		GLfloat* buffer = reinterpret_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		//copy all char vertex datas into buffer
		int j = 0;
		for (auto d = range.first; d != range.second; ++d)
		{
			memcpy(&buffer[6 * 4 * j++], d->second.get(), 6 * 4 * sizeof(GLfloat));
		}

		//release buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//draw all chars in one go
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(6 * range_size));

	}

	//clear render state
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_shader->Release();

	//delete all drawn events
	m_renderQueue.clear();
}

//ineffective method for imediate text drawing
void FontRenderer::draw_text(glm::vec3 color, std::string text, glm::vec2 position, GLfloat scale, Font* fnt) const
{
	// Activate corresponding render stateS
	m_shader->Use();
	glUniform3f(glGetUniformLocation(m_shader->GetShaderProgram(), "textColor"), color.r, color.g, color.b);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Font::Character ch = fnt->m_characters[*c];

		GLfloat xpos = position.x + ch.Bearing.x * scale;
		GLfloat ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//static FTLib
FT_Library* Font::m_library = nullptr;

}
