#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	CameraMat_ = MathUtility::Matrix4Identity();

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 0,0,50 };

	//3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle = TextureManager::Load("Reticle.png");

	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, { 640,360 }, { 1,1,1,1 }, { 0.5f,0.5f }));

	viewProjection_.Initialize();

}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update(ViewProjection viewProjection)
{

	//�f�X�t���O�̗������e���폜(remove_if->������v��S�č폜)
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)//if�̒��ŊȈՓI�Ȋ֐��𐶐����Ă�->[](����)
	{
		return bullet->IsDead();
	});

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float MoveSpeed = 1;

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * MoveSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * MoveSpeed;
		
	}

	/*if (input_->PushKey(DIK_W))
	{
		move = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_S))
	{
		move = { 0,-MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_D))
	{
		move = { MoveSpeed,0,0 };
	}

	if (input_->PushKey(DIK_A))
	{
		move = { -MoveSpeed,0,0 };
	}*/

	//�ړ����E���W
	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//���W�ړ�(�s��X�V�����̊֐���)
	worldTransform_.matWorldGeneration();

	//��]����arrow�L�[�̍��E�ŉ�]������
	Rotate();

	worldTransform_.matWorld_ *= CameraMat_;

	worldTransform_.TransferMatrix();

	Reticle2DMouseAttack(viewProjection);

	//Space�L�[�Œe�𐶐�����player�̐��ʂɐi�܂���
	Attack();

	//��������Ă���e���X�V����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	//���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z
	{

		////���@����3D���e�B�N���ւ̋���
		//const float kDistancePlayerTo3DReticle = 50.0f;

		////���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
		//Vector3 offset = { 0,0,1.0f };

		////���@�̃��[���h�s��̉�]�𔽉f
		//offset = VectorMat(offset,worldTransform_.matWorld_);

		//offset = offset.normalize() * kDistancePlayerTo3DReticle;

		////3D���e�B�N���̍��W��ݒ�
		//worldTransform3DReticle_.translation_ = { GetWorldPosition().x + offset.x,GetWorldPosition().y + offset.y,GetWorldPosition().z + offset.z };

		//worldTransform3DReticle_.matWorldGeneration();

	}

	//3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	{
		
		//Vector3 positionReticle = worldTransform3DReticle_.matWorldGetPos();

		//Matrix4 matViewport = {
		//	1280 / 2,0,0,0,
		//	0,-720 / 2,0,0,
		//	0,0,1,0,
		//	1280 / 2 + 0,720 / 2 + 0,0,1
		//};

		////�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
		//Matrix4 matViewProjectionViewport = viewProjection_.matView * viewProjection_.matProjection* matViewport;

		////���[���h���X�N���[�����W�ϊ�(������3D����2D�ɂȂ�)
		//positionReticle = matViewport.VectorMatDivW(matViewProjectionViewport, positionReticle);

		////positionReticle = matViewport.PosMat(positionReticle, matViewport);
		////positionReticle = matViewport.PosMat(worldTransform3DReticle_.matWorldGetPos(), matViewport);

		//
		////�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
		//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
		//debugText_->SetPos(50, 130);
		//debugText_->Printf("repos:(%f,%f,%f)", positionReticle.x, positionReticle.y, positionReticle.z);

	}
	

	debugText_->SetPos(50, 70);
	debugText_->Printf("pos:(%f,%f,%f)",worldTransform_.matWorldGetPos().x, worldTransform_.matWorldGetPos().y,worldTransform_.matWorldGetPos().z);
	
}

void Player::Draw(ViewProjection& viewProjection)
{

	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection);

	for(std::unique_ptr<PlayerBullet>& bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

void Player::Rotate()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 rotate = { 0,0,0 };

	const float MoveSpeed = 0.1f;

	if (input_->PushKey(DIK_RIGHT))
	{
		rotate = { 0,MoveSpeed,0 };
	}

	if (input_->PushKey(DIK_LEFT))
	{
		rotate = { 0,-MoveSpeed,0 };
	}

	worldTransform_.rotation_ += rotate;

	//���W�ړ�(�s��X�V�����̊֐���)
	worldTransform_.matWorldGeneration();
}

