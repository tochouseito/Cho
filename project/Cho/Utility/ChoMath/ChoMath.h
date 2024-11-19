#pragma once

//C++
#include<cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include<algorithm>
#include<numbers>
#include<array>

#include <assert.h>

// Utility
#include"Vector2.h"
#include"Vector3.h"
#include"Scale.h"
#include"Vector4.h"
#include"Quaternion.h"
#include"Matrix4.h"
#include"mathShapes.h"

constexpr float PiF = std::numbers::pi_v<float>; // float型のπを定数として定義

static const uint32_t MatNum = 4; //逆行列を求める行列の行数・列数
class ChoMath
{
public:
	// 行列のチェック関数
	static int Check(double mat[MatNum][MatNum], double inv[MatNum][MatNum]);

	static inline float Clamp(float x, float min, float max)
	{
		return (x < min) ? min : (x > max) ? max : x;
	}

	//// 行列の加法
	//static Matrix4 Add(const Matrix4& m1, const Matrix4& m2);

	//// 行列の減法
	//static Matrix4 Subtract(const Matrix4& m1, const Matrix4& m2);

	//// ベクトルの減法
	//static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	// 転置行列
	static Matrix4 Transpose(const Matrix4& m);

	// 単位行列の作成
	static Matrix4 MakeIdentity4x4();

	// 行列の積
	static Matrix4 Multiply(const Matrix4& m1, const Matrix4& m2);

	// 平行移動行列の作成
	static Matrix4 MakeTranslateMatrix(const Vector3& translate);

	// 拡大縮小行列の作成
	static Matrix4 MakeScaleMatrix(const Scale& scale);
	static Matrix4 MakeScaleMatrix(const Vector3& scale);

	//// 座標変換
	//static Vector3 Transform(const Vector3& vector, const Matrix4& matrix);

	// X軸回転行列の作成
	static Matrix4 MakeRotateXMatrix(float radian);

	// Y軸回転行列の作成
	static Matrix4 MakeRotateYMatrix(float radian);

	// Z軸回転行列の作成
	static Matrix4 MakeRotateZMatrix(float radian);

	// XYZ回転行列の作成
	static Matrix4 MakeRotateXYZMatrix(const Vector3& rotate);

	// 3次元アフィン変換行列の作成
	static Matrix4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Matrix4 MakeAffineMatrix(const Scale& scale, const Quaternion& rotate, const Vector3& translate);

	// 透視投影行列の作成
	static Matrix4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//// 正射影行列の作成
	//static Matrix4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	//// ビューポート変換行列の作成
	//static Matrix4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);


	//// ベクトルの長さ（ノルム）
	//static float Length(const Vector2& v);
	//static float Length(const Vector3& v);

	//// ベクトルの正規化
	static float Normalize(float x, float min, float max);
	//static Vector2 Normalize(const Vector2& v);
	//static Vector3 Normalize(const Vector3& v);

	//// ベクトルの外積
	//static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	//// ベクトルの内積
	//static float Dot(const Vector3& v1, const Vector3& v2);

	//// 衝突判定
	//static bool IsCollision(const Vector3& center, const AABB& aabb);

	//// 法線変換
	//static Vector3 TransformNormal(const Vector3& v, const Matrix4& m);

	//// 任意軸回転行列の作成
	//static Matrix4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	//// 方向ベクトルの変換
	//static Matrix4 DirectionToDirection(const Vector3& from, const Vector3& to);

	

	// 任意軸回転を表すクォータニオンの作成
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	// ベクトルをクォータニオンで回転させる
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	// クォータニオンから回転行列を求める
	static Matrix4 MakeRotateMatrix(const Quaternion& quaternion);

	// クォータニオンの内積
	static float Dot(const Quaternion& q0, const Quaternion& q1);

	//// 球面線形補間
	//static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	//// 線形補間
	//static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	//static float Lerp(float a, float b, float t);

	//// X軸回転行列の作成（ピッチ）
	//static Matrix4 MakePitchMatrix(float radian);

	//// Y軸回転行列の作成（ヨー）
	//static Matrix4 MakeYawMatrix(float radian);

	//// Z軸回転行列の作成（ロール）
	//static Matrix4 MakeRollMatrix(float radian);

	//// ベクトル変換
	//static Vector3 TransferNormal(const Vector3& v, const Matrix4& m);

	//// ラジアン変換
	//static float DegreesToRadians(float degrees);

	//// ベクトル回転
	//static Vector3 RotateVector(const Vector3& vec, float angleDegrees);

	//// 反射ベクトルの計算
	//static Vector3 Reflect(const Vector3& input, const Vector3& normal);

	// クォータニオンを利用してベクトルを回転
	/*Vector3 Rotate(const Vector3& v,const Quaternion & rotation) const {
		Quaternion vecQuat(v.x, v.y, v.z, 0.0f);
		Quaternion result = rotation * vecQuat * rotation.Inverse();
		return { result.x, result.y, result.z };
	}*/

	// オイラー角からクォータニオンを生成
	static Quaternion FromEulerAngles(const Vector3& euler);

	// クォータニオンからオイラー角を計算
	static Vector3 ToEulerAngles(const Quaternion& q);
};

