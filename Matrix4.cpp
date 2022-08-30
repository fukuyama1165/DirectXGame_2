#include "Matrix4.h"

Vector3 Matrix4::VectorMat(Vector3 vector, Matrix4 mat)
{
	Vector3 changeVector = { 0,0,0 };
	
	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 Matrix4::VectorMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}

Vector3 Matrix4::PosMat(Vector3 vector, Matrix4 mat)
{

	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 1.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 1.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 1.0f * mat.m[3][2];

	return changeVector;
}


Vector3 Matrix4::PosMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 1.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 1.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 1.0f;

	return changeVector;
}

Vector3 VectorMatDivW(Matrix4 mat, Vector3 pos)
{
	float w = pos.x * mat.m[0][3] + pos.y * mat.m[1][3] + pos.z * mat.m[2][3] + mat.m[3][3];

	Vector3 result =
	{
		(pos.x * mat.m[0][0] + pos.y * mat.m[1][0] + pos.z * mat.m[2][0] + mat.m[3][0]) / w,
		(pos.x * mat.m[0][1] + pos.y * mat.m[1][1] + pos.z * mat.m[2][1] + mat.m[3][1]) / w,
		(pos.x * mat.m[0][2] + pos.y * mat.m[1][2] + pos.z * mat.m[2][2] + mat.m[3][2]) / w
	};

	return result;
}

Matrix4 Matrix4::operator*(Matrix4& mat)
{
	Matrix4 ansMat = Matrix4();

	ansMat.m[0][0] = (m[0][0] * mat.m[0][0]) + (m[0][1] * mat.m[1][0]) + (m[0][2] * mat.m[2][0]) + (m[0][3] * mat.m[3][0]);
	ansMat.m[0][1] = (m[0][0] * mat.m[0][1]) + (m[0][1] * mat.m[1][1]) + (m[0][2] * mat.m[2][1]) + (m[0][3] * mat.m[3][1]);
	ansMat.m[0][2] = (m[0][0] * mat.m[0][2]) + (m[0][1] * mat.m[1][2]) + (m[0][2] * mat.m[2][2]) + (m[0][3] * mat.m[3][2]);
	ansMat.m[0][3] = (m[0][0] * mat.m[0][3]) + (m[0][1] * mat.m[1][3]) + (m[0][2] * mat.m[2][3]) + (m[0][3] * mat.m[3][3]);

	ansMat.m[1][0] = (m[1][0] * mat.m[0][0]) + (m[1][1] * mat.m[1][0]) + (m[1][2] * mat.m[2][0]) + (m[1][3] * mat.m[3][0]);
	ansMat.m[1][1] = (m[1][0] * mat.m[0][1]) + (m[1][1] * mat.m[1][1]) + (m[1][2] * mat.m[2][1]) + (m[1][3] * mat.m[3][1]);
	ansMat.m[1][2] = (m[1][0] * mat.m[0][2]) + (m[1][1] * mat.m[1][2]) + (m[1][2] * mat.m[2][2]) + (m[1][3] * mat.m[3][2]);
	ansMat.m[1][3] = (m[1][0] * mat.m[0][3]) + (m[1][1] * mat.m[1][3]) + (m[1][2] * mat.m[2][3]) + (m[1][3] * mat.m[3][3]);

	ansMat.m[2][0] = (m[2][0] * mat.m[0][0]) + (m[2][1] * mat.m[1][0]) + (m[2][2] * mat.m[2][0]) + (m[2][3] * mat.m[3][0]);
	ansMat.m[2][1] = (m[2][0] * mat.m[0][1]) + (m[2][1] * mat.m[1][1]) + (m[2][2] * mat.m[2][1]) + (m[2][3] * mat.m[3][1]);
	ansMat.m[2][2] = (m[2][0] * mat.m[0][2]) + (m[2][1] * mat.m[1][2]) + (m[2][2] * mat.m[2][2]) + (m[2][3] * mat.m[3][2]);
	ansMat.m[2][3] = (m[2][0] * mat.m[0][3]) + (m[2][1] * mat.m[1][3]) + (m[2][2] * mat.m[2][3]) + (m[2][3] * mat.m[3][3]);

	ansMat.m[3][0] = (m[3][0] * mat.m[0][0]) + (m[3][1] * mat.m[1][0]) + (m[3][2] * mat.m[2][0]) + (m[3][3] * mat.m[3][0]);
	ansMat.m[3][1] = (m[3][0] * mat.m[0][1]) + (m[3][1] * mat.m[1][1]) + (m[3][2] * mat.m[2][1]) + (m[3][3] * mat.m[3][1]);
	ansMat.m[3][2] = (m[3][0] * mat.m[0][2]) + (m[3][1] * mat.m[1][2]) + (m[3][2] * mat.m[2][2]) + (m[3][3] * mat.m[3][2]);
	ansMat.m[3][3] = (m[3][0] * mat.m[0][3]) + (m[3][1] * mat.m[1][3]) + (m[3][2] * mat.m[2][3]) + (m[3][3] * mat.m[3][3]);

	return ansMat;
}