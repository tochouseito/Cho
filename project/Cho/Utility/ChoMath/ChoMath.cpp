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
	return Matrix4();
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

Matrix4 ChoMath::Inverse(const Matrix4& m)
{
	Matrix4 result = { 0 };
	/* 逆行列を求める行列用の２次元配列 */
	double mat[MatNum][MatNum];

	/* 逆行列用の２次元配列 */
	double inv[MatNum][MatNum];

	/* 掃き出し法を行う行列 */
	double sweep[MatNum][MatNum * 2];

	int i; /* 行 */
	int j; /* 列 */
	int k; /* 注目対角成分が存在する列 */

	double a; /* 定数倍用 */

	/* 正方行列の各成分をセット */
	mat[0][0] = m.m[0][0]; mat[0][1] = m.m[0][1]; mat[0][2] = m.m[0][2]; mat[0][3] = m.m[0][3];
	mat[1][0] = m.m[1][0]; mat[1][1] = m.m[1][1]; mat[1][2] = m.m[1][2]; mat[1][3] = m.m[1][3];
	mat[2][0] = m.m[2][0]; mat[2][1] = m.m[2][1]; mat[2][2] = m.m[2][2]; mat[2][3] = m.m[2][3];
	mat[3][0] = m.m[3][0]; mat[3][1] = m.m[3][1]; mat[3][2] = m.m[3][2]; mat[3][3] = m.m[3][3];

	for (i = 0; i < MatNum; i++) {
		for (j = 0; j < MatNum; j++) {
			/* sweepの左側に逆行列を求める行列をセット */
			sweep[i][j] = mat[i][j];

			/* sweepの右側に単位行列をセット */
			sweep[i][MatNum + j] = (i == j) ? 1 : 0;
		}
	}


	/* 全ての列の対角成分に対する繰り返し */
	for (k = 0; k < MatNum; k++) {

		/* 最大の絶対値を注目対角成分の絶対値と仮定 */
		double max = fabs(sweep[k][k]);
		int max_i = k;

		/* k列目が最大の絶対値となる行を探す */
		for (i = k + 1; i < MatNum; i++) {
			if (fabs(sweep[i][k]) > max) {
				max = fabs(sweep[i][k]);
				max_i = i;
			}
		}



		/* 操作（１）：k行目とmax_i行目を入れ替える */
		if (k != max_i) {
			for (j = 0; j < MatNum * 2; j++) {
				double tmp = sweep[max_i][j];
				sweep[max_i][j] = sweep[k][j];
				sweep[k][j] = tmp;
			}
		}

		/* sweep[k][k]に掛けると1になる値を求める */
		a = 1 / sweep[k][k];

		/* 操作（２）：k行目をa倍する */
		for (j = 0; j < MatNum * 2; j++) {
			/* これによりsweep[k][k]が1になる */
			sweep[k][j] *= a;
		}

		/* 操作（３）によりk行目以外の行のk列目を0にする */
		for (i = 0; i < MatNum; i++) {
			if (i == k) {
				/* k行目はそのまま */
				continue;
			}

			/* k行目に掛ける値を求める */
			a = -sweep[i][k];

			for (j = 0; j < MatNum * 2; j++) {
				/* i行目にk行目をa倍した行を足す */
				/* これによりsweep[i][k]が0になる */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweepの右半分がmatの逆行列 */
	for (i = 0; i < MatNum; i++) {
		for (j = 0; j < MatNum; j++) {
			inv[i][j] = sweep[i][MatNum + j];
		}
	}

	/* 逆行列invを表示 */
	for (i = 0; i < MatNum; i++) {
		for (j = 0; j < MatNum; j++) {
			result.m[i][j] = float(inv[i][j]);
			//printf("%0.2f, ", inv[i][j]);
		}
		//printf("\n");
	}

	/* 検算 */
	if (Check(mat, inv)) {
		//printf("invはmatの逆行列です！！\n");
	} else {
		//printf("invはmatの逆行列になってません...\n");
	}
	return result;
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


float ChoMath::Norm(const Quaternion& quaternion)
{
	return std::sqrt(quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
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
	Quaternion qVector = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConjugate = { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
	Quaternion qResult = Multiply(Multiply(quaternion, qVector), qConjugate);
	return { qResult.x, qResult.y, qResult.z };
}

Matrix4 ChoMath::MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4 matrix = {};

	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;
	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
	matrix.m[0][1] = 2.0f * (xy - wz);
	matrix.m[0][2] = 2.0f * (xz + wy);
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 2.0f * (xy + wz);
	matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
	matrix.m[1][2] = 2.0f * (yz - wx);
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 2.0f * (xz - wy);
	matrix.m[2][1] = 2.0f * (yz + wx);
	matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	matrix = Transpose(matrix);

	return matrix;
}

float ChoMath::Dot(const Quaternion& q0, const Quaternion& q1)
{
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}
