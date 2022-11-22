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
	void Initialize(Model* model, Model* homdmodel,const Vector3& position);

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

	void setRotate(Vector3 rotate);

	//handのパンチをするためのフラグを変更
	void setisAttackFlagL(bool flag,Vector3 player);

	//プレス攻撃をするためのフラグを変更
	void setisBossPress(bool flag);

	//石落としをするためのフラグを変更
	void setisBossStoneFall(bool flag);
	void setisBossPillarFall(bool flag);
	

	void setisBossBeam(bool flag);

	void setisBossPillarRoll(bool flag);

	//その場でパンチの攻撃を跳ね返したことにする
	void playerAttackReturnL();

	//プレス攻撃
	void bossPress(Vector3 player);

	//石落とし
	void bossStoneFall(Vector3 player);
	void bossPillarFall(Vector3 player);

	void bossBeam();

	void bossPillarRoll();

	//pos???????????? 全部持ってくる
	WorldTransform getPos() { return worldTransform; };

	//handを持ってくる
	std::vector<bossHand*> getHand() { return hand; };

	//プレス攻撃をするときの目標座標移動用関数
	void setPressPos();

	//本体と一緒にhandが動くための関数
	void setPressHandPos();
	void setPillarHandPos();
	void setOphanimHandPos();

	void setCubeDefaultPos();
	void setPillarDefaultPos();
	void setOphanimDefaultPos();


	void setPressEnd();



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
	
	int state = pillar;

	float setbossCubeDistance = 6.0f;
	float setbossCubePressDistance = 4.8f;

	float setBossPillarRollDistance = setbossCubeDistance;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;

	bool isHandMove = false;

	bool isBossPress = false;
	bool isBossStoneFall = false;
	bool isBossPillarFall = false;
	bool isBossPillarRoll = false;
	bool isBossBeam = false;

	bool isPressStart = false;
	bool isPressFall = false;
	bool isPressReturn = false;
	bool pressFirstStart = false;
	bool pillarRollFirstStart = false;
	bool isPillarRollEnd = false;
	bool beamFirstStart = false;
	bool beamEndStart = false;

	int pressCount = 0;
	int bossStoneFallCount = 0;
	int bossPillarFallCount = 0;
	int bossBeamCount = 0;

	float pressPosY = 20;

	Vector3 returnPos = {};
	Vector3 targetPos = {};
	Vector3 beamStartPos = {};

	//タイムカウンター
	float timeCount = 0;
	float fallTimeCount = 0;
	float returnTimeCount = 0;
	float moveEndTimeCount = 0;

	float beamFirstMoveCount = 0;
	float beamEndMoveCount = 0;

	//動作時間
	float maxTime = 10.0f;
	float maxfallTime = 10.0f;
	float maxReturnTime = 20.0f;
	float maxMoveEndTime = 5.0f;

	float maxFirstBeamMoveTime = 20.0f;
	float maxEndBeamMoveTime = 20.0f;

	float maxBosspillarMoveTime = 10;

	float maxBosspillarRollTime = 10;
	float maxBosspillarRollEndTime = 600;

	//ウエイト
	float waitTime = 0;

	//待つ時間
	float fallWaitTime = 2;
	float returnWaitTime = 20;

	float bossStoneFallWaitTime = 20;

	float bossPillarFallWaitTime = 20;

	float bossBeamWaitTime = 40;

	float bosspillarMoveTime = 0;

	float bosspillarRollTime = 0;
	float bosspillarRollEndTime = 0;
	
	int bosspillarDefaultPosCount = 0;

	Vector3 pillarDefaultPosRotate[8] =
	{
		{ 0,0,1 },
		{ 1,0,1 },
		{ 1,0,0 },
		{ 1,0,-1 },
		{ 0,0,-1 },
		{ -1,0,-1 },
		{ -1,0,0 },
		{ -1,0,1 },
	};

	float bossOphanimMoveTime = 0;
	float maxBossOphanimMoveTime = 10;
	int bossOphanimDefaultPosCount = 0;

	Vector3 ophanimDefaultPosRotate[8] =
	{
		{ 0,1,0 },
		{ -1,1,0 },
		{ -1,0,0 },
		{ -1,-1,0 },
		{ 0,-1,0 },
		{ 1,-1,0 },
		{ 1,0,0 },
		{ 1,1,0 },
	};

};

