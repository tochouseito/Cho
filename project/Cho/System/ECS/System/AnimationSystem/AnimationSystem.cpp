#include "PrecompiledHeader.h"
#include "AnimationSystem.h"
#include"D3D12/D3DCommand/D3DCommand.h"
#include"D3D12/ResourceViewManager/ResourceViewManager.h"
#include"Graphics/GraphicsSystem/GraphicsSystem.h"
#include"SystemState/SystemState.h"

#include"MeshPattern/MeshPattern.h"

void AnimationSystem::Initialize(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		AnimationComponent* comp = componentManager.GetAnimation(entity);

		if (comp) {
			
		}
	}
}

void AnimationSystem::Update(EntityManager& entityManager, ComponentManager& componentManager)
{
	for (Entity entity : entityManager.GetActiveEntities()) {
		AnimationComponent* comp = componentManager.GetAnimation(entity);
		if (!comp) { continue; }
		MeshComponent* meshComp = componentManager.GetMesh(entity);
		ModelData* model = nullptr;
		if (!meshComp) { continue; }
		if (meshComp->meshModelName!="") {
			model = rvManager_->GetModelData(meshComp->meshModelName);
			comp->modelName = meshComp->meshModelName;
			comp->numAnimation = static_cast<uint32_t>(model->animations.size());
		}
		else { continue; }
		if (comp) {
			timeUpdate(comp,model);
		}
	}
}

Vector3 AnimationSystem::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
	assert(!keyframes.empty());// キーがないものは返す値がわからないのでだめ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {// キーが一つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの二つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Vector3::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまで来た場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

Quaternion AnimationSystem::CalculateValue(const std::vector<KeyframeQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());// キーがないものは返す値がわからないのでだめ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {// キーが一つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの二つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Quaternion::Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまで来た場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

