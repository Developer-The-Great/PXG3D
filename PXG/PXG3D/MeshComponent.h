#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PXG
{
	struct Mat4;
	class Mesh;
	class AbstractMaterial;


	class MeshComponent : public Component
	{
	public:

		/**@brief : Loads the 3D Model in the file location of name
		 * @param [in] name : the location of the 3D model you would like to load
		*/
		void Load3DModel(std::string name);

		void SetMaterial(std::shared_ptr<AbstractMaterial> material);

		std::shared_ptr<MeshComponent> CloneMeshComponent();
		void Clear()
		{
			meshes.clear();
		}

		void ClearTextures(int i);
		void DisableRender(bool cond)
		{
			cease_rendering = cond;
		}
		
		MeshComponent();

		void Draw(Mat4 parentTransform, Mat4 view, Mat4 projection);

		void AddTextureToMeshAt(Texture texture, int i);

		int GetMeshCount() const;


		std::vector<std::shared_ptr<Mesh>> GetMeshes() const;

	protected:
		//TODO decrease coupling with assimp

		static std::unordered_map<std::string, std::vector< std::shared_ptr<Mesh>>> cache;
		
		std::string directory;

		void processNode(const aiScene* scene, aiNode *node);

		std::shared_ptr<Mesh> processMesh(aiMesh * mesh, const aiScene * scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
			std::string typeName);

		std::vector<std::shared_ptr<Mesh>> meshes;

		std::vector<Texture> texturesLoaded;

		std::shared_ptr<AbstractMaterial> material;

		bool cease_rendering = false;


	};
}


