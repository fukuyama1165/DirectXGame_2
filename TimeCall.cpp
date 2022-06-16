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
	//カウントダウンが終わってたら終わり
	if (IsEnd)
	{
		return;
	}

	//タイマーを減らす
	time_--;

	//カウントダウンが0以下になったら
	if (time_ <= 0)
	{
		IsEnd = true;
		//コールバック関数呼び出し
		func_();
	}

}