#pragma once
#ifndef Out_cardDetail_H_
#define Out_cardDetail_H_
#include <string>
#include "IsPay.h"
#include "OpenState.h"
#include "ActiveState.h"
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �˺��������
	class Out_cardDetail :public Out_DataBaseArgs
	{
	public:
		/*
		*���ܵ���ʱ��
		*ע�⣺ֻ�п��ܼ��������£��������Ż᷵��
		*/
		string endtime;

		/*
		*����ʣ�����
		*ע�⣺ֻ�п��ܼ��������£��������Ż᷵��
		*/
		int surpluspointvalue;

		/*���һ�ε�¼�Ļ�����*/
		string maccode;

		//��ͨ�ļ۸�����ID
		string priceid;

		//��ͨ�ļ۸���������
		string pricename;

		//�Ƿ��Ѹ���  0:δ����  1:�Ѹ���
		IsPay ispay;

		//�ն��û���qq
		string qq;

		//�ն��û���΢��
		string wx;

		//�ն��û���֧����
		string alipay;

		//�ն��û����ֻ���
		string tel;

		//�ն��û�������
		string email;

		//����ʱ��
		string regtime;

		//��ע
		string remarks;

		//��ͨ״̬  0:δ��ͨ  1:�ѿ�ͨ
		OpenState openstate;

		//����״̬ 0:δ����  1:�Ѽ���
		ActiveState activestate;
	};
}
#endif