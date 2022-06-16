#pragma once

#include <functional>

/// <summary>
/// 時限発動
/// </summary>
class TimeCall
{
public:
	//function<この中身>は使いたい関数の返り値と引数で設定する <返り値(引数)>
	TimeCall(std::function<void(void)> func,uint32_t time);
	~TimeCall();

	//更新
	void Updata();

	//完了ならtrueを返す
	bool IsFinished() { return IsEnd; };

private:

	//コールバック
	std::function<void(void)> func_;

	//残り時間
	uint32_t time_;

	//完了フラグ
	bool IsEnd = false;

};

