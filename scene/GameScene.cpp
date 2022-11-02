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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//2Dなので使えない
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

//夏の残り↓
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
//ここまで