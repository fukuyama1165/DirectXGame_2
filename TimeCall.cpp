#include "TimeCall.h"

TimeCall::TimeCall(std::function<void(void)> func,uint32_t time)
	:func_(func),time_(time)
{
}

TimeCall::~TimeCall()
{
}

void TimeCall::Updata()
{
	//�J�E���g�_�E�����I����Ă���I���
	if (IsEnd)
	{
		return;
	}

	//�^�C�}�[�����炷
	time_--;

	//�J�E���g�_�E����0�ȉ��ɂȂ�����
	if (time_ <= 0)
	{
		IsEnd = true;
		//�R�[���o�b�N�֐��Ăяo��
		func_();
	}

}