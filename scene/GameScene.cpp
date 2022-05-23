#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

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



	//ワールドトランスフォームの初期化

				//ワールドトランスフォームの位置変更
				worldTransform_.translation_ = {0.0f  ,0.0f ,0.0f};
				worldTransform_.scale_ = {1.0f  ,1.0f ,1.0f};
				worldTransform_.rotation_ = {0.0f ,0.0f ,0.0f};
				worldTransform_.Initialize();

				matWorldGeneration(worldTransform_);

				////スケーリング行列を宣言
				//Matrix4 matScale;

				////スケーリング倍率を行列に設定
				//matScale.m[0][0] = worldTransform_.scale_.x;
				//matScale.m[1][1] = worldTransform_.scale_.y;
				//matScale.m[2][2] = worldTransform_.scale_.z;
				//matScale.m[3][3] = 1;

				////X軸回転行列を宣言
				//Matrix4 matRotateX;

				////回転角を行列に設定(ラジアン)
				//matRotateX.m[0][0] = 1;
				//matRotateX.m[1][1] = cos(worldTransform_.rotation_.x);
				//matRotateX.m[1][2] = sin(worldTransform_.rotation_.x);
				//matRotateX.m[2][1] = -sin(worldTransform_.rotation_.x);
				//matRotateX.m[2][2] = cos(worldTransform_.rotation_.x);
				//matRotateX.m[3][3] = 1;

				////Y軸回転行列を宣言
				//Matrix4 matRotateY;

				////回転角を行列に設定(ラジアン)
				//matRotateY.m[0][0] = cos(worldTransform_.rotation_.y);
				//matRotateY.m[0][2] = -sin(worldTransform_.rotation_.y);
				//matRotateY.m[1][1] = 1;
				//matRotateY.m[2][0] = sin(worldTransform_.rotation_.y);
				//matRotateY.m[2][2] = cos(worldTransform_.rotation_.y);
				//matRotateY.m[3][3] = 1;

				////Z軸回転行列を宣言
				//Matrix4 matRotateZ;

				////回転角を行列に設定(ラジアン)
				//matRotateZ.m[0][0] = cos(worldTransform_.rotation_.z);
				//matRotateZ.m[0][1] = sin(worldTransform_.rotation_.z);
				//matRotateZ.m[1][0] = -sin(worldTransform_.rotation_.z);
				//matRotateZ.m[1][1] = cos(worldTransform_.rotation_.z);
				//matRotateZ.m[2][2] = 1;
				//matRotateZ.m[3][3] = 1;

				////回転軸合成行列を宣言
				//Matrix4 matrotate;

				////計算した角度を計算(順番は回転させるモデルによって変える)

				//matRotateX *= matRotateY;

				//matRotateZ *= matRotateX;

				//matrotate = matRotateZ;

				////移動するための行列を用意
				//Matrix4 matMove = MathUtility::Matrix4Identity();

				////行列に移動量を代入
				//matMove.m[3][0] = worldTransform_.translation_.x;
				//matMove.m[3][1] = worldTransform_.translation_.y;
				//matMove.m[3][2] = worldTransform_.translation_.z;

				////単位行列を代入
				//worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

				////*ができなかったので合成

				//matrotate *= matMove;

				//matScale *= matrotate;

				////掛け算をして代入
				//worldTransform_.matWorld_ *= matScale;

				////行列の転送
				//worldTransform_.TransferMatrix();


				viewProjection_.eye = { -5.0f,-7.0f,6.0f };
				viewProjection_.target = { 0.0f,0.0f,0.0f };
				viewProjection_.up = { 0.0f,1.0f,0.0f };

			
		
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();



}

void GameScene::Update() 
{
	//debugCamera_->Update();

	Cameramove+=0.01;

	viewProjection_.eye = { cosf(Cameramove)*10,0.0f,sinf(Cameramove)*10 };

	viewProjection_.UpdateMatrix();

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

	
				model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
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

	debugText_->SetPos(50, 70);
	debugText_->Printf("eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf("target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf("up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	

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
	matMove.m[3][0] = worldTransform_.translation_.x;
	matMove.m[3][1] = worldTransform_.translation_.y;
	matMove.m[3][2] = worldTransform_.translation_.z;

	return matMove;
}

void GameScene::matWorldGeneration(WorldTransform worldTransform)
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