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

	enum class PrimitiveDrawingMode
	{
		POINT = GL_POINTS,
		LINE = GL_LINES,
		TRIANGLE = GL_TRIANGLES

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

		~Mesh()
		{
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			
			glDeleteVertexArrays(1, &VAO);

		}

		
		std::vector<unsigned int> Indices;
		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;

		void Draw(Shader* shader);

		static void SetRasterizationMode(RasterizationMode newRasterizationMode);

		static void SetPrimitiveDrawingMode(PrimitiveDrawingMode drawingMode);

		static std::shared_ptr<Mesh> InstantiateCubeMesh(Vector3 lowerV0, Vector3 lowerV1, Vector3 lowerV2, Vector3 lowerV3, Vector3 upperV0
			, Vector3 upperV1, Vector3 upperV2, Vector3 upperV3);

		Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures);
		Mesh();

		void SetupMesh();
	private:

		static GLenum primitiveDrawingMode;

		unsigned int VAO, VBO, EBO;


	};

}


