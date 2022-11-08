#include "WorldTransform.h"
#include "LightGroup.h"
#include "DebugText.h"


Matrix4 WorldTransform::matScaleGeneration()
{
	//ƒXƒP[ƒŠƒ“ƒOs—ñ‚ðéŒ¾
	Matrix4 matScale = MathUtility::Matrix4Identity();

	//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ðs—ñ‚ÉÝ’è
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 WorldTransform::matRotateXGeneration(float rotateX)
{
	//XŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateX = MathUtility::Matrix4Identity();

	//‰ñ“]Šp‚ðs—ñ‚ÉÝ’è(ƒ‰ƒWƒAƒ“)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cosf(rotateX);
	matRotateX.m[1][2] = sinf(rotateX);
	matRotateX.m[2][1] = -sinf(rotateX);
	matRotateX.m[2][2] = cosf(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4 WorldTransform::matRotateYGeneration(float rotateY)
{
	//YŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateY = MathUtility::Matrix4Identity();

	//‰ñ“]Šp‚ðs—ñ‚ÉÝ’è(ƒ‰ƒWƒAƒ“)
	matRotateY.m[0][0] = cosf(rotateY);
	matRotateY.m[0][2] = -sinf(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sinf(rotateY);
	matRotateY.m[2][2] = cosf(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4 WorldTransform::matRotateZGeneration(float rotateZ)
{
	//ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateZ = MathUtility::Matrix4Identity();

	//‰ñ“]Šp‚ðs—ñ‚ÉÝ’è(ƒ‰ƒWƒAƒ“)
	matRotateZ.m[0][0] = cosf(rotateZ);
	matRotateZ.m[0][1] = sinf(rotateZ);
	matRotateZ.m[1][0] = -sinf(rotateZ);
	matRotateZ.m[1][1] = cosf(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4 WorldTransform::matRotateGeneration()
{
	//XŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateX = matRotateXGeneration(rotation_.x);

	//YŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateY = matRotateYGeneration(rotation_.y);

	//ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	Matrix4 matRotateZ = matRotateZGeneration(rotation_.z);

	//‰ñ“]Ž²‡¬s—ñ‚ðéŒ¾
	Matrix4 matrotate = MathUtility::Matrix4Identity();

	//ŒvŽZ‚µ‚½Šp“x‚ðŒvŽZ(‡”Ô‚Í‰ñ“]‚³‚¹‚éƒ‚ƒfƒ‹‚É‚æ‚Á‚Ä•Ï‚¦‚é)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	return matrotate;
}

Matrix4 WorldTransform::matMoveGeneration()
{
	//ˆÚ“®‚·‚é‚½‚ß‚Ìs—ñ‚ð—pˆÓ
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//s—ñ‚ÉˆÚ“®—Ê‚ð‘ã“ü
	matMove.m[3][0] = translation_.x;
	matMove.m[3][1] = translation_.y;
	matMove.m[3][2] = translation_.z;

	return matMove;
}

void WorldTransform::matWorldGeneration()
{
	//ƒXƒP[ƒŠƒ“ƒOs—ñ‚ðéŒ¾
	Matrix4 matScale = matScaleGeneration();

	//‰ñ“]Ž²‡¬s—ñ‚ðéŒ¾
	Matrix4 matrotate = matRotateGeneration();

	//ˆÚ“®‚·‚é‚½‚ß‚Ìs—ñ‚ð—pˆÓ
	Matrix4 matMove = matMoveGeneration();

	matWorld_ = MathUtility::Matrix4Identity();

	matrotate *= matMove;

	matScale *= matrotate;

	matWorld_ *= matScale;

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;
	}

	TransferMatrix();

}

Vector3 WorldTransform::matWorldGetPos()
{
	Vector3 ans = {};

	ans.x = matWorld_.m[3][0];
	ans.y = matWorld_.m[3][1];
	ans.z = matWorld_.m[3][2];

	return ans;

}