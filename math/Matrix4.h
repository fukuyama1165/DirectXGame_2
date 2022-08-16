#pragma once
#include "Vector4.h"
#include "Vector3.h"
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
  public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
	  float m00, float m01, float m02, float m03,
	  float m10, float m11, float m12, float m13,
	  float m20, float m21, float m22, float m23,
	  float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	Matrix4 operator*(Matrix4& mat);

	Vector3 VectorMatDivW(Matrix4 mat, Vector3 pos);

	Vector3 VectorMat(Vector3 vector, Matrix4 mat);
	

	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	Vector3 PosMat(Vector3 vector, Matrix4 mat);


	Vector3 PosMat(Matrix4 mat, Vector3 vector);
	

};
