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
	delete move;
	delete scale;
	delete rotateX;
	delete rotateY;
	delete rotateZ;

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
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	siten = Vector3(0.0f, 0.0f, 0.0f);
	syuten = Vector3(20.0f, 20.0f, 20.0f);
	color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	color2 = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	color3 = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	color4 = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	color5 = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
	color6 = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	color7 = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
	color8 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	move = new Matrix4(1.0f, 0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	scale = new Matrix4(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	rotateX = new Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos(45 * (PI / 180)), -sin(45 * (PI / 180)), 0.0f, 0.0f, sin(45 * (PI / 180)), cos(45 * (PI / 180)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	rotateY = new Matrix4(cos(45 * (PI / 180)), 0.0f, sin(45 * (PI / 180)), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(45 * (PI / 180)), 0.0f, cos(45 * (PI / 180)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	rotateZ = new Matrix4(cos(45 * (PI / 180)), -sin(45 * (PI / 180)), 0.0f, 0.0f, sin(45 * (PI / 180)), cos(45 * (PI / 180)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	boxbuff[0] = vertex[0].x;
	boxbuff[1] = vertex[0].y;
	boxbuff[2] = vertex[0].z;
	boxbuff[3] = 1.0f;

	for (int i = 0; i < 8; i++)
	{
		//原点に移動
		boxOriginMoveBuff[0] = move->m[0][0] * vertex[i].x + move->m[0][1] * vertex[i].y + move->m[0][2] * vertex[i].z + -boxbuff[0] * 1.0f;
		boxOriginMoveBuff[1] = move->m[1][0] * vertex[i].x + move->m[1][1] * vertex[i].y + move->m[1][2] * vertex[i].z + -boxbuff[1] * 1.0f;
		boxOriginMoveBuff[2] = move->m[2][0] * vertex[i].x + move->m[2][1] * vertex[i].y + move->m[2][2] * vertex[i].z + -boxbuff[2] * 1.0f;
		boxOriginMoveBuff[3] = move->m[3][0] * vertex[i].x + move->m[3][1] * vertex[i].y + move->m[3][2] * vertex[i].z + boxbuff[3] * 1.0f;

		//大きさを変更するところ
		boxScaleBuff[0] = scale->m[0][0] * boxOriginMoveBuff[0] + scale->m[0][1] * boxOriginMoveBuff[1] + scale->m[0][2] * boxOriginMoveBuff[2] + scale->m[0][3] * boxOriginMoveBuff[3];
		boxScaleBuff[1] = scale->m[1][0] * boxOriginMoveBuff[0] + scale->m[1][1] * boxOriginMoveBuff[1] + scale->m[1][2] * boxOriginMoveBuff[2] + scale->m[1][3] * boxOriginMoveBuff[3];
		boxScaleBuff[2] = scale->m[2][0] * boxOriginMoveBuff[0] + scale->m[2][1] * boxOriginMoveBuff[1] + scale->m[2][2] * boxOriginMoveBuff[2] + scale->m[2][3] * boxOriginMoveBuff[3];
		boxScaleBuff[3] = scale->m[3][0] * boxOriginMoveBuff[0] + scale->m[3][1] * boxOriginMoveBuff[1] + scale->m[3][2] * boxOriginMoveBuff[2] + scale->m[3][3] * boxOriginMoveBuff[3];

		//X軸方向に回転するところ
		boxRotateXBuff[0] = rotateX->m[0][0] * boxOriginMoveBuff[0] + rotateX->m[0][1] * boxOriginMoveBuff[1] + rotateX->m[0][2] * boxOriginMoveBuff[2] + rotateX->m[0][3] * boxOriginMoveBuff[3];
		boxRotateXBuff[1] = rotateX->m[1][0] * boxOriginMoveBuff[0] + rotateX->m[1][1] * boxOriginMoveBuff[1] + rotateX->m[1][2] * boxOriginMoveBuff[2] + rotateX->m[1][3] * boxOriginMoveBuff[3];
		boxRotateXBuff[2] = rotateX->m[2][0] * boxOriginMoveBuff[0] + rotateX->m[2][1] * boxOriginMoveBuff[1] + rotateX->m[2][2] * boxOriginMoveBuff[2] + rotateX->m[2][3] * boxOriginMoveBuff[3];
		boxRotateXBuff[3] = rotateX->m[3][0] * boxOriginMoveBuff[0] + rotateX->m[3][1] * boxOriginMoveBuff[1] + rotateX->m[3][2] * boxOriginMoveBuff[2] + rotateX->m[3][3] * boxOriginMoveBuff[3];

		//Y軸方向に回転するところ
		boxRotateYBuff[0] = rotateY->m[0][0] * boxOriginMoveBuff[0] + rotateY->m[0][1] * boxOriginMoveBuff[1] + rotateY->m[0][2] * boxOriginMoveBuff[2] + rotateY->m[0][3] * boxOriginMoveBuff[3];
		boxRotateYBuff[1] = rotateY->m[1][0] * boxOriginMoveBuff[0] + rotateY->m[1][1] * boxOriginMoveBuff[1] + rotateY->m[1][2] * boxOriginMoveBuff[2] + rotateY->m[1][3] * boxOriginMoveBuff[3];
		boxRotateYBuff[2] = rotateY->m[2][0] * boxOriginMoveBuff[0] + rotateY->m[2][1] * boxOriginMoveBuff[1] + rotateY->m[2][2] * boxOriginMoveBuff[2] + rotateY->m[2][3] * boxOriginMoveBuff[3];
		boxRotateYBuff[3] = rotateY->m[3][0] * boxOriginMoveBuff[0] + rotateY->m[3][1] * boxOriginMoveBuff[1] + rotateY->m[3][2] * boxOriginMoveBuff[2] + rotateY->m[3][3] * boxOriginMoveBuff[3];

		//Z軸方向に回転するところ
		boxRotateZBuff[0] = rotateZ->m[0][0] * boxOriginMoveBuff[0] + rotateZ->m[0][1] * boxOriginMoveBuff[1] + rotateZ->m[0][2] * boxOriginMoveBuff[2] + rotateZ->m[0][3] * boxOriginMoveBuff[3];
		boxRotateZBuff[1] = rotateZ->m[1][0] * boxOriginMoveBuff[0] + rotateZ->m[1][1] * boxOriginMoveBuff[1] + rotateZ->m[1][2] * boxOriginMoveBuff[2] + rotateZ->m[1][3] * boxOriginMoveBuff[3];
		boxRotateZBuff[2] = rotateZ->m[2][0] * boxOriginMoveBuff[0] + rotateZ->m[2][1] * boxOriginMoveBuff[1] + rotateZ->m[2][2] * boxOriginMoveBuff[2] + rotateZ->m[2][3] * boxOriginMoveBuff[3];
		boxRotateZBuff[3] = rotateZ->m[3][0] * boxOriginMoveBuff[0] + rotateZ->m[3][1] * boxOriginMoveBuff[1] + rotateZ->m[3][2] * boxOriginMoveBuff[2] + rotateZ->m[3][3] * boxOriginMoveBuff[3];

		//大きさを変えつつX軸方向に回転するところ
		boxScaleRotateXBuff[0] = rotateX->m[0][0] * boxScaleBuff[0] + rotateX->m[0][1] * boxScaleBuff[1] + rotateX->m[0][2] * boxScaleBuff[2] + rotateX->m[0][3] * boxScaleBuff[3];
		boxScaleRotateXBuff[1] = rotateX->m[1][0] * boxScaleBuff[0] + rotateX->m[1][1] * boxScaleBuff[1] + rotateX->m[1][2] * boxScaleBuff[2] + rotateX->m[1][3] * boxScaleBuff[3];
		boxScaleRotateXBuff[2] = rotateX->m[2][0] * boxScaleBuff[0] + rotateX->m[2][1] * boxScaleBuff[1] + rotateX->m[2][2] * boxScaleBuff[2] + rotateX->m[2][3] * boxScaleBuff[3];
		boxScaleRotateXBuff[3] = rotateX->m[3][0] * boxScaleBuff[0] + rotateX->m[3][1] * boxScaleBuff[1] + rotateX->m[3][2] * boxScaleBuff[2] + rotateX->m[3][3] * boxScaleBuff[3];

		//大きさを変えつつY軸方向に回転するところ
		boxScaleRotateYBuff[0] = rotateY->m[0][0] * boxScaleBuff[0] + rotateY->m[0][1] * boxScaleBuff[1] + rotateY->m[0][2] * boxScaleBuff[2] + rotateY->m[0][3] * boxScaleBuff[3];
		boxScaleRotateYBuff[1] = rotateY->m[1][0] * boxScaleBuff[0] + rotateY->m[1][1] * boxScaleBuff[1] + rotateY->m[1][2] * boxScaleBuff[2] + rotateY->m[1][3] * boxScaleBuff[3];
		boxScaleRotateYBuff[2] = rotateY->m[2][0] * boxScaleBuff[0] + rotateY->m[2][1] * boxScaleBuff[1] + rotateY->m[2][2] * boxScaleBuff[2] + rotateY->m[2][3] * boxScaleBuff[3];
		boxScaleRotateYBuff[3] = rotateY->m[3][0] * boxScaleBuff[0] + rotateY->m[3][1] * boxScaleBuff[1] + rotateY->m[3][2] * boxScaleBuff[2] + rotateY->m[3][3] * boxScaleBuff[3];

		//大きさを変えつつZ軸方向に回転するところ
		boxScaleRotateZBuff[0] = rotateZ->m[0][0] * boxScaleBuff[0] + rotateZ->m[0][1] * boxScaleBuff[1] + rotateZ->m[0][2] * boxScaleBuff[2] + rotateZ->m[0][3] * boxScaleBuff[3];
		boxScaleRotateZBuff[1] = rotateZ->m[1][0] * boxScaleBuff[0] + rotateZ->m[1][1] * boxScaleBuff[1] + rotateZ->m[1][2] * boxScaleBuff[2] + rotateZ->m[1][3] * boxScaleBuff[3];
		boxScaleRotateZBuff[2] = rotateZ->m[2][0] * boxScaleBuff[0] + rotateZ->m[2][1] * boxScaleBuff[1] + rotateZ->m[2][2] * boxScaleBuff[2] + rotateZ->m[2][3] * boxScaleBuff[3];
		boxScaleRotateZBuff[3] = rotateZ->m[3][0] * boxScaleBuff[0] + rotateZ->m[3][1] * boxScaleBuff[1] + rotateZ->m[3][2] * boxScaleBuff[2] + rotateZ->m[3][3] * boxScaleBuff[3];

		//原点から元に戻すところ(大きさのみ)
		boxOriginMoveBuff2[0] = move->m[0][0] * boxScaleBuff[0] + move->m[0][1] * boxScaleBuff[1] + move->m[0][2] * boxScaleBuff[2] + boxbuff[0] * boxScaleBuff[3];
		boxOriginMoveBuff2[1] = move->m[1][0] * boxScaleBuff[0] + move->m[1][1] * boxScaleBuff[1] + move->m[1][2] * boxScaleBuff[2] + boxbuff[1] * boxScaleBuff[3];
		boxOriginMoveBuff2[2] = move->m[2][0] * boxScaleBuff[0] + move->m[2][1] * boxScaleBuff[1] + move->m[2][2] * boxScaleBuff[2] + boxbuff[2] * boxScaleBuff[3];
		boxOriginMoveBuff2[3] = move->m[3][0] * boxScaleBuff[0] + move->m[3][1] * boxScaleBuff[1] + move->m[3][2] * boxScaleBuff[2] + boxbuff[3] * boxScaleBuff[3];

		//原点から元に戻すところ(X軸方向に回転のみ)
		boxOriginMoveBuff3[0] = move->m[0][0] * boxRotateXBuff[0] + move->m[0][1] * boxRotateXBuff[1] + move->m[0][2] * boxRotateXBuff[2] + boxbuff[0] * boxRotateXBuff[3];
		boxOriginMoveBuff3[1] = move->m[1][0] * boxRotateXBuff[0] + move->m[1][1] * boxRotateXBuff[1] + move->m[1][2] * boxRotateXBuff[2] + boxbuff[1] * boxRotateXBuff[3];
		boxOriginMoveBuff3[2] = move->m[2][0] * boxRotateXBuff[0] + move->m[2][1] * boxRotateXBuff[1] + move->m[2][2] * boxRotateXBuff[2] + boxbuff[2] * boxRotateXBuff[3];
		boxOriginMoveBuff3[3] = move->m[3][0] * boxRotateXBuff[0] + move->m[3][1] * boxRotateXBuff[1] + move->m[3][2] * boxRotateXBuff[2] + boxbuff[3] * boxRotateXBuff[3];

		//原点から元に戻すところ(Y軸方向に回転のみ)
		boxOriginMoveBuff4[0] = move->m[0][0] * boxRotateYBuff[0] + move->m[0][1] * boxRotateYBuff[1] + move->m[0][2] * boxRotateYBuff[2] + boxbuff[0] * boxRotateYBuff[3];
		boxOriginMoveBuff4[1] = move->m[1][0] * boxRotateYBuff[0] + move->m[1][1] * boxRotateYBuff[1] + move->m[1][2] * boxRotateYBuff[2] + boxbuff[1] * boxRotateYBuff[3];
		boxOriginMoveBuff4[2] = move->m[2][0] * boxRotateYBuff[0] + move->m[2][1] * boxRotateYBuff[1] + move->m[2][2] * boxRotateYBuff[2] + boxbuff[2] * boxRotateYBuff[3];
		boxOriginMoveBuff4[3] = move->m[3][0] * boxRotateYBuff[0] + move->m[3][1] * boxRotateYBuff[1] + move->m[3][2] * boxRotateYBuff[2] + boxbuff[3] * boxRotateYBuff[3];

		//原点から元に戻すところ(Z軸方向に回転のみ)
		boxOriginMoveBuff5[0] = move->m[0][0] * boxRotateZBuff[0] + move->m[0][1] * boxRotateZBuff[1] + move->m[0][2] * boxRotateZBuff[2] + boxbuff[0] * boxRotateZBuff[3];
		boxOriginMoveBuff5[1] = move->m[1][0] * boxRotateZBuff[0] + move->m[1][1] * boxRotateZBuff[1] + move->m[1][2] * boxRotateZBuff[2] + boxbuff[1] * boxRotateZBuff[3];
		boxOriginMoveBuff5[2] = move->m[2][0] * boxRotateZBuff[0] + move->m[2][1] * boxRotateZBuff[1] + move->m[2][2] * boxRotateZBuff[2] + boxbuff[2] * boxRotateZBuff[3];
		boxOriginMoveBuff5[3] = move->m[3][0] * boxRotateZBuff[0] + move->m[3][1] * boxRotateZBuff[1] + move->m[3][2] * boxRotateZBuff[2] + boxbuff[3] * boxRotateZBuff[3];

		//原点から元に戻すところ(大きさを変えつつX軸方向に回転)
		boxOriginMoveBuff6[0] = move->m[0][0] * boxScaleRotateXBuff[0] + move->m[0][1] * boxScaleRotateXBuff[1] + move->m[0][2] * boxScaleRotateXBuff[2] + boxbuff[0] * boxScaleRotateXBuff[3];
		boxOriginMoveBuff6[1] = move->m[1][0] * boxScaleRotateXBuff[0] + move->m[1][1] * boxScaleRotateXBuff[1] + move->m[1][2] * boxScaleRotateXBuff[2] + boxbuff[1] * boxScaleRotateXBuff[3];
		boxOriginMoveBuff6[2] = move->m[2][0] * boxScaleRotateXBuff[0] + move->m[2][1] * boxScaleRotateXBuff[1] + move->m[2][2] * boxScaleRotateXBuff[2] + boxbuff[2] * boxScaleRotateXBuff[3];
		boxOriginMoveBuff6[3] = move->m[3][0] * boxScaleRotateXBuff[0] + move->m[3][1] * boxScaleRotateXBuff[1] + move->m[3][2] * boxScaleRotateXBuff[2] + boxbuff[3] * boxScaleRotateXBuff[3];

		//原点から元に戻すところ(大きさを変えつつY軸方向に回転)
		boxOriginMoveBuff7[0] = move->m[0][0] * boxScaleRotateYBuff[0] + move->m[0][1] * boxScaleRotateYBuff[1] + move->m[0][2] * boxScaleRotateYBuff[2] + boxbuff[0] * boxScaleRotateYBuff[3];
		boxOriginMoveBuff7[1] = move->m[1][0] * boxScaleRotateYBuff[0] + move->m[1][1] * boxScaleRotateYBuff[1] + move->m[1][2] * boxScaleRotateYBuff[2] + boxbuff[1] * boxScaleRotateYBuff[3];
		boxOriginMoveBuff7[2] = move->m[2][0] * boxScaleRotateYBuff[0] + move->m[2][1] * boxScaleRotateYBuff[1] + move->m[2][2] * boxScaleRotateYBuff[2] + boxbuff[2] * boxScaleRotateYBuff[3];
		boxOriginMoveBuff7[3] = move->m[3][0] * boxScaleRotateYBuff[0] + move->m[3][1] * boxScaleRotateYBuff[1] + move->m[3][2] * boxScaleRotateYBuff[2] + boxbuff[3] * boxScaleRotateYBuff[3];

		//原点から元に戻すところ(大きさを変えつつZ軸方向に回転)
		boxOriginMoveBuff8[0] = move->m[0][0] * boxScaleRotateZBuff[0] + move->m[0][1] * boxScaleRotateZBuff[1] + move->m[0][2] * boxScaleRotateZBuff[2] + boxbuff[0] * boxScaleRotateZBuff[3];
		boxOriginMoveBuff8[1] = move->m[1][0] * boxScaleRotateZBuff[0] + move->m[1][1] * boxScaleRotateZBuff[1] + move->m[1][2] * boxScaleRotateZBuff[2] + boxbuff[1] * boxScaleRotateZBuff[3];
		boxOriginMoveBuff8[2] = move->m[2][0] * boxScaleRotateZBuff[0] + move->m[2][1] * boxScaleRotateZBuff[1] + move->m[2][2] * boxScaleRotateZBuff[2] + boxbuff[2] * boxScaleRotateZBuff[3];
		boxOriginMoveBuff8[3] = move->m[3][0] * boxScaleRotateZBuff[0] + move->m[3][1] * boxScaleRotateZBuff[1] + move->m[3][2] * boxScaleRotateZBuff[2] + boxbuff[3] * boxScaleRotateZBuff[3];

		//移動のみをするところ
		boxMoveBuff[0] = move->m[0][0] * vertex[i].x + move->m[0][1] * vertex[i].y + move->m[0][2] * vertex[i].z + move->m[0][3] * 1.0f;
		boxMoveBuff[1] = move->m[1][0] * vertex[i].x + move->m[1][1] * vertex[i].y + move->m[1][2] * vertex[i].z + move->m[1][3] * 1.0f;
		boxMoveBuff[2] = move->m[2][0] * vertex[i].x + move->m[2][1] * vertex[i].y + move->m[2][2] * vertex[i].z + move->m[1][3] * 1.0f;
		boxMoveBuff[3] = move->m[3][0] * vertex[i].x + move->m[3][1] * vertex[i].y + move->m[3][2] * vertex[i].z + move->m[1][3] * 1.0f;

		//大きさを変えてから移動をするところ
		boxMoveBuff2[0] = move->m[0][0] * boxOriginMoveBuff2[0] + move->m[0][1] * boxOriginMoveBuff2[1] + move->m[0][2] * boxOriginMoveBuff2[2] + move->m[0][3] * boxOriginMoveBuff2[3];
		boxMoveBuff2[1] = move->m[1][0] * boxOriginMoveBuff2[0] + move->m[1][1] * boxOriginMoveBuff2[1] + move->m[1][2] * boxOriginMoveBuff2[2] + move->m[1][3] * boxOriginMoveBuff2[3];
		boxMoveBuff2[2] = move->m[2][0] * boxOriginMoveBuff2[0] + move->m[2][1] * boxOriginMoveBuff2[1] + move->m[2][2] * boxOriginMoveBuff2[2] + move->m[1][3] * boxOriginMoveBuff2[3];
		boxMoveBuff2[3] = move->m[3][0] * boxOriginMoveBuff2[0] + move->m[3][1] * boxOriginMoveBuff2[1] + move->m[3][2] * boxOriginMoveBuff2[2] + move->m[1][3] * boxOriginMoveBuff2[3];

		//X軸方向に回転と移動をするところ
		boxMoveBuff3[0] = move->m[0][0] * boxOriginMoveBuff3[0] + move->m[0][1] * boxOriginMoveBuff3[1] + move->m[0][2] * boxOriginMoveBuff3[2] + move->m[0][3] * boxOriginMoveBuff3[3];
		boxMoveBuff3[1] = move->m[1][0] * boxOriginMoveBuff3[0] + move->m[1][1] * boxOriginMoveBuff3[1] + move->m[1][2] * boxOriginMoveBuff3[2] + move->m[1][3] * boxOriginMoveBuff3[3];
		boxMoveBuff3[2] = move->m[2][0] * boxOriginMoveBuff3[0] + move->m[2][1] * boxOriginMoveBuff3[1] + move->m[2][2] * boxOriginMoveBuff3[2] + move->m[2][3] * boxOriginMoveBuff3[3];
		boxMoveBuff3[3] = move->m[3][0] * boxOriginMoveBuff3[0] + move->m[3][1] * boxOriginMoveBuff3[1] + move->m[3][2] * boxOriginMoveBuff3[2] + move->m[3][3] * boxOriginMoveBuff3[3];

		//Y軸方向に回転と移動をするところ
		boxMoveBuff4[0] = move->m[0][0] * boxOriginMoveBuff4[0] + move->m[0][1] * boxOriginMoveBuff4[1] + move->m[0][2] * boxOriginMoveBuff4[2] + move->m[0][3] * boxOriginMoveBuff4[3];
		boxMoveBuff4[1] = move->m[1][0] * boxOriginMoveBuff4[0] + move->m[1][1] * boxOriginMoveBuff4[1] + move->m[1][2] * boxOriginMoveBuff4[2] + move->m[1][3] * boxOriginMoveBuff4[3];
		boxMoveBuff4[2] = move->m[2][0] * boxOriginMoveBuff4[0] + move->m[2][1] * boxOriginMoveBuff4[1] + move->m[2][2] * boxOriginMoveBuff4[2] + move->m[2][3] * boxOriginMoveBuff4[3];
		boxMoveBuff4[3] = move->m[3][0] * boxOriginMoveBuff4[0] + move->m[3][1] * boxOriginMoveBuff4[1] + move->m[3][2] * boxOriginMoveBuff4[2] + move->m[3][3] * boxOriginMoveBuff4[3];

		//Z軸方向に回転と移動をするところ
		boxMoveBuff5[0] = move->m[0][0] * boxOriginMoveBuff5[0] + move->m[0][1] * boxOriginMoveBuff5[1] + move->m[0][2] * boxOriginMoveBuff5[2] + move->m[0][3] * boxOriginMoveBuff5[3];
		boxMoveBuff5[1] = move->m[0][0] * boxOriginMoveBuff5[0] + move->m[0][1] * boxOriginMoveBuff5[1] + move->m[0][2] * boxOriginMoveBuff5[2] + move->m[0][3] * boxOriginMoveBuff5[3];
		boxMoveBuff5[2] = move->m[0][0] * boxOriginMoveBuff5[0] + move->m[0][1] * boxOriginMoveBuff5[1] + move->m[0][2] * boxOriginMoveBuff5[2] + move->m[0][3] * boxOriginMoveBuff5[3];
		boxMoveBuff5[3] = move->m[0][0] * boxOriginMoveBuff5[0] + move->m[0][1] * boxOriginMoveBuff5[1] + move->m[0][2] * boxOriginMoveBuff5[2] + move->m[0][3] * boxOriginMoveBuff5[3];

		//大きさ変更とX軸方向に回転と移動をするところ
		boxMoveScaleRotateXBuff[0] = move->m[0][0] * boxOriginMoveBuff6[0] + move->m[0][1] * boxOriginMoveBuff6[1] + move->m[0][2] * boxOriginMoveBuff6[2] + move->m[0][3] * boxOriginMoveBuff6[3];
		boxMoveScaleRotateXBuff[1] = move->m[1][0] * boxOriginMoveBuff6[0] + move->m[1][1] * boxOriginMoveBuff6[1] + move->m[1][2] * boxOriginMoveBuff6[2] + move->m[1][3] * boxOriginMoveBuff6[3];
		boxMoveScaleRotateXBuff[2] = move->m[2][0] * boxOriginMoveBuff6[0] + move->m[2][1] * boxOriginMoveBuff6[1] + move->m[2][2] * boxOriginMoveBuff6[2] + move->m[2][3] * boxOriginMoveBuff6[3];
		boxMoveScaleRotateXBuff[3] = move->m[3][0] * boxOriginMoveBuff6[0] + move->m[3][1] * boxOriginMoveBuff6[1] + move->m[3][2] * boxOriginMoveBuff6[2] + move->m[3][3] * boxOriginMoveBuff6[3];

		//大きさ変更とX軸方向に回転と移動をするところ
		boxMoveScaleRotateYBuff[0] = move->m[0][0] * boxOriginMoveBuff7[0] + move->m[0][1] * boxOriginMoveBuff7[1] + move->m[0][2] * boxOriginMoveBuff7[2] + move->m[0][3] * boxOriginMoveBuff7[3];
		boxMoveScaleRotateYBuff[1] = move->m[1][0] * boxOriginMoveBuff7[0] + move->m[1][1] * boxOriginMoveBuff7[1] + move->m[1][2] * boxOriginMoveBuff7[2] + move->m[1][3] * boxOriginMoveBuff7[3];
		boxMoveScaleRotateYBuff[2] = move->m[2][0] * boxOriginMoveBuff7[0] + move->m[2][1] * boxOriginMoveBuff7[1] + move->m[2][2] * boxOriginMoveBuff7[2] + move->m[2][3] * boxOriginMoveBuff7[3];
		boxMoveScaleRotateYBuff[3] = move->m[3][0] * boxOriginMoveBuff7[0] + move->m[3][1] * boxOriginMoveBuff7[1] + move->m[3][2] * boxOriginMoveBuff7[2] + move->m[3][3] * boxOriginMoveBuff7[3];

		//大きさ変更とX軸方向に回転と移動をするところ
		boxMoveScaleRotateZBuff[0] = move->m[0][0] * boxOriginMoveBuff8[0] + move->m[0][1] * boxOriginMoveBuff8[1] + move->m[0][2] * boxOriginMoveBuff8[2] + move->m[0][3] * boxOriginMoveBuff8[3];
		boxMoveScaleRotateZBuff[1] = move->m[1][0] * boxOriginMoveBuff8[0] + move->m[1][1] * boxOriginMoveBuff8[1] + move->m[1][2] * boxOriginMoveBuff8[2] + move->m[1][3] * boxOriginMoveBuff8[3];
		boxMoveScaleRotateZBuff[2] = move->m[2][0] * boxOriginMoveBuff8[0] + move->m[2][1] * boxOriginMoveBuff8[1] + move->m[2][2] * boxOriginMoveBuff8[2] + move->m[2][3] * boxOriginMoveBuff8[3];
		boxMoveScaleRotateZBuff[3] = move->m[3][0] * boxOriginMoveBuff8[0] + move->m[3][1] * boxOriginMoveBuff8[1] + move->m[3][2] * boxOriginMoveBuff8[2] + move->m[3][3] * boxOriginMoveBuff8[3];

		//移動だけをした立方体
		vertex2[i].x = boxMoveBuff[0];
		vertex2[i].y = boxMoveBuff[1];
		vertex2[i].z = boxMoveBuff[2];


		//大きさだけを変更した立方体
		vertex3[i].x = boxOriginMoveBuff2[0];
		vertex3[i].y = boxOriginMoveBuff2[1];
		vertex3[i].z = boxOriginMoveBuff2[2];


		//X軸方向に回転だけを変更した立方体
		vertex4[i].x = boxOriginMoveBuff3[0];
		vertex4[i].y = boxOriginMoveBuff3[1];
		vertex4[i].z = boxOriginMoveBuff3[2];

		//Y軸方向に回転だけを変更した立方体
		vertex5[i].x = boxOriginMoveBuff4[0];
		vertex5[i].y = boxOriginMoveBuff4[1];
		vertex5[i].z = boxOriginMoveBuff4[2];

		//Z軸方向に回転だけを変更した立方体
		vertex6[i].x = boxOriginMoveBuff5[0];
		vertex6[i].y = boxOriginMoveBuff5[1];
		vertex6[i].z = boxOriginMoveBuff5[2];


		//大きさを変更しつつX軸方向に回転した立方体
		vertex7[i].x = boxOriginMoveBuff6[0];
		vertex7[i].y = boxOriginMoveBuff6[1];
		vertex7[i].z = boxOriginMoveBuff6[2];

		//大きさを変更しつつY軸方向に回転した立方体
		vertex8[i].x = boxOriginMoveBuff7[0];
		vertex8[i].y = boxOriginMoveBuff7[1];
		vertex8[i].z = boxOriginMoveBuff7[2];

		//大きさを変更しつつZ軸方向に回転した立方体
		vertex9[i].x = boxOriginMoveBuff8[0];
		vertex9[i].y = boxOriginMoveBuff8[1];
		vertex9[i].z = boxOriginMoveBuff8[2];


		//大きさを変更しつつ移動した立方体
		vertex10[i].x = boxMoveBuff2[0];
		vertex10[i].y = boxMoveBuff2[1];
		vertex10[i].z = boxMoveBuff2[2];


		//移動しつつX軸方向に回転した立方体
		vertex11[i].x = boxMoveBuff3[0];
		vertex11[i].y = boxMoveBuff3[1];
		vertex11[i].z = boxMoveBuff3[2];

		//移動しつつY軸方向に回転した立方体
		vertex12[i].x = boxMoveBuff4[0];
		vertex12[i].y = boxMoveBuff4[1];
		vertex12[i].z = boxMoveBuff4[2];

		//移動しつつZ軸方向に回転した立方体
		vertex13[i].x = boxMoveBuff5[0];
		vertex13[i].y = boxMoveBuff5[1];
		vertex13[i].z = boxMoveBuff5[2];


		//大きさを変更しつつ移動しつつX軸方向に回転した立方体
		vertex14[i].x = boxMoveScaleRotateXBuff[0];
		vertex14[i].y = boxMoveScaleRotateXBuff[1];
		vertex14[i].z = boxMoveScaleRotateXBuff[2];

		//大きさを変更しつつ移動しつつY軸方向に回転した立方体
		vertex15[i].x = boxMoveScaleRotateYBuff[0];
		vertex15[i].y = boxMoveScaleRotateYBuff[1];
		vertex15[i].z = boxMoveScaleRotateYBuff[2];

		//大きさを変更しつつ移動しつつZ軸方向に回転した立方体
		vertex16[i].x = boxMoveScaleRotateZBuff[0];
		vertex16[i].y = boxMoveScaleRotateZBuff[1];
		vertex16[i].z = boxMoveScaleRotateZBuff[2];

	}


}

void GameScene::Update() 
{
	debugCamera_->Update();
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

	
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	/// 
		//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	for (int i = 0; i < 12; i++)
	{
		//大元の描画
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex[eqgeList[i][0]], vertex[eqgeList[i][1]], color);

		//移動のみ
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex2[eqgeList[i][0]], vertex2[eqgeList[i][1]], color2);

		//大きさ変更のみ
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex3[eqgeList[i][0]], vertex3[eqgeList[i][1]], color3);

		//回転(上からX,Y,Z)
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex4[eqgeList[i][0]], vertex4[eqgeList[i][1]], color4);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex5[eqgeList[i][0]], vertex5[eqgeList[i][1]], color4);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex6[eqgeList[i][0]], vertex6[eqgeList[i][1]], color4);

		//大きさ変更と回転(上からX,Y,Z)
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex7[eqgeList[i][0]], vertex7[eqgeList[i][1]], color5);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex8[eqgeList[i][0]], vertex8[eqgeList[i][1]], color5);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex9[eqgeList[i][0]], vertex9[eqgeList[i][1]], color5);
		
		//大きさ変更と移動
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex10[eqgeList[i][0]], vertex10[eqgeList[i][1]], color6);
		
		//回転と移動(上からX,Y,Z)
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex11[eqgeList[i][0]], vertex11[eqgeList[i][1]], color7);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex12[eqgeList[i][0]], vertex12[eqgeList[i][1]], color7);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex13[eqgeList[i][0]], vertex13[eqgeList[i][1]], color7);
		
		//大きさ変更と移動と回転(上からX,Y,Z)
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex14[eqgeList[i][0]], vertex14[eqgeList[i][1]], color8);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex15[eqgeList[i][0]], vertex15[eqgeList[i][1]], color8);
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex16[eqgeList[i][0]], vertex16[eqgeList[i][1]], color8);
		
	}

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
