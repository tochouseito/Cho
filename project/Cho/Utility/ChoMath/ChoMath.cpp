#include "PrecompiledHeader.h"
#include "ChoMath.h"


// 行列のチェック関数

int ChoMath::Check(double mat[MatNum][MatNum], double inv[MatNum][MatNum])
{
	double inner_product;
	int i, j, k;
	double ans;
	double diff;
	for (i = 0; i < MatNum; i++) {
		for (j = 0; j < MatNum; j++) {
			inner_product = 0;
			for (k = 0; k < MatNum; k++) {
				inner_product += mat[i][k] * inv[k][j];
			}

			ans = (i == j) ? 1 : 0;
			diff = ans - inner_product;


		}
	}

	return 1;
}

Matrix4 ChoMath::Transpose(const Matrix4& m)
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatNum; ++i) {
		for (int j = 0; j < MatNum; ++j) {
			result.m[j][i] = m.m[i][j];
		}
	}
	return result;
}

Matrix4 ChoMath::MakeIdentity4x4()
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatNum; ++i) {
		result.m[i][i] = 1.0;
	}
	return result;
}

Matrix4 ChoMath::Multiply(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = { 0 };
	for (int i = 0; i < MatNum; ++i) {
		for (int j = 0; j < MatNum; ++j) {
			for (int k = 0; k < MatNum; ++k) {
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
	Matrix4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	return rotateXYZMatrix;
}

Matrix4 ChoMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4 result;
	Matrix4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
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
	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
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
	float cy = std::cos(eulerAngles.y * 0.5f);
	float sy = std::sin(eulerAngles.y * 0.5f);
	float cp = std::cos(eulerAngles.x * 0.5f);
	float sp = std::sin(eulerAngles.x * 0.5f);
	float cr = std::cos(eulerAngles.z * 0.5f);
	float sr = std::sin(eulerAngles.z * 0.5f);

	// 左手座標系の場合、y と z に符号を反転
	return Quaternion{
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		cr * cp * cy + sr * sp * sy
	};
}

// クォータニオンからオイラー角を計算（回転順序を指定）

Vector3 ChoMath::ToEulerAngles(const Quaternion& q, RotationOrder order) {
	Vector3 angles;

	switch (order) {
	case RotationOrder::XYZ: {
		// ピッチ
		float sinp = 2 * (q.w * q.x + q.y * q.z);
		if (std::abs(sinp) >= 1)
			angles.x = std::copysign(PiF / 2, sinp);
		else
			angles.x = std::asin(sinp);

		// ヨー
		float siny_cosp = 2 * (q.w * q.y - q.z * q.x);
		float cosy_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.y = std::atan2(siny_cosp, cosy_cosp);

		// ロール
		float sinr_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosr_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);
		break;
	}
						   // 他の回転順序も同様に実装
	default:
		break;
	}

	return angles;
}


