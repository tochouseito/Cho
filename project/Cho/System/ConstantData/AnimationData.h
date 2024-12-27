#pragma once

// 数学ライブラリ
#include"ChoMath.h"

// デバッグ用
#include"ConstantData/DebugTransform.h"

// C++
#include<cstdint>
#include<vector>
#include<map>
#include<string>
#include<optional>

template<typename T>
struct Keyframe {
	float time;
	T value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;
using KeyframeScale = Keyframe<Scale>;

template<typename T>
struct AnimationCurve
{
	std::vector<Keyframe<T>> keyframes;
};
struct NodeAnimation {
	AnimationCurve<Scale> scale;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> translate;
};

// ジョイント
struct Joint {
	DebugTransform transform;// Transform情報
	Matrix4 localMatrix;
	Matrix4 skeletonSpaceMatrix;// skeletonSpaceでの変換行列
	std::string name;// 名前
	std::vector<int32_t> children;// 子JointのIndexのリスト。いなければ空
	int32_t index;// 自身のIndex
	std::optional<int32_t> parent;// 親JointのIndex。いなければnull
};

// スケルトン
struct Skeleton {
	int32_t root;// RootJointのIndex
	std::map<std::string, int32_t> jointMap;// Joint名とIndexとの辞書
	std::vector<Joint> joints;// 所属しているJoint
	Skeleton() = default;
};

// ウェイトデータ
struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};
struct JointWeightData {
	Matrix4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};

// skinning
struct SkinningInformation {
	uint32_t numVertices;
};

// アニメーションデータ構造体
struct AnimationData {
	float duration = 0.0f;// アニメーション全体の尺（単位は秒）
	std::map<std::string, NodeAnimation> nodeAnimations;
	AnimationData() = default;
};