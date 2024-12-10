// ChoMath.cpp : スタティック ライブラリ用の関数を定義します。
//

#include "pch.h"
#include "framework.h"
#include "ChoMath.h"

// 行列のチェック関数

int ChoMath::Check(double mat[MatCount][MatCount], double inv[MatCount][MatCount])
{
	double inner_product;
	int i, j, k;
	double ans;
	double diff;
	for (i = 0; i < MatCount; i++) {
		for (j = 0; j < MatCount; j++) {
			inner_product = 0;
			for (k = 0; k < MatCount; k++) {
				inner_product += mat[i][k] * inv[k][j];
			}

			ans = (i == j) ? 1 : 0;
			diff = ans - inner_product;


		}
	}

	return 1;
}

float ChoMath::Clamp(float x, float min, float max)
{
	return (x < min) ? min : (x > max) ? max : x;
}

Matrix4 ChoMath::Transpose(const Matrix4& m)
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatCount; ++i) {
		for (int j = 0; j < MatCount; ++j) {
			result.m[j][i] = m.m[i][j];
		}
	}
	return result;
}

Matrix4 ChoMath::MakeIdentity4x4()
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatCount; ++i) {
		result.m[i][i] = 1.0;
	}
	return result;
}

Matrix4 ChoMath::Multiply(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatCount; ++i) {
		for (int j = 0; j < MatCount; ++j) {
			for (int k = 0; k < MatCount; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4 ChoMath::MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4 translationMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x,translate.y, translate.z, 1.0f
	};
	return translationMatrix;
}

Matrix4 ChoMath::MakeScaleMatrix(const Scale& scale)
{
	Matrix4 scaleMatrix = {
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};
	return scaleMatrix;
}

Matrix4 ChoMath::MakeScaleMatrix(const Vector3& scale)
{
	Matrix4 scaleMatrix = {
		scale.x, 0.0f,    0.0f,    0.0f,
		0.0f,    scale.y, 0.0f,    0.0f,
		0.0f,    0.0f,    scale.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};
	return scaleMatrix;
}

Matrix4 ChoMath::MakeRotateXMatrix(float radian)
{
	float cosX = cosf(radian);
	float sinX = sinf(radian);
	Matrix4 result = {
		1.0f, 0.0f,  0.0f,   0.0f,
		0.0f, cosX,  sinX,   0.0f,
		0.0f, -sinX, cosX,   0.0f,
		0.0f, 0.0f,  0.0f,   1.0f
	};
	return result;
}

Matrix4 ChoMath::MakeRotateYMatrix(float radian)
{
	float cosY = cosf(radian);
	float sinY = sinf(radian);
	Matrix4 result = {
		cosY,  0.0f, -sinY,  0.0f,
		0.0f,  1.0f, 0.0f,   0.0f,
		sinY,  0.0f, cosY,   0.0f,
		0.0f,  0.0f, 0.0f,   1.0f
	};
	return result;
}

Matrix4 ChoMath::MakeRotateZMatrix(float radian)
{
	float cosZ = cosf(radian);
	float sinZ = sinf(radian);
	Matrix4 result = {
		cosZ,  sinZ,  0.0f,  0.0f,
		-sinZ, cosZ,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f
	};
	return result;
}

Matrix4 ChoMath::MakeRotateXYZMatrix(const Vector3& rotate)
{
	Matrix4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4 rotateXYZMatrix = Multiply(Multiply(rotateZMatrix, rotateYMatrix), rotateXMatrix);

	return rotateXYZMatrix;
}

Matrix4 ChoMath::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	float tanHalfFovY = std::tanf(fovY / 2.0f);
	return Matrix4{
		1.0f / (aspectRatio * tanHalfFovY), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / tanHalfFovY, 0.0f, 0.0f,
		0.0f, 0.0f, (farClip + nearClip) / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, -(2.0f * farClip * nearClip) / (farClip - nearClip), 0.0f
	};
}

//// ベクトルの正規化
float ChoMath::Normalize(float x, float min, float max)
{
	if (max - min == 0)
	{
		return 0.0f;
	}
	float normalized = (x - min) / (max - min);
	return Clamp(normalized, 0.0f, 1.0f); // 結果を [0, 1] にクランプ
}

float ChoMath::Dot(const Vector3& v1, const Vector3& v2)
{
	float result;
	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return result;
}

