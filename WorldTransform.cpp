#include "WorldTransform.h"
#include "LightGroup.h"
#include "DebugText.h"


Matrix4 WorldTransform::matScaleGeneration(Vector3 scale)
{
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 WorldTransform::matRotateXGeneration(float rotateX)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX;

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(rotateX);
	matRotateX.m[1][2] = sin(rotateX);
	matRotateX.m[2][1] = -sin(rotateX);
	matRotateX.m[2][2] = cos(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4 WorldTransform::matRotateYGeneration(float rotateY)
{
	//Y軸回転行列を宣言
	Matrix4 matRotateY;

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cos(rotateY);
	matRotateY.m[0][2] = -sin(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(rotateY);
	matRotateY.m[2][2] = cos(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4 WorldTransform::matRotateZGeneration(float rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4 matRotateZ;

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cos(rotateZ);
	matRotateZ.m[0][1] = sin(rotateZ);
	matRotateZ.m[1][0] = -sin(rotateZ);
	matRotateZ.m[1][1] = cos(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4 WorldTransform::matRotateGeneration(Vector3 rotate)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4 matrotate;

	//計算した角度を計算(順番は回転させるモデルによって変える)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	return matrotate;
}

Matrix4 WorldTransform::matMoveGeneration(Vector3 move)
{
	//移動するための行列を用意
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//行列に移動量を代入
	matMove.m[3][0] = move.x;
	matMove.m[3][1] = move.y;
	matMove.m[3][2] = move.z;

	return matMove;
}

void WorldTransform::matWorldGeneration(WorldTransform& worldTransform)
{
	//スケーリング行列を宣言
	Matrix4 matScale = matScaleGeneration(worldTransform.scale_);

	//回転軸合成行列を宣言
	Matrix4 matrotate = matRotateGeneration(worldTransform.rotation_);

	//移動するための行列を用意
	Matrix4 matMove = matMoveGeneration(worldTransform.translation_);

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	matrotate *= matMove;

	matScale *= matrotate;

	worldTransform.matWorld_ *= matScale;

	worldTransform.TransferMatrix();

}