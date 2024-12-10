#pragma once
// ChoMath.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"

class ChoMath {

public:

	// 行列のチェック関数

	static int Check(double mat[MatCount][MatCount], double inv[MatCount][MatCount]);


	static float Clamp(float x, float min, float max);


	static Matrix4 Transpose(const Matrix4& m);


	static Matrix4 MakeIdentity4x4();


	static Matrix4 Multiply(const Matrix4& m1, const Matrix4& m2);

	static Matrix4 MakeTranslateMatrix(const Vector3& translate);


	static Matrix4 MakeScaleMatrix(const Scale& scale);

	static Matrix4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4 MakeRotateXMatrix(float radian);

	static Matrix4 MakeRotateYMatrix(float radian);

	static Matrix4 MakeRotateZMatrix(float radian);

	static Matrix4 MakeRotateXYZMatrix(const Vector3& rotate);

	static Matrix4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//// ベクトルの正規化
	static float Normalize(float x, float min, float max);

	static float Dot(const Vector3& v1, const Vector3& v2);

	static Matrix4 MakeRotateAxisAngle(const Vector3& axis, float angle);

	static Matrix4 DirectionToDirection(const Vector3& from, const Vector3& to);


	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
	static Matrix4 MakeRotateMatrix(const Quaternion& quaternion);
	static float Dot(const Quaternion& q0, const Quaternion& q1);
	// オイラー角からクォータニオンを生成

	static Quaternion FromEulerAngles(const Vector3& eulerAngles);

	// クォータニオンからオイラー角を計算（回転順序を指定）

	static Vector3 ToEulerAngles(const Quaternion& q, RotationOrder order);
	static Matrix4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Matrix4 MakeAffineMatrix(const Scale& scale, const Quaternion& rotate, const Vector3& translate);

};