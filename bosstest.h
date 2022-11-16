#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "bossHand.h"
#include <memory>
#include <vector>

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
	void Update(Vector3 player);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//ボス本体の座標を変える
	void setPos(Vector3 pos);

	//handのパンチをするためのフラグを変更
	void setisAttackFlagL(bool flag);

	//プレス攻撃をするためのフラグを変更
	void setisBossPress(bool flag);

	//石落としをするためのフラグを変更
	void setisBossStoneFall(bool flag);

	//その場でパンチの攻撃を跳ね返したことにする
	void playerAttackReturnL();

	//プレス攻撃
	void bossPress(Vector3 player);

	//石落とし
	void bossStoneFall(Vector3 player);

	//pos???????????? 全部持ってくる
	WorldTransform getPos() { return worldTransform; };

	//handを持ってくる
	std::vector<bossHand*> getHand() { return hand; };

	//プレス攻撃をするときの目標座標移動用関数
	void setPressPos();

	//本体と一緒にhandが動くための関数
	void setPressHandPos();

	int getHP() { return bossHP; };

	//与えるダメージを入れる(ダメージが確定したら専用で関数を作ること)
	void OnCollision(int damage);

private:

	DebugText* debugText_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform;

	int bossHP = 100;

	//手の座標(もしかしたら処理が同じかもしれないのでクラス化したほうがいいかも)
	/*bossHand hand[8];*/

	//クラス化した
	std::vector<bossHand*> hand;

	enum phase
	{
		Cube,
		pillar,
		ophanim
	};
	
	int state = Cube;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	bool isHandMove = false;

	bool isBossPress = false;
	bool isBossStoneFall = false;

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;

	int pressCount = 0;
	int bossStoneFallCount = 0;

	float pressPosY = 20;

	Vector3 returnPos = {};
	Vector3 targetPos = {};

	//タイムカウンター
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;
	float moveEndTimeCount = 0;

	//動作時間
	float maxTime = 300.0f;
	float maxfallTime = 500.0f;
	float maxReturnTime = 200.0f;
	float maxMoveEndTime = 200.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float fallWaitTime = 20;
	float returnWaitTime = 700;

	float bossStoneFallWaitTime = 120;

};

