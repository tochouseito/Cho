#include "PrecompiledHeader.h"
#include "ModelLoader.h"

// 数学ライブラリ
#include"ChoMath.h"

#include"ConvertString/ConvertString.h"
#include<assert.h>
#include"Load/MeshLoader/MeshLoader.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"

void ModelLoader::Initialize(ResourceViewManager* rvManager, TextureLoader* texLoader,MeshLoader* meshLoader)
{
	rvManager_ = rvManager;
	texLoader_ = texLoader;
	meshLoader_ = meshLoader;
}

void ModelLoader::LoadModel(const std::string& directoryPath, const fs::directory_entry& entry)
{
	// モデルファイルの名前でコンテナに保存する
	std::string modelName = entry.path().stem().string(); // 拡張子を除いた部分

	// モデルファイルの読み込み
	rvManager_->AddModel(modelName);
	
	ModelData* modelData = rvManager_->GetModelData(modelName);
	LoadModelFile(modelData,directoryPath, entry);

	// meshリソースの作成
	for (const std::string& name : modelData->names) {
		uint32_t vertices = static_cast<uint32_t>(modelData->objects[name].vertices.size());
		uint32_t indices = static_cast<uint32_t>(modelData->objects[name].indices.size());
		modelData->objects[name].meshIndex = meshLoader_->LoadMesh(name, vertices, indices);
		rvManager_->ModelMeshMap(modelData->objects[name].meshIndex, name);
		rvManager_->MeshDataCopy(modelData->objects[name].meshIndex, name, modelName);
		if (modelData->isBone) {
			// InputResourceのMapping
			rvManager_->GetHandle(
				modelData->skinCluster.skinningData.inputSRVIndex).resource->Map(
					0,
					nullptr,
					reinterpret_cast<void**>(&rvManager_->GetMesh(
						modelData->objects[name].meshIndex)->meshData[name].vertexData)
				);
			modelData->objects[name].infoCBVIndex = rvManager_->CreateCBV(sizeof(SkinningInformation));
			rvManager_->GetCBVResource(modelData->objects[name].infoCBVIndex)->Map(0, nullptr, reinterpret_cast<void**>(&modelData->objects[name].infoData));
			modelData->objects[name].infoData->numVertices = static_cast<uint32_t>(modelData->objects[name].vertices.size());
		}

		rvManager_->MeshDataCopy(modelData->objects[name].meshIndex, name, modelName);
	}
}

void ModelLoader::FirstResourceLoad(const std::string& directoryPath)
{
	// ファイルパスのリストを取得
	for (const auto& entry : fs::directory_iterator(directoryPath))
	{
		// ファイルかどうかを確認
		if (fs::is_directory(entry.path()))
		{
			for (const auto& modelEntry : fs::directory_iterator(entry.path()))
			{
				if (fs::is_regular_file(modelEntry.path())) {
					std::string filePath = modelEntry.path().string();

					// ファイル名部分のみ取得（ディレクトリパスを除去）
					std::string fileName = modelEntry.path().filename().string();
					std::string fileStem = modelEntry.path().stem().string(); // 拡張子を除いた部分

					// ファイルの形式をチェック
					if (fileName.ends_with(".obj") || fileName.ends_with(".gltf"))
					{
						// directoryPath をフルパスに変換
						std::string fullDirectoryPath = fs::absolute(directoryPath).string();
						// ファイルを読み込み
						LoadModel(fullDirectoryPath, modelEntry);
					}
				}
			}
		}
	}
}

