#include "MeshComponent.h"
#include "Mat4.h"
#include "GameObject.h"
#include  <memory>
#include "Mesh.h"
#include "AbstractMaterial.h"
#include "Shader.h"
#include "RasterizationMode.h"

namespace PXG
{
	std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> MeshComponent::cache;

	MeshComponent::MeshComponent() : Component()
	{
		//setup mesh
	}

	void MeshComponent::Load3DModel(std::string name)
	{

		Assimp::Importer importer;

		if(auto iter = cache.find(name); iter != cache.end())
		{
			meshes.reserve(iter->second.size());

			for(auto &M : iter->second)
			{
				auto m = std::make_shared<Mesh>(*M);
				meshes.emplace_back(m);
			}
			
		}
		else{
			directory = name.substr(0, name.find_last_of('/'));;
			//use assimp importer to import scene
			Debug::Log("Load 3D Model Called");

			const aiScene* scene = importer.ReadFile(name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			Debug::Log("LOG PROCESS NODE");

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				Debug::Log(Verbosity::Error, "ERROR::ASSIMP::{0}", importer.GetErrorString());
				return;
			}

			//process scene graph
			processNode(scene,  scene->mRootNode);

			auto& cmesh = cache[name];
			std::copy(meshes.begin(), meshes.end(), std::back_inserter(cmesh));
		}

	}

	void MeshComponent::SetMaterial(std::shared_ptr<AbstractMaterial> material)
	{
		this->material = material;
	}

	void MeshComponent::processNode(const aiScene* scene, aiNode* node)
	{

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(scene,node->mChildren[i]);
		}

	}

	std::shared_ptr<Mesh> MeshComponent::processMesh(aiMesh * mesh, const aiScene * scene)
	{
		Debug::Log("Process Mesh Called");

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		//get vertices

		Debug::Log("Processing Vertices");

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0])
			{
				float x = mesh->mTextureCoords[0][i].x;
				float y = mesh->mTextureCoords[0][i].y;

				vertex.UV = glm::vec2(x, y);

			}
			else
			{
				vertex.UV = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}



		//indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				unsigned int index = face.mIndices[j];
				indices.push_back(index);
			}
		}

		//textures
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

			std::vector<Texture> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

		}

		return std::make_shared<Mesh>(indices,vertices, textures);
	}

	std::vector<Texture> MeshComponent::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textureFound;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;

			mat->GetTexture(type, i, &str);
			bool isTextureLoaded = false;

			for (unsigned int j = 0; j < texturesLoaded.size(); j++)
			{
				if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
				{
					textureFound.push_back(texturesLoaded[j]);
					isTextureLoaded = true;
					break;
				}
			}

			if (!isTextureLoaded)
			{
				Texture texture;

				texture.id = Texture::GenerateTextureID(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = std::string(str.C_Str());

				texturesLoaded.push_back(texture);
				textureFound.push_back(texture);

			}
		}

		return textureFound;
	}


	std::shared_ptr<MeshComponent> MeshComponent::CloneMeshComponent()
	{
		return std::shared_ptr<MeshComponent>();
	}


	void MeshComponent::Draw(Mat4 parentTransform,Mat4 view,Mat4 projection)
	{
		if (cease_rendering) return;
		Debug::SetDebugState(false);
		auto ownerPointer = GetOwner();

		std::string name = ownerPointer->name;

		Mat4 currentTransform = ownerPointer->GetTransform()->GetLocalTransform() * parentTransform  ;

		//material send uniforms
		if (material)
		{
			std::weak_ptr<World> worldPtr = ownerPointer->GetWorld();
			material->SendUniforms(worldPtr, currentTransform, view, projection);
			Debug::Log("Material Set");

			Debug::Log("drawing with {0}", material->GetShader()->shaderName);

			for (auto const& mesh : meshes)
			{
				mesh->Draw(material->GetShader());
			}

		}

		if (ownerPointer)
		{
			for (std::shared_ptr<GameObject> const& child : ownerPointer->GetChildren())
			{
				child->GetMeshComponent()->Draw(currentTransform, view, projection);
			}
		}

		Debug::SetDebugState(true);

	}

	void MeshComponent::AddTextureToMeshAt(Texture texture, int i)
	{
		meshes.at(i)->Textures.push_back(texture);
	}

	int MeshComponent::GetMeshCount() const
	{
		return meshes.size();
	}

	std::vector<std::shared_ptr<Mesh>> MeshComponent::GetMeshes() const
	{
		return meshes;
	}


	void MeshComponent::ClearTextures(int i)
	{
		meshes.at(i)->Textures.clear();
		
	}
}
