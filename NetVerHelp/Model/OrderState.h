#pragma once
#ifndef OrderState_H_
#define OrderState_H_
namespace Model
{
	//����֧��״̬
	enum OrderState
	{
		// ֧����
		WaitPay = 0,
		//֧���ɹ�
		PaySuccess = 1,
		//֧��δ���
		PayNoComplete = 2,
		//֧��ʧ��
		PayError = 3,
	};
}
#endif