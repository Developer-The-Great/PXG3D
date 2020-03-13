#pragma once
#include "Vector3.h"
#include <vector>
#include "Texture.h"

namespace PXG
{
	class Shader;
	enum class RasterizationMode;

	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		glm::vec2 UV; //TODO make Vector2 Wrapper
	};

	class Mesh
	{
	public:

		Mesh(const Mesh& other)
		{
			std::copy(other.Indices.begin(), other.Indices.end(), std::back_inserter(Indices));
			std::copy(other.Vertices.begin(), other.Vertices.end(), std::back_inserter(Vertices));
		//	std::copy(other.Textures.begin(), other.Textures.end(), std::back_inserter(Textures));

			SetupMesh();
		}

		
		std::vector<unsigned int> Indices;
		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;

		void Draw(Shader* shader);

		static void SetRasterizationMode(RasterizationMode newRasterizationMode);

		Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures);
		Mesh();

		void SetupMesh();
	private:

		unsigned int VAO, VBO, EBO;


	};

}