void Player::Attack()
{
	//if (input_->TriggerKey(DIK_SPACE))
	//{
	//	//���˒n�_�ׂ̈Ɏ��L�����̍��W���R�s�[
	//	Vector3 position = worldTransform_.matWorldGetPos();

	//	//�ړ��ʂ�ǉ�
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, 0);
	//	velocity = worldTransform3DReticle_.matWorldGetPos() - worldTransform_.matWorldGetPos();
	//	velocity = velocity.normalize() * kBulletSpeed;

	//	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]����
	//	velocity = VectorMat(velocity, worldTransform_.matWorld_);

	//	//�e�̐����Ə�����
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initlize(model_, position,velocity);

	//	//�e��o�^
	//	bullets_.push_back(std::move(newBullet));
	//}

	XINPUT_STATE joyState;

	//�Q�[���p�b�h���ڑ��Ȃ牽������������
	if (!Input::GetInstance()->GetJoystickState(0, joyState))
	{
		return;
	}

	//R�g���K�[�������Ă�����
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		//���˒n�_�ׂ̈Ɏ��L�����̍��W���R�s�[
		Vector3 position = worldTransform_.matWorldGetPos();

		//�ړ��ʂ�ǉ�
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, 0);
		velocity = worldTransform3DReticle_.matWorldGetPos() - worldTransform_.matWorldGetPos();
		velocity = velocity.normalize() * kBulletSpeed;

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]����
		velocity = VectorMat(velocity, worldTransform_.matWorld_);

		//�e�̐����Ə�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initlize(model_, position,velocity);

		//�e��o�^
		bullets_.push_back(std::move(newBullet));
	}

}

Vector3 Player::VectorMat(Vector3 vector, Matrix4 mat)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = vector.x * mat.m[0][0] + vector.y * mat.m[1][0] + vector.z * mat.m[2][0] + 0.0f * mat.m[3][0];
	changeVector.y = vector.x * mat.m[0][1] + vector.y * mat.m[1][1] + vector.z * mat.m[2][1] + 0.0f * mat.m[3][1];
	changeVector.z = vector.x * mat.m[0][2] + vector.y * mat.m[1][2] + vector.z * mat.m[2][2] + 0.0f * mat.m[3][2];

	return changeVector;
}

Vector3 Player::VectorMat(Matrix4 mat, Vector3 vector)
{
	Vector3 changeVector = { 0,0,0 };

	changeVector.x = mat.m[0][0] * vector.x + mat.m[0][1] * vector.y + mat.m[0][2] * vector.z + mat.m[0][3] * 0.0f;
	changeVector.y = mat.m[1][0] * vector.x + mat.m[1][1] * vector.y + mat.m[1][2] * vector.z + mat.m[1][3] * 0.0f;
	changeVector.z = mat.m[2][0] * vector.x + mat.m[2][1] * vector.y + mat.m[2][2] * vector.z + mat.m[2][3] * 0.0f;

	return changeVector;
}

Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldpos;

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldpos.x = worldTransform_.matWorld_.m[3][0];
	worldpos.y = worldTransform_.matWorld_.m[3][1];
	worldpos.z = worldTransform_.matWorld_.m[3][2];

	return worldpos;

}

void Player::OnCollision()
{

}

void Player::SetCameraMat(Matrix4 CameraMat)
{
	CameraMat_ = CameraMat;
}

void Player::Reticle2DMouseAttack(ViewProjection viewProjection)
{

	POINT mousePosition={};

	////�X�N���[�����W���擾
	//GetCursorPos(&mousePosition);

	////�N���C�A���g�G���A���W�ɕϊ�����
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	//sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 7.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 7.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	}



	Matrix4 matViewport = {
			640,0,0,0,
			0,-360,0,0,
			0,0,1,0,
			640 + 0,360 + 0,0,1
	};

	//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	Matrix4 matVPV;
	matVPV = viewProjection.matView;
	matVPV *= viewProjection.matProjection;
	matVPV *= matViewport;
	//�����s��̋t�s����v�Z����
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	
	//�X�N���[�����W
	Vector3 posNear = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	//�X�N���[�����W�n���烏�[���h���W�n��
	posNear = VectorMatDivW(matInverseVPV, posNear);
	posFar = VectorMatDivW(matInverseVPV, posFar);

	//�}�E�X���C�̕���
 	Vector3 mouseDirection = nainavec3(posFar,posNear);
	mouseDirection = mouseDirection.normalize();

	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 100;

	Vector3 A = posNear;
	A += Vector3(mouseDirection.x * kDistanceTestObject, mouseDirection.y * kDistanceTestObject, mouseDirection.z * kDistanceTestObject);
	worldTransform3DReticle_.translation_ = A;

	/*worldTransform3DReticle_.translation_ = posNear+mouseDirection * kDistanceTestObject;*/
	
	worldTransform3DReticle_.matWorldGeneration();

	debugText_->SetPos(20, 200);
	debugText_->Printf("mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	debugText_->SetPos(20, 220);
	debugText_->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
	debugText_->SetPos(20, 240);
	debugText_->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
	debugText_->SetPos(20, 260);
	debugText_->Printf("mouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);


}