void ModelLoader::LoadModelFile(ModelData* modelData, const std::string& directoryPath, const fs::directory_entry& entry)
{
	// 変数の宣言
	//ModelData* modelData = new ModelData();
	std::vector<Vector4> positions;// 位置
	std::vector<Vector3> normals;// 法線
	std::vector<Vector2> rexcoords;// UV座標
	std::string line;// ファイルから読んだ1行を格納するもの
	Assimp::Importer importer;
	std::string filePath;

	// ここからファイルを開く
	directoryPath;
	filePath = entry.path().string();
	filePath = fs::absolute(filePath).string();

	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	std::string err = importer.GetErrorString();
	assert(scene->HasMeshes());// メッシュがないのは対応しない

	// SceneのRootNodeを読んでシーン全体の階層構造を作り上げる
	modelData->rootNode = ReadNode(scene->mRootNode);

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
		/*ボーン解析*/
		if (!mesh->mNumBones) {
			modelData->isBone = false;
		}
		else
		{
			modelData->isBone = true;
		}
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];// AssimpではJointをBoneと呼んでいる
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData->objects[meshName].skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();// BindPoseMatrixに戻す
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);// 成分を抽出
			/*左手系のBindPoseMatrixを作る*/
			Matrix4 bindPoseMatrix = MakeAffineMatrix(
				{ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			/*InverseBindPoseMatrixにする*/
			jointWeightData.inverseBindPoseMatrix = Matrix4::Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
			}
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
	// ファイルの形式をチェック
	if (scene->mNumAnimations != 0) {
		LoadAnimationFile(modelData, directoryPath, entry);
		if (modelData->isBone) {
			CreateSkeleton(modelData, modelData->rootNode);
			for (std::string& name : modelData->names) {
				CreateSkinCluster(modelData,name, modelData->objects[name]);
			}
		}
	}
}

void ModelLoader::LoadAnimationFile(ModelData* modelData, const std::string& directoryPath, const fs::directory_entry& entry)
{
	AnimationData animation;// 今回作るアニメーション
	Assimp::Importer importer;
	std::string filePath;

	directoryPath;
	filePath = entry.path().string();
	filePath = fs::absolute(filePath).string();
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);// アニメーションがない

	aiAnimation* animationAssimp = scene->mAnimations[0];// 最初のanimationだけ採用。複数対応予定
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);// 時間の単位を秒に変換
	// assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここも秒に変換
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };// 右手->左手
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w };
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeScale keyframe;
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);// ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
		/*
		RotateはmNumRotationKeys/mRotationKeys,ScaleはmNumScalingKeys/mScalingKeysで取得できるので同様に行う。
		RotateはQuaternionで、右手->左手に変換するために、yとzを反転させる必要がある。Scaleはそのままでいい。
		Keyframe.value={rotate.x,-rotate.y,-rotate.z,rotate.w};
		*/
	}
	// 解析完了
	modelData->animations.push_back(animation);
}

Node ModelLoader::ReadNode(aiNode* node)
{
	Node result;
	Node result2;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;// nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();// 列ベクトルを行ベクトルに転置
	for (uint32_t mindex = 0; mindex < 4; ++mindex) {
		for (uint32_t index = 0; index < 4; ++index) {
			result2.localMatrix.m[mindex][index] = aiLocalMatrix[mindex][index];
		}
	}
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);// assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x,scale.y,scale.z };// scaleはそのまま
	result.transform.rotation = { rotate.x,-rotate.y,-rotate.z,rotate.w };// x軸を反転。さらに回転方向が逆なので軸を反転させる
	result.transform.translation = { -translate.x,translate.y,translate.z };// x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotation, result.transform.translation);
	result.name = node->mName.C_Str();// Node名を格納
	result.children.resize(node->mNumChildren);// 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}

void ModelLoader::CreateSkeleton(ModelData* modelData, const Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}
	modelData->skeleton = skeleton;
}

int32_t ModelLoader::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = static_cast<int32_t>(joints.size());// 現在登録されている数をIndexに
	joint.parent = parent;
	joints.push_back(joint);// SkeletonのJoint列に追加
	for (const Node& child : node.children) {
		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
	/*
	本来はanimationするNodeのみを対象にしたほうがいいが今は全Nodeを対象にしている
	*/
}

