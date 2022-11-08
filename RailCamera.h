#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"


class RailCamera
{

public:

	RailCamera();
	~RailCamera();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	/// <param name="Rot">回転角(ラジアン)</param>
	void Initialize(Vector3 worldPos,Vector3 Rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Vector3 vector, Matrix4 mat);

	/// <summary>
	/// 行列とベクトルの計算(左側が行列計算の先)
	/// </summary>
	Vector3 VectorMat(Matrix4 mat, Vector3 vector);

	ViewProjection getView();

	Matrix4 getMatWorld();

	Vector3 getForwardVec();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void setPos(Vector3 pos);

	void setRotate(Vector3 rotate);

	void setWorldMat(Matrix4 woeldMat);

private:

	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	DebugText* debugText_ = nullptr;

	Vector3 forward = { 0.0f, 0.0f, 1.0f };

};

