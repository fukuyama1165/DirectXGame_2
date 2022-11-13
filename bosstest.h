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

	void setPos(Vector3 pos);

	void setisAttackFlagL(bool flag);

	void setisBossPress(bool flag);

	void playerAttackReturnL();

	void bossPress(Vector3 player);

	WorldTransform getPos() { return worldTransform; };

	std::vector<bossHand*> getHand() { return hand; };

	void setPressPos();

	void endPress();

	void setPressHandPos();

private:

	DebugText* debugText_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform;

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

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;

	int pressCount = 0;

	float pressPosY = 20;

	Vector3 returnPos = {};
	Vector3 targetPos = {};

	//タイムカウンター
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;

	//動作時間
	float maxTime = 300.0f;
	float maxfallTime = 500.0f;
	float maxReturnTime = 200.0f;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float fallWaitTime = 20;
	float returnWaitTime = 700;

};

