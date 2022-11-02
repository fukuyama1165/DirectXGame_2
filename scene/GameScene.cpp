#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#include <fstream>

const float PI = 3.141592653589f;



GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete playerModel_;
	delete playerbulletModel_;
	delete enemybulletModel_;
	delete enemyModel_;
	delete debugCamera_;
	delete modelSkydome_;
}



void GameScene::Initialize() {

	fps.Init();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	
	/*textureNum_[0] = TextureManager::Load("suuji0.png");
	textureNum_[1] = TextureManager::Load("suuji1.png");
	textureNum_[2] = TextureManager::Load("suuji2.png");
	textureNum_[3] = TextureManager::Load("suuji3.png");
	textureNum_[4] = TextureManager::Load("suuji4.png");
	textureNum_[5] = TextureManager::Load("suuji5.png");
	textureNum_[6] = TextureManager::Load("suuji6.png");
	textureNum_[7] = TextureManager::Load("suuji7.png");
	textureNum_[8] = TextureManager::Load("suuji8.png");
	textureNum_[9] = TextureManager::Load("suuji9.png");

	SAimTexture = TextureManager::Load("aimImage.png");
	SAttackTexture = TextureManager::Load("attackImage.png");
	mouseTexture = TextureManager::Load("mouseimage.png");
	SMoveTexture = TextureManager::Load("moveImage.png");
	keyTexture = TextureManager::Load("movekeyimage.png");
	SOrTexture = TextureManager::Load("orImage.png");
	padButtonATexture = TextureManager::Load("padButtonAimage.png");
	padButtonBTexture = TextureManager::Load("padButtonBimage.png");
	RTriggerTexture = TextureManager::Load("Rtrigger.png");
	SSpaceTexture = TextureManager::Load("SPACEimage.png");
	SStartTexture = TextureManager::Load("STARTimage.png");
	stickLTexture = TextureManager::Load("stickL2.png");
	stickRTexture = TextureManager::Load("stickR2.png");
	STitleTexture = TextureManager::Load("titleImage.png");

	SPlayerTexture = TextureManager::Load("playerImage.png");
	SEnemyTexture = TextureManager::Load("enemyImage.png");
	SBulletTexture = TextureManager::Load("bulletImage.png");
	SHitTexture = TextureManager::Load("hitImage.png");
	SclearTimeTexture = TextureManager::Load("clearTimeImage.png");*/

	//TextureManager::Load("Reticle.png");

	//ScoreTexture = TextureManager::Load("score.png");
	//GoalTexture = TextureManager::Load("GOAL.png");
	//titleTexture = TextureManager::Load("title.png");
	
	/*ScoreImage.reset(Sprite::Create(ScoreTexture, { 0,0 }, { 1,1,1,1 }, { 0.0f,0.0f }));
	GoalImage.reset(Sprite::Create(GoalTexture, { 672,0 }, { 1,1,1,1 }, { 0.0f,0.0f }));
	titleImage.reset(Sprite::Create(titleTexture, { 640,320 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SAimImage.reset(Sprite::Create(SAimTexture, { 1255,500 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SAttackImage.reset(Sprite::Create(SAttackTexture, { 1240,620 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	mouseImage.reset(Sprite::Create(mouseTexture, { 1220,500 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SMoveImage.reset(Sprite::Create(SMoveTexture, { 1245,560 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	keyImage.reset(Sprite::Create(keyTexture, { 1185,560 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SOrImage.reset(Sprite::Create(SOrTexture, { 610,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	padButtonAImage.reset(Sprite::Create(padButtonATexture, { 640,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	padButtonBImage.reset(Sprite::Create(padButtonBTexture, { 640,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	RTriggerImage.reset(Sprite::Create(RTriggerTexture, { 1120,620 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SSpaceImage.reset(Sprite::Create(SSpaceTexture, { 565,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SStartImage.reset(Sprite::Create(SStartTexture, { 690,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	stickLImage.reset(Sprite::Create(stickLTexture, { 1140,560 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	stickRImage.reset(Sprite::Create(stickRTexture, { 1190,500 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	STitleImage.reset(Sprite::Create(STitleTexture, { 690,480 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SPlayerImage.reset(Sprite::Create(SPlayerTexture, { 580,235 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SEnemyImage.reset(Sprite::Create(SEnemyTexture, { 575, 300 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SBulletImage.reset(Sprite::Create(SBulletTexture, { 580, 365 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SHitImage1.reset(Sprite::Create(SHitTexture, { 630,235 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	SHitImage2.reset(Sprite::Create(SHitTexture, { 630, 300 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	SHitImage3.reset(Sprite::Create(SHitTexture, { 630, 365 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	SClearTimeImage.reset(Sprite::Create(SclearTimeTexture, { 620,427 }, { 1,1,1,1 }, { 0.5f,0.5f }));*/

	

	//3Dモデルの生成

	model_ = Model::Create();
	playerModel_ = Model::CreateFromOBJ("player",true);
	enemyModel_ = Model::CreateFromOBJ("enemy",true);
	playerbulletModel_ = Model::CreateFromOBJ("playerbullet",true);
	enemybulletModel_ = Model::CreateFromOBJ("enemybullet",true);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自キャラの生成
	player_p = new Player();
	
	//自キャラの初期化
	player_p->Initialize(playerbulletModel_,playerModel_, textureHandle_);

	//ユニークポインタに登録
	player_.reset(player_p);

	LoadEnemyPopData();


	//敵キャラの生成と初期化
	//std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//
	//newEnemy->SetPlayer(player_p);
	//newEnemy->SetGameScene(this);

	//newEnemy->Initialize(model_, { 0,5,150 }, { 0,0,-0.1f });

	////敵キャラを登録
	//enemy_.push_back(std::move(newEnemy));

	//敵キャラの生成
	//enemy_p = new Enemy();
	//enemy_p->SetPlayer(player_p);

	////敵キャラの初期化
	//enemy_p->Initialize(model_, { 0,5,150 },{0,0,-0.001f});
	//

	////ユニークポインタに登録
	//enemy_.reset(enemy_p);



	//天球の生成
	skydome_p = new Skydome;

	skydome_p->Initialize(modelSkydome_);

	//ユニークポインタに登録
	skydome_.reset(skydome_p);

	//レールカメラの生成
	railCamera_p = new RailCamera;

	railCamera_p->Initialize({ 0.0f,0.0f,-50.0f },{0.0f,0.0f,0.0f});

	//ユニークポインタに登録
	railCamera_.reset(railCamera_p);

	railView = railCamera_->getView();

	player_->SetCameraMat(railCamera_->getMatWorld());

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	scoore.init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	Goal.init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	clearScore[0].init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	clearScore[1].init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	clearScore[2].init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	clearScore[3].init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);
	clear.init(textureNum_[0], textureNum_[1], textureNum_[2], textureNum_[3], textureNum_[4], textureNum_[5], textureNum_[6], textureNum_[7], textureNum_[8], textureNum_[9]);


	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向補油時が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	siten = Vector3();
	syuten = Vector3(20.0f, 20.0f, 20.0f);
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	//ワールドトランスフォームの初期化

				//ワールドトランスフォームの位置変更

	title.Initialize();
	title.translation_={ 5,0,-30 };
	title.rotation_ = { 0,0,135 * PI / 180 };
	title.matWorldGeneration();


				viewProjection_.eye = { 0.0f,0.0f,-50.0f };
				viewProjection_.target = { 0.0f,0.0f,0.0f };
				viewProjection_.up = { 0.0f,1.0f,0.0f };
				viewProjection_.farZ = 1000000.0f;




	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//railView.Initialize();

	scoore.Scorenum = 0;
	scoore.HiScorenum = 10000;

	titleView.Initialize();

	titleView = railView;

	effectM.Init();
	effectM2.Init();
	effectM.setPos({ 10,0,0 });

}

