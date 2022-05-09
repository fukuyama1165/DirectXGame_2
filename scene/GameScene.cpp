#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

const float PI = 3.141592653589;

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	//delete debugCamera_;

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
	//debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向補油時が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	siten = Vector3();
	syuten = Vector3(20.0f, 20.0f, 20.0f);
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			
		}
	}


	//ワールドトランスフォームの初期化
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{

			//ワールドトランスフォームの位置変更
			worldTransform_[i][j].translation_ = { (float)j * 4-15 ,(float)i * 4-15 ,0.0f };
			worldTransform_[i][j].Initialize();



			//スケーリング行列を宣言
			Matrix4 matScale;

			//スケーリング倍率を行列に設定
			matScale.m[0][0] = worldTransform_[i][j].scale_.x;
			matScale.m[1][1] = worldTransform_[i][j].scale_.y;
			matScale.m[2][2] = worldTransform_[i][j].scale_.z;
			matScale.m[3][3] = 1;

			//X軸回転行列を宣言
			Matrix4 matRotateX;

			//回転角を行列に設定(ラジアン)
			matRotateX.m[0][0] = 1;
			matRotateX.m[1][1] = cos(worldTransform_[i][j].rotation_.x);
			matRotateX.m[1][2] = sin(worldTransform_[i][j].rotation_.x);
			matRotateX.m[2][1] = -sin(worldTransform_[i][j].rotation_.x);
			matRotateX.m[2][2] = cos(worldTransform_[i][j].rotation_.x);
			matRotateX.m[3][3] = 1;

			//Y軸回転行列を宣言
			Matrix4 matRotateY;

			//回転角を行列に設定(ラジアン)
			matRotateY.m[0][0] = cos(worldTransform_[i][j].rotation_.y);
			matRotateY.m[0][2] = -sin(worldTransform_[i][j].rotation_.y);
			matRotateY.m[1][1] = 1;
			matRotateY.m[2][0] = sin(worldTransform_[i][j].rotation_.y);
			matRotateY.m[2][2] = cos(worldTransform_[i][j].rotation_.y);
			matRotateY.m[3][3] = 1;

			//Z軸回転行列を宣言
			Matrix4 matRotateZ;

			//回転角を行列に設定(ラジアン)
			matRotateZ.m[0][0] = cos(worldTransform_[i][j].rotation_.z);
			matRotateZ.m[0][1] = sin(worldTransform_[i][j].rotation_.z);
			matRotateZ.m[1][0] = -sin(worldTransform_[i][j].rotation_.z);
			matRotateZ.m[1][1] = cos(worldTransform_[i][j].rotation_.z);
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
			matMove.m[3][0] = worldTransform_[i][j].translation_.x;
			matMove.m[3][1] = worldTransform_[i][j].translation_.y;
			matMove.m[3][2] = worldTransform_[i][j].translation_.z;

			//単位行列を代入
			worldTransform_[i][j].matWorld_ = MathUtility::Matrix4Identity();

			//*ができなかったので合成

			matrotate *= matMove;

			matScale *= matrotate;

			//掛け算をして代入
			worldTransform_[i][j].matWorld_ *= matScale;

			//行列の転送
			worldTransform_[i][j].TransferMatrix();


		}
	}
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	


}

void GameScene::Update() 
{
	//debugCamera_->Update();
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

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (i % 2 == 0)
			{
				model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
			}
			else if (j % 2 == 0)
			{
				model_->Draw(worldTransform_[i][j], viewProjection_, textureHandle_);
			}
		}
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
