#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"

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

	bool getisAttackFlagL() { return isAttackFlagL; };
	bool getisAttackFlagR() { return isAttackFlagR; };

	bool getisReturnHandL() { return isReturnHandL; };
	bool getisReturnHandR() { return isReturnHandR; };

	void setisAttackFlagL(bool flag);
	void setisAttackFlagR(bool flag);

	//プレイヤーの攻撃によって跳ね返された時１回だけ行う処理
	void playerAttackReturnL();
	void playerAttackReturnR();

	void attackEnd();

private:

	DebugText* debugText_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform;

	//手の座標(もしかしたら処理が同じかもしれないのでクラス化したほうがいいかも)
	WorldTransform leftHand;
	WorldTransform rightHand;

	//手が攻撃中か
	bool isAttackFlagL = false;
	bool isAttackFlagR = false;
	//攻撃が終わって戻っている時か
	bool isAttackReturnFlagL = false;
	bool isAttackReturnFlagR = false;

	//手を跳ね返した時のフラグ
	bool isReturnHandL = false;
	bool isReturnHandR = false;

	
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	float timeCount = 0;
	float returnTimeCount = 0;
	float returnAttackTimeCount = 0;

	float maxTime = 500.0f;
	float maxReturnTime = 500.0f;
	float maxReturnAttackTime = 200.0f;

	Vector3 returnPos = {};
};