void GameScene::Update() 
{
	fps.Ran();

	debugCamera_->Update();

	//scoore.Scorenum++;

	//cameraSpeed += 0.01f;
	int a;

	XINPUT_STATE joyState = {};

	Input::GetInstance()->GetJoystickState(0, joyState);

	//if (effectM.GetIsEffctEnd() == false)
	//{
	//	effectM.ExplosionEffect(5000);
	//}

	//effectM.Update();

	//if (effectM2.GetIsEffctEnd() == false)
	//{
	//	effectM2.ExplosionEffect(5000);
	//}

	//effectM2.Update();

	/*switch (scene)
	{
	case 0:
		

		if (input_->TriggerKey(DIK_SPACE) or joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			scene = 1;
		}

		break;
	case 1:
		railCamera_->setPos({ 0.0f,0.0f,-20 + cameraSpeed });
		railCamera_->Update();
		player_->SetCameraMat(railCamera_->getMatWorld());

		//ぬるぽチェック
		assert(player_);
		assert(player_p);

		//プレイヤーの更新
		player_->Update(railCamera_->getView());

		//ぬるぽチェック

		//assert(enemy_p);

		if (input_->TriggerKey(DIK_R))
		{
			resetGame();
		}

		//エネミーの更新
		//弾の更新処理

		UpdateEnemyPopCommands();

		for (std::unique_ptr<Enemy>& enemy : enemy_)
		{
			enemy->Update();
			if (enemy->IsDead())
			{
				scoore.Scorenum += 300;

				enemyHitNum++;
			}
		}
		//enemy_->Update();

		

		//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
		enemy_.remove_if([](std::unique_ptr<Enemy>& enemy)//ifの中で簡易的な関数を生成してる->[](引数)
			{
				return enemy->IsDead();
			});

		//デスフラグの立った弾を削除(remove_if->条件一致を全て削除)
		enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
			{
				return bullet->IsDead();
			});

		//弾の更新処理
		for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
		{
			enemyBullet->Update();
		}

		CheckAllCollision();

		skydome_->Update();

		if (scoore.Scorenum >= 10000)
		{
			scene = 2;
		}
		clearTimer++;

		break;
	case 2:

		if (playerHitNum > 0)
		{
			playerHitNum--;

			clearPlayerHitNum++;

		}
		else if (enemyHitNum > 0)
		{

			enemyHitNum--;

			clearEnemyHitNum++;

		}
		else if (bulletHitNum > 0)
		{

			bulletHitNum--;

			clearBulletHitNum++;

		}


		if ((input_->TriggerKey(DIK_SPACE) or joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) and bulletHitNum<=0)
		{
			resetGame();
			scene = 0;
			playerHitNum = 0;
			enemyHitNum = 0;
			bulletHitNum = 0;

			clearScene = 0;

			clearPlayerHitNum = 0;
			clearEnemyHitNum = 0;
			clearBulletHitNum = 0;
			clearTimer = 0;

			enemy_.remove_if([](std::unique_ptr<Enemy>& enemy)//ifの中で簡易的な関数を生成してる->[](引数)
			{
				return 1;
			});

			//弾を削除(remove_if->条件一致を全て削除)
			enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)//ifの中で簡易的な関数を生成してる->[](引数)
			{
				return 1;
			});

			player_->reset();
		}

		break;
	default:
		break;
	}*/

	
	
	

#ifdef _DEBUG

	//デバックで起動していると使える
	if (input_->TriggerKey(DIK_P))
	{
		IsdebugCameraUse = !IsdebugCameraUse;
	}

#endif 

	if (IsdebugCameraUse)
	{
		debugCamera_->Update();
		//デバックカメラがONになっているならviewProjectionをデバックカメラに
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		debugCamera_->SetDistance(10000);
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}
	else
	{
		viewProjection_.UpdateMatrix();

		viewProjection_.TransferMatrix();
	}

	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	/// 	//3Dモデル描画
	
				//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	/*switch (scene)
	{
	case 0:
		
		skydome_->Draw(railView);
		playerModel_->Draw(title, titleView);

		break;
	case 1:
		railView = railCamera_->getView();

		skydome_->Draw(railView);

		player_->Draw(railView);

		for (std::unique_ptr<Enemy>& enemy : enemy_)
		{
			enemy->Draw(railView);
		}
		//enemy_->Draw(railView);

		//生成された弾を描画
		for (std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets_)
		{
			enemyBullet->Draw(railView);
		}
		break;
	case 2:
		skydome_->Draw(railView);
		break;
	default:
		break;
	};*/
	
	skydome_->Draw(viewProjection_);

	//model_->Draw(worldTransform_, viewProjection_);
	//effectM.draw(viewProjection_);
	//effectM2.draw(viewProjection_);
	///

	//PrimitiveDrawer::GetInstance()->DrawLine3d(siten, syuten, color);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	/*switch (scene)
	{
	case 0:
		
		titleImage->Draw();

		SSpaceImage->SetPosition({ 565,480 });
		SSpaceImage->Draw();
		padButtonAImage->Draw();

		SOrImage->SetPosition({ 610,480 });
		SOrImage->Draw();
		SStartImage->Draw();


		break;
	case 1:
		player_->DrawUI();
		SAimImage->Draw();
		SAttackImage->Draw();
		mouseImage->Draw();
		SMoveImage->Draw();
		keyImage->Draw();
		RTriggerImage->Draw();
		SSpaceImage->SetPosition({1170,620});
		SSpaceImage->Draw();
		stickLImage->Draw();
		stickRImage->Draw();

		scoore.graphNumberDisplayScore(64, 0, scoore.Scorenum, 5);
		ScoreImage->SetPosition({ 0,0 });
		ScoreImage->Draw();
		Goal.graphNumberDisplayScore(736, 0, scoore.HiScorenum, 5);
		GoalImage->Draw();
		break;
	case 2:

		ScoreImage->SetPosition({ 580,120 });
		ScoreImage->Draw();

		SPlayerImage->Draw();

		SEnemyImage->Draw();

		SBulletImage->Draw();

		SHitImage1->Draw();
		SHitImage2->Draw();
		SHitImage3->Draw();

		SClearTimeImage->Draw();

		if (bulletHitNum <= 0)
		{
			SSpaceImage->SetPosition({ 565,480 });
			SSpaceImage->Draw();

			SOrImage->SetPosition({ 610,480 });
			SOrImage->Draw();

			padButtonBImage->Draw();

			STitleImage->Draw();
		}

		scoore.graphNumberDisplayScore(640, 120, scoore.Scorenum, 5);
		clearScore[0].graphNumberDisplayScore(640, 220, clearPlayerHitNum, 5);
		clearScore[1].graphNumberDisplayScore(640, 284, clearEnemyHitNum, 5);
		clearScore[2].graphNumberDisplayScore(640, 348, clearBulletHitNum, 5);
		clear.graphNumberDisplayScore(672, 412, clearTimer/60, 4);

		break;
	default:
		break;
	};*/

	/*debugText_->SetPos(50, 70);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);*/

	//fps.FPSDraw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::afin(WorldTransform Transform)
{
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = Transform.scale_.x;
	matScale.m[1][1] = Transform.scale_.y;
	matScale.m[2][2] = Transform.scale_.z;
	matScale.m[3][3] = 1;

	//X軸回転行列を宣言
	Matrix4 matRotateX;

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(Transform.rotation_.x);
	matRotateX.m[1][2] = sin(Transform.rotation_.x);
	matRotateX.m[2][1] = -sin(Transform.rotation_.x);
	matRotateX.m[2][2] = cos(Transform.rotation_.x);
	matRotateX.m[3][3] = 1;

	//Y軸回転行列を宣言
	Matrix4 matRotateY;

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cos(Transform.rotation_.y);
	matRotateY.m[0][2] = -sin(Transform.rotation_.y);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(Transform.rotation_.y);
	matRotateY.m[2][2] = cos(Transform.rotation_.y);
	matRotateY.m[3][3] = 1;

	//Z軸回転行列を宣言
	Matrix4 matRotateZ;

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cos(Transform.rotation_.z);
	matRotateZ.m[0][1] = sin(Transform.rotation_.z);
	matRotateZ.m[1][0] = -sin(Transform.rotation_.z);
	matRotateZ.m[1][1] = cos(Transform.rotation_.z);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	//回転軸合成行列を宣言
	Matrix4 matrotate;

	//計算した角度を計算(順番は回転させるモデルによって変える)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	//移動するための行列を用意
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//行列に移動量を代入
	matMove.m[3][0] = Transform.translation_.x;
	matMove.m[3][1] = Transform.translation_.y;
	matMove.m[3][2] = Transform.translation_.z;

	//単位行列を代入
	Transform.matWorld_ = MathUtility::Matrix4Identity();

	//*ができなかったので合成

	matrotate *= matMove;

	matScale *= matrotate;

	//掛け算をして代入
	Transform.matWorld_ *= matScale;

	//行列の転送
	Transform.TransferMatrix();
}

void GameScene::CheckAllCollision()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_.get()->GetBullets();

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemyBullets_;

#pragma region 自キャラと敵弾の当たり判定

	posA = player_->GetWorldPosition();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		posB = bullet->GetWorldPosition();

		float len = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

		//半径が分からなかったので適当
		if (len <= ((1 + 1) * (1 + 1)))
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();

			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

			scoore.Scorenum -= 10;

			playerHitNum++;
			

		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		posA = enemy->GetWorldPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			posB = bullet->GetWorldPosition();

			float len = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

			//半径が分からなかったので適当
			if (len <= ((1 + 1) * (1 + 1)))
			{
				//自キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();

				//敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();

				
				

			}

		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		posA = bullet->GetWorldPosition();

		for (const std::unique_ptr<PlayerBullet>& bullet2 : playerBullets)
		{
			posB = bullet2->GetWorldPosition();

			float len = ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) + ((posB.z - posA.z) * (posB.z - posA.z));

			//半径が分からなかったので適当
			if (len <= ((1 + 1) * (1 + 1)))
			{
				//自キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();

				//敵弾の衝突時コールバックを呼び出す
				bullet2->OnCollision();

				scoore.Scorenum+=5;
				bulletHitNum++;

			}

		}
	}

