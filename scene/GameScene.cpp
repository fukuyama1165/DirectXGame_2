﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

const float PI = 3.141592653589;

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;

}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");



	//3Dモデルの生成

	model_ = Model::Create();

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

	//乱数シード生成器
	std::random_device seed_gen;

	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//乱数範囲の指定
	std::uniform_real_distribution<float> Posdist(-10, 10);
	std::uniform_real_distribution<float> rotadist(0, PI);

	//ワールドトランスフォームの初期化

			
	//ワールドトランスフォームの位置変更

	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();

	//脊髄
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0,0,0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

	//上半身

	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];

	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0,3,0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];

	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { -3,0,0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];

	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { 3,0,0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];

	//下半身

	//尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0,-3,0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];

	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { -3,-3,0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];

	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { 3,-3,0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];

	//カメラ視点座標を設定
	//viewProjection_.eye = { 0,0,-10 };

	//カメラ注視点座標を設定
	//viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度指定)
	//viewProjection_.up = { cosf(PI / 4.0f),sinf(PI / 4.0f),0.0f };

	//viewProjection_.fovAngleY = RadianChange(10.0f);

	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();




}

void GameScene::Update()
{
	debugCamera_->Update();

	//視点移動処理
	{
		////視点の移動ベクトル
		//Vector3 move = {};

		//const float kEyeSpeed = 0.2f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_W))
		//{
		//	move.z += kEyeSpeed;
		//}
		//else if (input_->PushKey(DIK_S))
		//{
		//	move.z -= kEyeSpeed;
		//}

		////視点移動(ベクトルの加算)
		//viewProjection_.eye += move;

		////行列の再計算
		//viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 50);
		//debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	}

	//注視点移動処理
	{
		//Vector3 move = {};

		////注視点の移動速さ
		//const float kTargetSpeed = 0.2f;

		////押した方向で移動ベクトルを変更

		//if (input_->PushKey(DIK_LEFT))
		//{
		//	move.x += kTargetSpeed;
		//}
		//else if (input_->PushKey(DIK_RIGHT))
		//{
		//	move.x -= kTargetSpeed;
		//}

		////注視点移動(ベクトルの加算)
		//viewProjection_.target += move;

		////行列の再計算

		//viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 70);
		//debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	}

	//上方向回転処理
	{

		////上方向の回転速さ[ラジアン/frame]
		//const float kUpRotSpeed = 0.05f;

		////押した方向で移動ベクトルを変更
		//if (input_->PushKey(DIK_SPACE))
		//{
		//	viewAngle += kUpRotSpeed;

		//	//2πを超えたら0に戻す
		//	viewAngle = fmodf(viewAngle,PI * 2.0f);

		//}

		////上方向ベクトルを計算(半径1の円周上の座標)
		//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		////行列の再計算
		//viewProjection_.UpdateMatrix();


		////デバック用表示
		//debugText_->SetPos(50, 90);
		//debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	}

	//FoV変更処理
	{
		////上キーで視野角が広がる
		//if (input_->PushKey(DIK_UP))
		//{
		//	viewProjection_.fovAngleY += 0.01;
		//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
		//}
		//else if (input_->PushKey(DIK_DOWN))
		//{
		//	viewProjection_.fovAngleY -= 0.01;
		//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01);
		//}

		//viewProjection_.UpdateMatrix();

		////デバック用表示
		//debugText_->SetPos(50, 110);
		//debugText_->Printf("fovAngle(Degree):%f", AngleChange(viewProjection_.fovAngleY));
	}

	//クリップ距離変更処理
	{
		////上下キーでニアクリップ距離を増減
		//if(input_->PushKey(DIK_UP))
		//{
		//	viewProjection_.nearZ += 0.1;
		//}
		//else if (input_->PushKey(DIK_DOWN))
		//{
		//	viewProjection_.nearZ -= 0.1;
		//}

		//viewProjection_.UpdateMatrix();

		//debugText_->SetPos(50, 130);
		//debugText_->Printf("nearZ:%f", viewProjection_.nearZ);

	}

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		const float kMoveSpeed = 0.5;

		if (input_->PushKey(DIK_RIGHT))
		{
			move.x += kMoveSpeed;
		}
		else if (input_->PushKey(DIK_LEFT))
		{
			move.x -= kMoveSpeed;
		}

		worldTransforms_[PartId::kRoot].translation_ += move;

		matWorldGeneration(worldTransforms_[PartId::kRoot]);

		debugText_->SetPos(50, 50);
		debugText_->Printf("translation:%f %f %f", worldTransforms_[PartId::kRoot].translation_.x, worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);

	}

	//上半身回転処理
	{
		if (input_->PushKey(DIK_J))
		{
			worldTransforms_[PartId::kChest].rotation_.y += 0.05;
		}
		else if (input_->PushKey(DIK_K))
		{
			worldTransforms_[PartId::kChest].rotation_.y -= 0.05;
		}
	}

	//下半身回転処理
	{
		if (input_->PushKey(DIK_U))
		{
			worldTransforms_[PartId::kHip].rotation_.y += 0.05;
		}
		else if (input_->PushKey(DIK_I))
		{
			worldTransforms_[PartId::kHip].rotation_.y -= 0.05;
		}
	}

	//子の更新
	{

		
		matWorldParentGeneration(worldTransforms_[PartId::kSpine]);
		matWorldParentGeneration(worldTransforms_[PartId::kChest]);
		matWorldParentGeneration(worldTransforms_[PartId::kHead]);
		matWorldParentGeneration(worldTransforms_[PartId::kArmL]);
		matWorldParentGeneration(worldTransforms_[PartId::kArmR]);
		matWorldParentGeneration(worldTransforms_[PartId::kHip]);
		matWorldParentGeneration(worldTransforms_[PartId::kLegL]);
		matWorldParentGeneration(worldTransforms_[PartId::kLegR]);

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

	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		if (i == 0 or i == 1)
		{
			continue;
		}

		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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

Matrix4 GameScene::matScaleGeneration(Vector3 scale)
{
	//スケーリング行列を宣言
	Matrix4 matScale;

	//スケーリング倍率を行列に設定
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;
	matScale.m[3][3] = 1;

	return matScale;
}

Matrix4 GameScene::matRotateXGeneration(float rotateX)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX;

	//回転角を行列に設定(ラジアン)
	matRotateX.m[0][0] = 1;
	matRotateX.m[1][1] = cos(rotateX);
	matRotateX.m[1][2] = sin(rotateX);
	matRotateX.m[2][1] = -sin(rotateX);
	matRotateX.m[2][2] = cos(rotateX);
	matRotateX.m[3][3] = 1;

	return matRotateX;
}

Matrix4 GameScene::matRotateYGeneration(float rotateY)
{
	//Y軸回転行列を宣言
	Matrix4 matRotateY;

	//回転角を行列に設定(ラジアン)
	matRotateY.m[0][0] = cos(rotateY);
	matRotateY.m[0][2] = -sin(rotateY);
	matRotateY.m[1][1] = 1;
	matRotateY.m[2][0] = sin(rotateY);
	matRotateY.m[2][2] = cos(rotateY);
	matRotateY.m[3][3] = 1;

	return matRotateY;
}

Matrix4 GameScene::matRotateZGeneration(float rotateZ)
{
	//Z軸回転行列を宣言
	Matrix4 matRotateZ;

	//回転角を行列に設定(ラジアン)
	matRotateZ.m[0][0] = cos(rotateZ);
	matRotateZ.m[0][1] = sin(rotateZ);
	matRotateZ.m[1][0] = -sin(rotateZ);
	matRotateZ.m[1][1] = cos(rotateZ);
	matRotateZ.m[2][2] = 1;
	matRotateZ.m[3][3] = 1;

	return matRotateZ;
}

Matrix4 GameScene::matRotateGeneration(Vector3 rotate)
{
	//X軸回転行列を宣言
	Matrix4 matRotateX = matRotateXGeneration(rotate.x);

	//Y軸回転行列を宣言
	Matrix4 matRotateY = matRotateYGeneration(rotate.y);

	//Z軸回転行列を宣言
	Matrix4 matRotateZ = matRotateZGeneration(rotate.z);

	//回転軸合成行列を宣言
	Matrix4 matrotate;

	//計算した角度を計算(順番は回転させるモデルによって変える)

	matRotateX *= matRotateY;

	matRotateZ *= matRotateX;

	matrotate = matRotateZ;

	return matrotate;
}

Matrix4 GameScene::matMoveGeneration(Vector3 move)
{
	//移動するための行列を用意
	Matrix4 matMove = MathUtility::Matrix4Identity();

	//行列に移動量を代入
	matMove.m[3][0] = move.x;
	matMove.m[3][1] = move.y;
	matMove.m[3][2] = move.z;

	return matMove;
}

void GameScene::matWorldGeneration(WorldTransform& worldTransform)
{
	//スケーリング行列を宣言
	Matrix4 matScale = matScaleGeneration(worldTransform.scale_);

	//回転軸合成行列を宣言
	Matrix4 matrotate = matRotateGeneration(worldTransform.rotation_);

	//移動するための行列を用意
	Matrix4 matMove = matMoveGeneration(worldTransform.translation_);

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	matrotate *= matMove;

	matScale *= matrotate;

	worldTransform.matWorld_ *= matScale;

	worldTransform.TransferMatrix();

}

void GameScene::matWorldParentGeneration(WorldTransform& worldTransform)
{
	//スケーリング行列を宣言
	Matrix4 matScale = matScaleGeneration(worldTransform.scale_);

	//回転軸合成行列を宣言
	Matrix4 matrotate = matRotateGeneration(worldTransform.rotation_);

	//移動するための行列を用意
	Matrix4 matMove = matMoveGeneration(worldTransform.translation_);

	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	matMove *= worldTransform.parent_->matWorld_;

	matrotate *= matMove;

	matScale *= matrotate;

	worldTransform.matWorld_ *= matScale;

	worldTransform.TransferMatrix();

}

float GameScene::RadianChange(float Angle)
{
	return Angle * PI / 180;
}

float GameScene::AngleChange(float Radian)
{
	return Radian / PI * 180;
}