Matrix4 ChoMath::MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Vector3 normAxis = axis;
	float axisLength = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	if (axisLength != 0.0f) {
		normAxis.x /= axisLength;
		normAxis.y /= axisLength;
		normAxis.z /= axisLength;
	}

	float cosTheta = std::cos(angle);
	float sinTheta = std::sin(angle);
	float oneMinusCosTheta = 1.0f - cosTheta;

	Matrix4 rotateMatrix;

	rotateMatrix.m[0][0] = cosTheta + normAxis.x * normAxis.x * oneMinusCosTheta;
	rotateMatrix.m[0][1] = normAxis.x * normAxis.y * oneMinusCosTheta - normAxis.z * sinTheta;
	rotateMatrix.m[0][2] = normAxis.x * normAxis.z * oneMinusCosTheta + normAxis.y * sinTheta;
	rotateMatrix.m[0][3] = 0.0f;

	rotateMatrix.m[1][0] = normAxis.y * normAxis.x * oneMinusCosTheta + normAxis.z * sinTheta;
	rotateMatrix.m[1][1] = cosTheta + normAxis.y * normAxis.y * oneMinusCosTheta;
	rotateMatrix.m[1][2] = normAxis.y * normAxis.z * oneMinusCosTheta - normAxis.x * sinTheta;
	rotateMatrix.m[1][3] = 0.0f;

	rotateMatrix.m[2][0] = normAxis.z * normAxis.x * oneMinusCosTheta - normAxis.y * sinTheta;
	rotateMatrix.m[2][1] = normAxis.z * normAxis.y * oneMinusCosTheta + normAxis.x * sinTheta;
	rotateMatrix.m[2][2] = cosTheta + normAxis.z * normAxis.z * oneMinusCosTheta;
	rotateMatrix.m[2][3] = 0.0f;

	rotateMatrix.m[3][0] = 0.0f;
	rotateMatrix.m[3][1] = 0.0f;
	rotateMatrix.m[3][2] = 0.0f;
	rotateMatrix.m[3][3] = 1.0f;

	rotateMatrix = Transpose(rotateMatrix);

	return rotateMatrix;
}

Matrix4 ChoMath::DirectionToDirection(const Vector3& from, const Vector3& to) {
	// 入力ベクトルを正規化
	Vector3 normalizedFrom = Vector3::Normalize(from);
	Vector3 normalizedTo = Vector3::Normalize(to);

	// 回転軸を計算
	Vector3 axis = Vector3::Cross(normalizedFrom, normalizedTo);
	axis.Normalize();

	// 特殊ケース: from と -to が一致する場合
	if (normalizedFrom == -normalizedTo) {
		// 任意の直交軸を選択
		if (std::abs(normalizedFrom.x) < std::abs(normalizedFrom.y)) {
			axis = Vector3::Normalize(Vector3{ 0.0f, -normalizedFrom.z, normalizedFrom.y });
		} else {
			axis = Vector3::Normalize(Vector3{ -normalizedFrom.y, normalizedFrom.x, 0.0f });
		}
	}

	// cosTheta と sinTheta を計算
	float cosTheta = normalizedFrom.Dot(normalizedTo);
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

	// 最初から転置された形で回転行列を作成
	Matrix4 rotateMatrix = {
		(axis.x * axis.x) * (1 - cosTheta) + cosTheta,        (axis.x * axis.y) * (1 - cosTheta) + (axis.z * sinTheta), (axis.x * axis.z) * (1 - cosTheta) - (axis.y * sinTheta), 0.0f,
		(axis.x * axis.y) * (1 - cosTheta) - (axis.z * sinTheta), (axis.y * axis.y) * (1 - cosTheta) + cosTheta,        (axis.y * axis.z) * (1 - cosTheta) + (axis.x * sinTheta), 0.0f,
		(axis.x * axis.z) * (1 - cosTheta) + (axis.y * sinTheta), (axis.y * axis.z) * (1 - cosTheta) - (axis.x * sinTheta), (axis.z * axis.z) * (1 - cosTheta) + cosTheta,        0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return rotateMatrix;
}


Quaternion ChoMath::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Vector3 normAxis = axis;
	normAxis.Normalize();
	float sinHalfAngle = std::sin(angle / 2.0f);
	float cosHalfAngle = std::cos(angle / 2.0f);
	return { normAxis.x * sinHalfAngle, normAxis.y * sinHalfAngle, normAxis.z * sinHalfAngle, cosHalfAngle };
}

Vector3 ChoMath::RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	vector, quaternion;
	/*Quaternion qVector = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConjugate = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
	Quaternion qResult = Multiply(Multiply(quaternion, qVector), qConjugate);
	return { qResult.x, qResult.y, qResult.z };*/
	return vector;
}