void ModelLoader::CreateSkinCluster(ModelData* modelData, const std::string& name, const ObjectData& objectData)
{

	SkinCluster skinCluster;
	/*palette用のSRVを作成*/
	skinCluster.paletteData.srvIndex = rvManager_->GetNewHandle();
	rvManager_->CreateSRVResource(
		skinCluster.paletteData.srvIndex,
		sizeof(ConstBufferDataWell) * modelData->skeleton.joints.size()
	);
	ConstBufferDataWell* mappedPalette = nullptr;
	rvManager_->GetHandle(
		skinCluster.paletteData.srvIndex).resource->Map(
			0,
			nullptr,
			reinterpret_cast<void**>(&mappedPalette)
		);
	skinCluster.paletteData.map = { mappedPalette,modelData->skeleton.joints.size() };// spanを使ってアクセスする
	rvManager_->CreateSRVforStructuredBuffer(
		skinCluster.paletteData.srvIndex,
		static_cast<UINT>(modelData->skeleton.joints.size()),
		sizeof(ConstBufferDataWell)
	);

	/*influence用のsrvを作成。頂点ごとにInfluence情報を追加できるようにする*/
	skinCluster.influenceData.srvIndex = rvManager_->GetNewHandle();
	rvManager_->CreateSRVResource(
		skinCluster.influenceData.srvIndex,
		sizeof(ConstBufferDataVertexInfluence) * objectData.vertices.size()
	);
	ConstBufferDataVertexInfluence* mappedInfluence = nullptr;
	rvManager_->GetHandle(
		skinCluster.influenceData.srvIndex).resource->Map(
			0,
			nullptr,
			reinterpret_cast<void**>(&mappedInfluence)
		);
	std::memset(mappedInfluence, 0, sizeof(ConstBufferDataVertexInfluence) * objectData.vertices.size());// 0埋め。weightを0にしておく
	skinCluster.influenceData.map = { mappedInfluence,objectData.vertices.size() };// spanを使ってアクセスする
	rvManager_->CreateSRVforStructuredBuffer(
		skinCluster.influenceData.srvIndex,
		static_cast<UINT>(objectData.vertices.size()),
		sizeof(ConstBufferDataVertexInfluence)
	);

	// Skinning用のInputResourceを作成
	// Handleを取得
	skinCluster.skinningData.inputSRVIndex = rvManager_->GetNewHandle();
	// Resourceを作成
	rvManager_->CreateSRVResource(
		skinCluster.skinningData.inputSRVIndex,
		sizeof(VertexData) * objectData.vertices.size()
	);
	// SRVを作成
	rvManager_->CreateSRVforStructuredBuffer(
		skinCluster.skinningData.inputSRVIndex,
		static_cast<UINT>(modelData->objects.size()),
		static_cast<UINT>(sizeof(VertexData) * objectData.vertices.size())
	);
	// MeshResourceを作成
	skinCluster.skinningData.inputMVIndex = 
		rvManager_->CreateMeshResource(
			name,
			static_cast<uint32_t>(objectData.vertices.size()),
			0,
			sizeof(VertexData),
			rvManager_->GetHandle(
				skinCluster.skinningData.inputSRVIndex).resource.Get()
		);

	// Skinning用のOutputResourceを作成
	// Handleを取得
	skinCluster.skinningData.outputUAVIndex = rvManager_->GetNewHandle();
	// Resourceを作成
	rvManager_->CreateUAVResource(skinCluster.skinningData.outputUAVIndex,
		sizeof(VertexData) * objectData.vertices.size()
	);
	// UAVを作成
	rvManager_->CreateUAVforStructuredBuffer(
		skinCluster.skinningData.outputUAVIndex,
		static_cast<UINT>(modelData->objects.size()),
		static_cast<UINT>(sizeof(VertexData) * objectData.vertices.size())
	);
	// MeshResourceを作成
	skinCluster.skinningData.outputMVIndex =
		rvManager_->CreateMeshResource(
			name,
			static_cast<uint32_t>(objectData.vertices.size()),
			0,
			sizeof(VertexData),
			rvManager_->GetHandle(
				skinCluster.skinningData.outputUAVIndex).resource.Get()
		);

	/*Influence用のVBVを作成*/
	skinCluster.influenceData.meshViewIndex = rvManager_->CreateMeshResource(name, static_cast<uint32_t>(objectData.vertices.size()), 0,sizeof(ConstBufferDataVertexInfluence));
	/*InverseBindPoseMatrixの保存領域を作成*/
	skinCluster.inverseBindPoseMatrices.resize(modelData->skeleton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), []() { return ChoMath::MakeIdentity4x4(); });
	/*ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める*/
	for (const auto& jointWeight : objectData.skinClusterData) {// ModelのSkinClusterの情報を解析
		auto it = modelData->skeleton.jointMap.find(jointWeight.first);// jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == modelData->skeleton.jointMap.end()) {// そんな名前のjointは存在しない、なので次に回す
			continue;
		}
		/*(*it).secondにはJointのIndexが入っているので、該当のindexのInverseBindPoseMatrixを代入*/
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.influenceData.map[vertexWeight.vertexIndex];// 該当のvertexIndexのinfluence情報を参照しておく
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {// 空いてるところに入れる
				if (currentInfluence.weights[index] == 0.0f) {// weight==0が空いてる状態なので、その場所にweightとjointのIndexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}
	modelData->skinCluster = skinCluster;
}

