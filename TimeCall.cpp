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

	if (IsEnd)
	{
		return;
	}
	time_--;
	if (time_ <= 0)
	{
		IsEnd = true;
		//�R�[���o�b�N�֐��Ăяo��
		func_();
	}

}