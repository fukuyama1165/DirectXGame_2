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
	delete debugCamera_;
	delete modelSkydome_;
}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	

	TextureManager::Load("Reticle.png");
	//3Dモデルの生成

	model_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//自キャラの生成
	player_p = new Player();
	
	//自キャラの初期化
	player_p->Initialize(model_, textureHandle_);

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




				viewProjection_.eye = { 0.0f,0.0f,-50.0f };
				viewProjection_.target = { 0.0f,0.0f,0.0f };
				viewProjection_.up = { 0.0f,1.0f,0.0f };



	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	railView.Initialize();


}

void GameScene::Update() 
{
	//debugCamera_->Update();

	

	//cameraSpeed += 0.01f;

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



	//エネミーの更新
	//弾の更新処理

	UpdateEnemyPopCommands();

	for (std::unique_ptr<Enemy>& enemy : enemy_)
	{
		enemy->Update();
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
		railView.matView = debugCamera_->GetViewProjection().matView;
		railView.matProjection = debugCamera_->GetViewProjection().matProjection;

		railView.TransferMatrix();
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

	player_->DrawUI();

	/*debugText_->SetPos(50, 70);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);*/


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

	newEnemy->Initialize(model_, pos, { 0,0,-0.1f });

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