Matrix4 ChoMath::MakeRotateMatrix(const Quaternion& quaternion) {
	Matrix4 matrix;

	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;
	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	// 左手座標系に合わせて z に符号を反転
	matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
	matrix.m[0][1] = 2.0f * (xy + wz); // 符号を反転
	matrix.m[0][2] = 2.0f * (xz - wy); // 符号を反転
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 2.0f * (xy - wz); // 符号を反転
	matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
	matrix.m[1][2] = 2.0f * (yz + wx); // 符号を反転
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 2.0f * (xz + wy); // 符号を反転
	matrix.m[2][1] = 2.0f * (yz - wx); // 符号を反転
	matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

float ChoMath::Dot(const Quaternion& q0, const Quaternion& q1)
{
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}




// オイラー角からクォータニオンを生成

Quaternion ChoMath::FromEulerAngles(const Vector3& eulerAngles) {
	// 半角を計算
	float halfX = eulerAngles.x * 0.5f; // ピッチ（X軸）
	float halfY = eulerAngles.y * 0.5f; // ヨー（Y軸）
	float halfZ = eulerAngles.z * 0.5f; // ロール（Z軸）

	// 三角関数を計算
	float cx = std::cos(halfX); // cos(θx / 2)
	float sx = std::sin(halfX); // sin(θx / 2)
	float cy = std::cos(halfY); // cos(θy / 2)
	float sy = std::sin(halfY); // sin(θy / 2)
	float cz = std::cos(halfZ); // cos(θz / 2)
	float sz = std::sin(halfZ); // sin(θz / 2)

	// 左手座標系を考慮したXYZ順のクォータニオン計算
	return Quaternion{
		sx * cy * cz + cx * sy * sz, // X
		cx * sy * cz - sx * cy * sz, // Y
		cx * cy * sz - sx * sy * cz, // Z
		cx * cy * cz + sx * sy * sz  // W
	};
}

// クォータニオンからオイラー角を計算（回転順序を指定）

Vector3 ChoMath::ToEulerAngles(const Quaternion& q, RotationOrder order) {
	Vector3 angles;

	switch (order) {
	case RotationOrder::XYZ: {
		float sinp = 2 * (q.w * q.x + q.y * q.z);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.y - q.z * q.x);
		float cosy_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosr_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	case RotationOrder::YXZ: {
		float sinp = -2 * (q.w * q.y - q.x * q.z);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.x + q.z * q.y);
		float cosy_cosp = 1 - 2 * (q.y * q.y + q.x * q.x);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.z - q.x * q.y);
		float cosr_cosp = 1 - 2 * (q.z * q.z + q.x * q.x);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	case RotationOrder::ZXY: {
		float sinp = 2 * (q.w * q.z - q.x * q.y);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.x + q.y * q.z);
		float cosy_cosp = 1 - 2 * (q.z * q.z + q.x * q.x);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.y - q.z * q.x);
		float cosr_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	case RotationOrder::ZYX: {
		float sinp = 2 * (q.w * q.y + q.z * q.x);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.x - q.y * q.z);
		float cosy_cosp = 1 - 2 * (q.z * q.z + q.x * q.x);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.z - q.x * q.y);
		float cosr_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	case RotationOrder::YZX: {
		float sinp = 2 * (q.w * q.y - q.x * q.z);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.x - q.z * q.y);
		float cosr_cosp = 1 - 2 * (q.z * q.z + q.x * q.x);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	case RotationOrder::XZY: {
		float sinp = -2 * (q.w * q.x - q.y * q.z);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosy_cosp = 1 - 2 * (q.z * q.z + q.x * q.x);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		float sinr_cosp = 2 * (q.w * q.y - q.x * q.z);
		float cosr_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}

	default:
		break;
	}

	return angles;
}

Matrix4 ChoMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4 result;
	Matrix4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4 rotateXYZMatrix = Multiply(Multiply(rotateZMatrix, rotateYMatrix), rotateXMatrix);

	Matrix4 translateMatrix = MakeTranslateMatrix(translate);
	//result = Multiply(rotateXYZMatrix, Multiply(scaleMatrix, translateMatrix));
	result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	//result = Multiply(translateMatrix, Multiply(scaleMatrix, rotateXYZMatrix));
	result = Multiply(Multiply(scaleMatrix, rotateXYZMatrix), translateMatrix);
	return result;
}

Matrix4 ChoMath::MakeAffineMatrix(const Scale& scale, const Quaternion& rotate, const Vector3& translate)
{
	Matrix4 result;
	Matrix4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4 rotateMatrix = MakeRotateMatrix(rotate);
	Matrix4 translateMatrix = MakeTranslateMatrix(translate);
	result = scaleMatrix * rotateMatrix * translateMatrix;
	return result;
}