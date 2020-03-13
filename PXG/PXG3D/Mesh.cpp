#include "Mesh.h"
#include <glad/glad.h>
#include "Shader.h"

namespace PXG
{
	//std::string Mesh::textureDiffuseStr = "texture_diffuse";
	//std::string Mesh::textureSpecularStr = "texture_specular";
	Mesh::Mesh(std::vector<unsigned int> indices, std::vector<Vertex> vertices, std::vector<Texture> textures)
	{
		
		this->Textures = textures;
		this->Indices = indices;
		this->Vertices = vertices;
		SetupMesh();
	}

	Mesh::Mesh()
	{

	}

	void Mesh::Draw(Shader* shader)
	{
		unsigned int diffuseCount = 1;
		unsigned int specularCount = 1;
		//assume shader.Use() was already called before calling mesh.Draw()
		for (unsigned int i = 0; i < Textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = Textures[i].type;


			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseCount);
				diffuseCount++;

			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularCount);
				specularCount++;
			}
			//std::cerr << "LOG::Mesh::Draw::using texture " << ("material." + name + number).c_str() << std::endl;

			//int test = glGetUniformLocation(shader.shaderID, ("material." + name + number).c_str());
			//std::cerr << "DEBUG::Mesh::Draw::uniform found? " << test << std::endl;

			shader->SetInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, Textures[i].id);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::SetRasterizationMode(RasterizationMode newRasterizationMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (int)newRasterizationMode);
	}

	void Mesh::SetupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,UV));

		glBindVertexArray(0);

	}
}

