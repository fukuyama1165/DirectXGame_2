#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "bossHand.h"

class bosstest
{
public:
	bosstest();
	~bosstest();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	void setPos(Vector3 pos);

	void setisAttackFlagL(bool flag);

	void playerAttackReturnL();

private:

	DebugText* debugText_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform;

	//手の座標(もしかしたら処理が同じかもしれないのでクラス化したほうがいいかも)
	bossHand Hand[8];

	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;


};

