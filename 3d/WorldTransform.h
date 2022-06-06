#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = {1, 1, 1};
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = {0, 0, 0};
	// ローカル座標
	Vector3 translation_ = {0, 0, 0};
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	//移動、回転,大きさ変更をする関数群

	//scaleを行列に入れる関数
	Matrix4 matScaleGeneration();

	//rotation_を行列に入れる関数
	Matrix4 matRotateGeneration();

	//translation_を行列に入れる関数
	Matrix4 matMoveGeneration();

	//scale,rotation_,translation_を行列に入れmatworldにかけていれ転送する関数
	void matWorldGeneration();

private:

	//x軸の回転を行列に入れる関数
	Matrix4 matRotateXGeneration(float rotateX);

	//y軸の回転を行列に入れる関数
	Matrix4 matRotateYGeneration(float rotateY);

	//z軸の回転を行列に入れる関数
	Matrix4 matRotateZGeneration(float rotateZ);

};
