#pragma once

#include <string>
#include <glad/glad.h>
#include "Debug.h"

#include "stb_image.h"
#include <memory.h>

namespace PXG
{
	enum class TextureType
	{
		DIFFUSE,
		SPECULAR
	};
	struct Texture
	{
		Texture(std::string path, TextureType type)
		{
			id = GenerateTexture(path);

			switch (type)
			{
			case PXG::TextureType::DIFFUSE:
				this->type = "texture_diffuse";
				break;
			case PXG::TextureType::SPECULAR:
				this->type = "texture_specular";
				break;
			default:
				break;
			}

		}

		Texture(){}

		unsigned int id;
		std::string type;
		std::string path;

		//path
		static int GenerateTextureID(std::string path,std::string directory)
		{
			return GenerateTexture(directory + '/' + path);
		}

		static int GenerateTexture(std::string path)
		{
			//filename = directory + '/' + filename;

			Debug::Log(Verbosity::Info, "TextureID Filename {0} ", path);

			unsigned int TextureID;

			glGenTextures(1, &TextureID);

			int width, height, nrComponents;
			//get image data
			unsigned char * data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

			if (data)
			{
				GLenum format;

				if (nrComponents == 1)
				{
					format = GL_RED;
				}
				else if (nrComponents == 3)
				{
					format = GL_RGB;
				}
				else if (nrComponents == 4)
				{
					format = GL_RGBA;
				}

				glBindTexture(GL_TEXTURE_2D, TextureID);

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);

			}
			else
			{
				Debug::Log("ERROR::TEXTURE::Cannot generate image data");

				stbi_image_free(data);
			}

			return TextureID;
		}

	};
}