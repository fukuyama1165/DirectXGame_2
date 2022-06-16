#pragma once

#include <functional>

/// <summary>
/// ��������
/// </summary>
class TimeCall
{
public:
	//function<���̒��g>�͎g�������֐��̕Ԃ�l�ƈ����Őݒ肷�� <�Ԃ�l(����)>
	TimeCall(std::function<void(void)> func,uint32_t time);
	~TimeCall();

	//�X�V
	void Updata();

	//�����Ȃ�true��Ԃ�
	bool IsFinished() { return IsEnd; };

private:

	//�R�[���o�b�N
	std::function<void(void)> func_;

	//�c�莞��
	uint32_t time_;

	//�����t���O
	bool IsEnd = false;

};

