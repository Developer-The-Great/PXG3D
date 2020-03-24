#include "Mesh.h"
#include <glad/glad.h>
#include "Shader.h"

namespace PXG
{

	GLenum Mesh::primitiveDrawingMode = (GLenum)PrimitiveDrawingMode::TRIANGLE;
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
		glDrawElements(primitiveDrawingMode, Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::SetRasterizationMode(RasterizationMode newRasterizationMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (int)newRasterizationMode);
	}

	void Mesh::SetPrimitiveDrawingMode(PrimitiveDrawingMode drawingMode)
	{
		switch (drawingMode)
		{
		case PrimitiveDrawingMode::LINE:
			primitiveDrawingMode = GL_LINES;
			break;

		case PrimitiveDrawingMode::POINT:
			primitiveDrawingMode = GL_POINTS;

		case PrimitiveDrawingMode::TRIANGLE:
			primitiveDrawingMode = GL_TRIANGLES;
		default:
			break;
		}

	}

	std::shared_ptr<Mesh> Mesh::InstantiateCubeMesh(Vector3 lowerV0, Vector3 lowerV1, Vector3 lowerV2, Vector3 lowerV3, Vector3 upperV0, Vector3 upperV1, Vector3 upperV2, Vector3 upperV3)
	{

		std::shared_ptr<Mesh> cube = std::make_shared<Mesh>();

		//---------------------Create top and bottom face--------------------------//
		Vector3 topNormal = Vector3(0.0, 1.0, 0.0);
		Vector3 bottomNormal = Vector3(0.0, -1.0, 0.0);

		{
			//top face
			Vertex topFaceTriangle1V0;
			topFaceTriangle1V0.position = upperV0;
			topFaceTriangle1V0.normal = topNormal;

			Vertex topFaceTriangle1V1;
			topFaceTriangle1V1.position = upperV1;
			topFaceTriangle1V1.normal = topNormal;

			Vertex topFaceTriangle1V2;
			topFaceTriangle1V2.position = upperV3;
			topFaceTriangle1V2.normal = topNormal;

			Vertex topFaceTriangle2V0;
			topFaceTriangle2V0.position = upperV0;
			topFaceTriangle2V0.normal = topNormal;

			Vertex topFaceTriangle2V1;
			topFaceTriangle2V1.position = upperV3;
			topFaceTriangle2V1.normal = topNormal;

			Vertex topFaceTriangle2V2;
			topFaceTriangle2V2.position = upperV2;
			topFaceTriangle2V2.normal = topNormal;

			//top face push_back
			cube->Vertices.push_back(topFaceTriangle1V0);
			cube->Vertices.push_back(topFaceTriangle1V1);
			cube->Vertices.push_back(topFaceTriangle1V2);

			cube->Vertices.push_back(topFaceTriangle2V0);
			cube->Vertices.push_back(topFaceTriangle2V1);
			cube->Vertices.push_back(topFaceTriangle2V2);
		}

		{
			//bottom face
			Vertex bottomFaceTriangle1V0;
			bottomFaceTriangle1V0.position = lowerV0;
			bottomFaceTriangle1V0.normal = bottomNormal;

			Vertex bottomFaceTriangle1V1;
			bottomFaceTriangle1V1.position = lowerV1;
			bottomFaceTriangle1V1.normal = bottomNormal;

			Vertex bottomFaceTriangle1V2;
			bottomFaceTriangle1V2.position = lowerV3;
			bottomFaceTriangle1V2.normal = bottomNormal;

			Vertex bottomFaceTriangle2V0;
			bottomFaceTriangle2V0.position = lowerV0;
			bottomFaceTriangle2V0.normal = bottomNormal;

			Vertex bottomFaceTriangle2V1;
			bottomFaceTriangle2V1.position = lowerV3;
			bottomFaceTriangle2V1.normal = bottomNormal;

			Vertex bottomFaceTriangle2V2;
			bottomFaceTriangle2V2.position = lowerV2;
			bottomFaceTriangle2V2.normal = bottomNormal;

			//bottom face push_back
			cube->Vertices.push_back(bottomFaceTriangle1V0);
			cube->Vertices.push_back(bottomFaceTriangle1V1);
			cube->Vertices.push_back(bottomFaceTriangle1V2);

			cube->Vertices.push_back(bottomFaceTriangle2V0);
			cube->Vertices.push_back(bottomFaceTriangle2V1);
			cube->Vertices.push_back(bottomFaceTriangle2V2);

		}

		//---------------------Create forward and back face-----------------------------//
		Vector3 forwardNormal = Vector3(0.0, 0.0, 1.0);
		Vector3 backNormal = Vector3(-0.0, -0.0, -1.0);


		{
			//forward face
			Vertex forwardFaceTriangle1V0;
			forwardFaceTriangle1V0.position = upperV3;
			forwardFaceTriangle1V0.normal = forwardNormal;

			Vertex forwardFaceTriangle1V1;
			forwardFaceTriangle1V1.position = lowerV3;
			forwardFaceTriangle1V1.normal = forwardNormal;

			Vertex forwardFaceTriangle1V2;
			forwardFaceTriangle1V2.position = upperV2;
			forwardFaceTriangle1V2.normal = forwardNormal;

			Vertex forwardFaceTriangle2V0;
			forwardFaceTriangle2V0.position = upperV2;
			forwardFaceTriangle2V0.normal = forwardNormal;

			Vertex forwardFaceTriangle2V1;
			forwardFaceTriangle2V1.position = lowerV3;
			forwardFaceTriangle2V1.normal = forwardNormal;

			Vertex forwardFaceTriangle2V2;
			forwardFaceTriangle2V2.position = lowerV2;
			forwardFaceTriangle2V2.normal = forwardNormal;

			//forward face push_back
			cube->Vertices.push_back(forwardFaceTriangle1V0);
			cube->Vertices.push_back(forwardFaceTriangle1V1);
			cube->Vertices.push_back(forwardFaceTriangle1V2);

			cube->Vertices.push_back(forwardFaceTriangle2V0);
			cube->Vertices.push_back(forwardFaceTriangle2V1);
			cube->Vertices.push_back(forwardFaceTriangle2V2);

		}

		{
			//backward face 
			Vertex backwardFaceTriangle1V0;
			backwardFaceTriangle1V0.position = upperV0;
			backwardFaceTriangle1V0.normal = backNormal;

			Vertex backwardFaceTriangle1V1;
			backwardFaceTriangle1V1.position = upperV1;
			backwardFaceTriangle1V1.normal = backNormal;

			Vertex backwardFaceTriangle1V2;
			backwardFaceTriangle1V2.position = lowerV1;
			backwardFaceTriangle1V2.normal = backNormal;

			Vertex backwardFaceTriangle2V0;
			backwardFaceTriangle2V0.position = upperV0;
			backwardFaceTriangle2V0.normal = backNormal;

			Vertex backwardFaceTriangle2V1;
			backwardFaceTriangle2V1.position = lowerV0;
			backwardFaceTriangle2V1.normal = backNormal;

			Vertex backwardFaceTriangle2V2;
			backwardFaceTriangle2V2.position = lowerV1;
			backwardFaceTriangle2V2.normal = backNormal;

			cube->Vertices.push_back(backwardFaceTriangle1V0);
			cube->Vertices.push_back(backwardFaceTriangle1V1);
			cube->Vertices.push_back(backwardFaceTriangle1V2);

			cube->Vertices.push_back(backwardFaceTriangle2V0);
			cube->Vertices.push_back(backwardFaceTriangle2V1);
			cube->Vertices.push_back(backwardFaceTriangle2V2);


		}

		//---------------------Create right and left face--------------------------//
		Vector3 rightNormal = Vector3(1.0, 0.0, 0.0);
		Vector3 leftNormal = Vector3(-1.0, -0.0, 0.0);

		{
			//right face 
			Vertex rightFaceTriangle1V0;
			rightFaceTriangle1V0.position = upperV1;
			rightFaceTriangle1V0.normal = rightNormal;

			Vertex rightFaceTriangle1V1;
			rightFaceTriangle1V1.position = lowerV3;
			rightFaceTriangle1V1.normal = rightNormal;

			Vertex rightFaceTriangle1V2;
			rightFaceTriangle1V2.position = upperV3;
			rightFaceTriangle1V2.normal = rightNormal;

			Vertex rightFaceTriangle2V0;
			rightFaceTriangle2V0.position = upperV1;
			rightFaceTriangle2V0.normal = rightNormal;

			Vertex rightFaceTriangle2V1;
			rightFaceTriangle2V1.position = lowerV1;
			rightFaceTriangle2V1.normal = rightNormal;

			Vertex rightFaceTriangle2V2;
			rightFaceTriangle2V2.position = lowerV3;
			rightFaceTriangle2V2.normal = rightNormal;

			cube->Vertices.push_back(rightFaceTriangle1V0);
			cube->Vertices.push_back(rightFaceTriangle1V1);
			cube->Vertices.push_back(rightFaceTriangle1V2);

			cube->Vertices.push_back(rightFaceTriangle2V0);
			cube->Vertices.push_back(rightFaceTriangle2V1);
			cube->Vertices.push_back(rightFaceTriangle2V2);
		}

		{
			//left face 
			Vertex leftFaceTriangle1V0;
			leftFaceTriangle1V0.position = upperV0;
			leftFaceTriangle1V0.normal = leftNormal;

			Vertex leftFaceTriangle1V1;
			leftFaceTriangle1V1.position = upperV2;
			leftFaceTriangle1V1.normal = leftNormal;

			Vertex leftFaceTriangle1V2;
			leftFaceTriangle1V2.position = lowerV2;
			leftFaceTriangle1V2.normal = leftNormal;

			Vertex leftFaceTriangle2V0;
			leftFaceTriangle2V0.position = upperV0;
			leftFaceTriangle2V0.normal = leftNormal;

			Vertex leftFaceTriangle2V1;
			leftFaceTriangle2V1.position = lowerV0;
			leftFaceTriangle2V1.normal = leftNormal;

			Vertex leftFaceTriangle2V2;
			leftFaceTriangle2V2.position = lowerV2;
			leftFaceTriangle2V2.normal = leftNormal;

			cube->Vertices.push_back(leftFaceTriangle1V0);
			cube->Vertices.push_back(leftFaceTriangle1V1);
			cube->Vertices.push_back(leftFaceTriangle1V2);

			cube->Vertices.push_back(leftFaceTriangle2V0);
			cube->Vertices.push_back(leftFaceTriangle2V1);
			cube->Vertices.push_back(leftFaceTriangle2V2);
		}


		//set indices
		for (int i = 0; i < cube->Vertices.size(); i++)
		{
			cube->Indices.push_back(i);
		}

		cube->SetupMesh();


		return cube;
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