Scale AnimationSystem::CalculateValue(const std::vector<KeyframeScale>& keyframes, float time)
{
	assert(!keyframes.empty());// キーがないものは返す値がわからないのでだめ
	if (keyframes.size() == 1 || time <= keyframes[0].time) {// キーが一つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの二つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補間する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Scale::Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまで来た場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
}

void AnimationSystem::timeUpdate(AnimationComponent* comp, ModelData* model)
{
	// animationIndexが変更された場合、遷移を開始する
	if (comp->prevAnimationIndex != comp->animationIndex) {
		comp->transitionIndex = comp->prevAnimationIndex;
		comp->transition = true;
		comp->transitionTime = 0.0f;
	}

	// 遷移が終了した場合、遷移フラグをfalseにする
	if (comp->lerpTime >= 1.0f) {
		comp->transition = false;
		comp->lerpTime = 0.0f;
		comp->time = comp->transitionTime;
		comp->transitionTime = 0.0f;
	}
	static float deltaTime = 1.0f / 60.0f;
	// 遷移中の場合、遷移時間を更新
	if (comp->transition) {
		comp->transitionTime += deltaTime;//DeltaTime();
		comp->lerpTime = comp->transitionTime / comp->transitionDuration;
		comp->lerpTime = std::clamp(comp->lerpTime, 0.0f, 1.0f);
	}

	// 時間更新
	static bool isLoop =false;
	comp->time += deltaTime;//DeltaTime();
	if (comp->time >= model->animations[comp->animationIndex].duration) {
		isLoop = true;
	}
	else {
		isLoop = false;
	}
	comp->time = std::fmod(comp->time, model->animations[comp->animationIndex].duration);

	// アニメーション更新
	ApplyAnimation(comp, model);
	SkeletonUpdate(comp, model);
	SkinClusterUpdate(comp, model);
	ApplySkinning(comp, model);

	comp->prevAnimationIndex = comp->animationIndex;
	
}

void AnimationSystem::ApplyAnimation(AnimationComponent* comp, ModelData* model)
{
	for (Joint& joint : model->skeleton.joints) {
		// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
		if (auto it = model->animations[comp->animationIndex].nodeAnimations.find(joint.name); it != model->animations[comp->animationIndex].nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			if (comp->transition) {
				if(auto it2 = model->animations[comp->transitionIndex].nodeAnimations.find(joint.name); it2 != model->animations[comp->transitionIndex].nodeAnimations.end()) {
					const NodeAnimation& rootNodeAnimation2 = (*it2).second;
					Vector3 startTranslate = CalculateValue(rootNodeAnimation2.translate.keyframes, comp->time);
					Quaternion startRotate = CalculateValue(rootNodeAnimation2.rotate.keyframes, comp->time);
					Scale startScale = CalculateValue(rootNodeAnimation2.scale.keyframes, comp->time);
					Vector3 endTranslate = CalculateValue(rootNodeAnimation.translate.keyframes, comp->transitionTime);
					Quaternion endRotate = CalculateValue(rootNodeAnimation.rotate.keyframes, comp->transitionTime);
					Scale endScale = CalculateValue(rootNodeAnimation.scale.keyframes, comp->transitionTime);
					joint.transform.translation = Vector3::Lerp(startTranslate, endTranslate, comp->lerpTime);
					joint.transform.rotation = Quaternion::Slerp(startRotate, endRotate, comp->lerpTime);
					joint.transform.scale = Scale::Lerp(startScale, endScale, comp->lerpTime);
				}
			}
			else {
				joint.transform.translation = CalculateValue(rootNodeAnimation.translate.keyframes, comp->time);
				joint.transform.rotation = CalculateValue(rootNodeAnimation.rotate.keyframes, comp->time);
				joint.transform.scale = CalculateValue(rootNodeAnimation.scale.keyframes, comp->time);
			}
		}
	}
}

void AnimationSystem::SkeletonUpdate(AnimationComponent* comp, ModelData* model)
{
	comp;
	// すべてのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : model->skeleton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotation, joint.transform.translation);
		if (joint.parent) {
			joint.skeletonSpaceMatrix = joint.localMatrix * model->skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else {// 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void AnimationSystem::SkinClusterUpdate(AnimationComponent* comp, ModelData* model)
{
	comp;
	for (size_t jointIndex = 0; jointIndex < model->skeleton.joints.size(); ++jointIndex) {
		assert(jointIndex < model->skinCluster.inverseBindPoseMatrices.size());
		model->skinCluster.paletteData.map[jointIndex].skeletonSpaceMatrix =
			model->skinCluster.inverseBindPoseMatrices[jointIndex] * model->skeleton.joints[jointIndex].skeletonSpaceMatrix;
		model->skinCluster.paletteData.map[jointIndex].skeletonSpaceInverseTransposeMatrix =
			ChoMath::Transpose(Matrix4::Inverse(model->skinCluster.paletteData.map[jointIndex].skeletonSpaceMatrix)
			);
	}
}

void AnimationSystem::ApplySkinning(AnimationComponent* comp, ModelData* model)
{
	comp;
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = d3dCommand_->GetCommand(CommandType::Compute).list.Get();

	commandList->SetComputeRootSignature(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::SkinningCS).rootSignature.Get());
	commandList->SetPipelineState(graphicsSystem_->GetPipeline()->GetPSO(PSOMode::SkinningCS).Blend[kBlendModeNone].Get());
	for (std::string name : model->names) {
		commandList->SetComputeRootDescriptorTable(0, rvManager_->GetHandle(model->skinCluster.paletteData.srvIndex).GPUHandle);
		commandList->SetComputeRootDescriptorTable(1, rvManager_->GetHandle(model->skinCluster.skinningData.inputSRVIndex).GPUHandle);
		commandList->SetComputeRootDescriptorTable(2, rvManager_->GetHandle(model->skinCluster.influenceData.srvIndex).GPUHandle);
		commandList->SetComputeRootDescriptorTable(3, rvManager_->GetHandle(model->skinCluster.skinningData.outputUAVIndex).GPUHandle);
		commandList->SetComputeRootConstantBufferView(4, rvManager_->GetCBVResource(model->objects[name].infoCBVIndex)->GetGPUVirtualAddress());
		commandList->Dispatch(UINT(model->objects[name].vertices.size() + 1023) / 1024, 1, 1);

		// リソース遷移
		d3dCommand_->BarrierTransition(
			CommandType::Compute,
			rvManager_->GetHandle(
				model->skinCluster.skinningData.outputUAVIndex).resource.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
		);
	}
}
