#include "PrecompiledHeader.h"
#include "ModelLoader.h"

// 数学ライブラリ
#include"ChoMath.h"

#include"ConvertString/ConvertString.h"
#include<assert.h>

void ModelLoader::Initialize(TextureLoader* texLoader,MeshLoader* meshLoader)
{
	texLoader_ = texLoader;
	meshLoader_ = meshLoader;
}

void ModelLoader::LoadModel(const std::string& fileRoot, const std::string& fileName)
{
	// モデルファイルの名前でコンテナに保存する
	std::string modelName = fileName;

	// モデルファイルの読み込み
	modelContainer[modelName] = std::make_unique<ModelData>();
	ModelData* modelData = modelContainer[modelName].get();
	modelData = LoadModelFile(fileRoot, fileName);

	// meshリソースの作成
	for (const std::string& name : modelData->names) {
		
	}
}

ModelData* ModelLoader::LoadModelFile(const std::string& fileRoot, const std::string& fileName)
{
	// 変数の宣言
	ModelData* modelData = new ModelData();
	std::vector<Vector4> positions;// 位置
	std::vector<Vector3> normals;// 法線
	std::vector<Vector2> rexcoords;// UV座標
	std::string line;// ファイルから読んだ1行を格納するもの
	Assimp::Importer importer;
	std::string filePath;

	// ここからファイルを開く
	
	// まずobjファイルの読み込みのみ
	filePath = fileRoot + "/" + fileName + "/" + fileName + ".obj";

	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());// メッシュがないのは対応しない

	// SceneのRootNodeを読んでシーン全体の階層構造を作る
	//modelData->rootNode=

	// Meshの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());// 法線がないMeshは非対応
		std::string meshName = mesh->mName.C_Str();
		std::wstring meshName2 = ConvertString(mesh->mName.C_Str());
		modelData->names.push_back(meshName);
		modelData->objects[meshName].useTexture = mesh->HasTextureCoords(0);

		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			VertexData vertex;
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			// UV座標のチェックと設定
			if (mesh->HasTextureCoords(0)) {
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				vertex.texcoord = { texcoord.x,texcoord.y };
			} else {
				vertex.texcoord = { 0.0f,0.0f };// ダミーのUV座標
			}
			vertex.position = { position.x,position.y,position.z,1.0f };
			vertex.normal = { normal.x,normal.y,normal.z };
			vertex.position.x *= -1.0f;
			vertex.normal.x *= -1.0f;
			// データを追加
			modelData->objects[meshName].vertices.push_back(vertex);
		}
		// Index解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			// 三角形の処理
			if (face.mNumIndices == 3) {
				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					modelData->objects[meshName].indices.push_back(vertexIndex);
				}
			}
			// 四角形の処理（四角形の2つの三角形に分割）
			else if (face.mNumIndices == 4) {
				// 四角形の1つ目の三角形(0,1,2)
				modelData->objects[meshName].indices.push_back(face.mIndices[0]);
				modelData->objects[meshName].indices.push_back(face.mIndices[1]);
				modelData->objects[meshName].indices.push_back(face.mIndices[2]);

				// 四角形の2つ目の三角形(0,1,2)
				modelData->objects[meshName].indices.push_back(face.mIndices[0]);
				modelData->objects[meshName].indices.push_back(face.mIndices[2]);
				modelData->objects[meshName].indices.push_back(face.mIndices[3]);
			}
			// サポート外のポリゴン数の場合のエラーチェック
			else {
				assert(false && "Unsupported polygon type");
			}
		}
		// ボーン解析
		if (!mesh->mNumBones) {
			modelData->isBone = false;
		} else {
			modelData->isBone = true;
		}

		// マテリアル解析
		uint32_t materialIndex = mesh->mMaterialIndex;
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			// 画像テクスチャの読み込み処理
			//
			//
		} else {
			modelData->objects[meshName].useTexture = false;
		}
	}
	return modelData;
}
