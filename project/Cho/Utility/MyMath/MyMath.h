#pragma once
#include <assert.h>
#include<cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include<algorithm>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4x4.h"
#include"mathShapes.h"
#include<numbers>
static const uint32_t MatNum = 4; //逆行列を求める行列の行数・列数
class MyMath
{
public:
	// 行列のチェック関数
	static int Check(double mat[MatNum][MatNum], double inv[MatNum][MatNum]);

	//// 行列の加法
	//static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	//// 行列の減法
	//static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

	//// ベクトルの減法
	//static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);

	// 単位行列の作成
	static Matrix4x4 MakeIdentity4x4();

	// 行列の積
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 平行移動行列の作成
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// 拡大縮小行列の作成
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	//// 座標変換
	//static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// X軸回転行列の作成
	static Matrix4x4 MakeRotateXMatrix(float radian);

	// Y軸回転行列の作成
	static Matrix4x4 MakeRotateYMatrix(float radian);

	// Z軸回転行列の作成
	static Matrix4x4 MakeRotateZMatrix(float radian);

	// XYZ回転行列の作成
	static Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate);

	// 3次元アフィン変換行列の作成
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	//static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	// 透視投影行列の作成
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//// 正射影行列の作成
	//static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	//// ビューポート変換行列の作成
	//static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 逆行列の作成
	static Matrix4x4 Inverse(const Matrix4x4& m);

	//// ベクトルの長さ（ノルム）
	//static float Length(const Vector2& v);
	//static float Length(const Vector3& v);

	//// ベクトルの正規化
	//static Vector2 Normalize(const Vector2& v);
	//static Vector3 Normalize(const Vector3& v);

	//// ベクトルの外積
	//static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	//// ベクトルの内積
	//static float Dot(const Vector3& v1, const Vector3& v2);

	//// 衝突判定
	//static bool IsCollision(const Vector3& center, const AABB& aabb);

	//// 法線変換
	//static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	//// 任意軸回転行列の作成
	//static Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	//// 方向ベクトルの変換
	//static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	//// クォータニオンの積
	//static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	//// 単位クォータニオンの作成
	//static Quaternion IdentityQuaternion();

	//// クォータニオンの共役を返す
	//static Quaternion Conjugate(const Quaternion& quaternion);

	//// クォータニオンのノルムを返す
	//static float Norm(const Quaternion& quaternion);

	//// クォータニオンの正規化
	//static Quaternion Normalize(const Quaternion& quaternion);

	//// クォータニオンの逆を返す
	//static Quaternion Inverse(const Quaternion& quaternion);

	//// 任意軸回転を表すクォータニオンの作成
	//static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	//// ベクトルをクォータニオンで回転させる
	//static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	//// クォータニオンから回転行列を求める
	//static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	//// クォータニオンの内積
	//static float Dot(const Quaternion& q0, const Quaternion& q1);

	//// 球面線形補間
	//static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	//// 線形補間
	//static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	//static float Lerp(float a, float b, float t);

	//// X軸回転行列の作成（ピッチ）
	//static Matrix4x4 MakePitchMatrix(float radian);

	//// Y軸回転行列の作成（ヨー）
	//static Matrix4x4 MakeYawMatrix(float radian);

	//// Z軸回転行列の作成（ロール）
	//static Matrix4x4 MakeRollMatrix(float radian);

	//// ベクトル変換
	//static Vector3 TransferNormal(const Vector3& v, const Matrix4x4& m);

	//// ラジアン変換
	//static float DegreesToRadians(float degrees);

	//// ベクトル回転
	//static Vector3 RotateVector(const Vector3& vec, float angleDegrees);

	//// 反射ベクトルの計算
	//static Vector3 Reflect(const Vector3& input, const Vector3& normal);
};