#pragma endregion

	
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//リストに追加する
	enemyBullets_.push_back(std::move(enemyBullet));

}

void GameScene::LoadEnemyPopData()
{

	//ファイルを開く
	std::ifstream file;
	file.open("Resources\\Enemypop.csv");

	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	file.close();

}

void GameScene::PopEnemy(Vector3 pos)
{
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	newEnemy->SetPlayer(player_p);
	newEnemy->SetGameScene(this);

	newEnemy->Initialize(enemybulletModel_,enemyModel_, pos, { 0,0,-0.1f });

	//敵キャラを登録
	enemy_.push_back(std::move(newEnemy));
}

void GameScene::UpdateEnemyPopCommands()
{

	//待機処理
	if (isWait)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			isWait = false;
		}

		return;
	}


	//一行分の文字列を入れる関数
	std::string line;

	while (getline(enemyPopCommands, line))
	{

		//一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{

			//コメント行を飛ばす
			continue;

		}

		//POPコマンド
		if (word.find("POP") == 0)
		{

			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			PopEnemy(Vector3(x, y, z));

		}
		else if (word.find("WAIT") == 0)
		{
			
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;

		}
		

	}

}

void GameScene::resetGame()
{
	scoore.Scorenum = 0;

	enemyPopCommands.str("");
	enemyPopCommands.clear(std::stringstream::goodbit);
	LoadEnemyPopData();

}