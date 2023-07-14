#pragma once
#ifndef Out_Login_H_
#define Out_Login_H_
#include <string>
#include <list>
#include "Out_LoginJurisdiction.h"
#include "IsPay.h"
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//��¼����
	class Out_Login :public Out_DataBaseArgs
	{

	public:
		//��¼�ɹ��������
		string token;

		//����Key,ÿ��������ֵ�����
		string heartbeatkey;

		//���ܻ��˺ŵ���ʱ��
		string endtime;

		//���ܻ��˺�ʣ�����
		int surpluspointvalue;

		//�˿��ܻ��˺��ѵ�¼������
		int currentloginnum;

		//��ͨ�ļ۸�����ID
		string priceid;

		//��ͨ�ļ۸���������
		string pricename;

		//�Ƿ��Ѹ���  0:δ����  1:�Ѹ���
		IsPay ispay;

		//��ɫID
		string roleid;

		//��ɫ����
		string rolename;

		//ͬһ���ܻ��˺�����¼����
		int maxloginnum;

		//�˺��û���qq
		string qq;

		// �˺��û���΢��
		string wx;

		// �˺��û���֧����
		string alipay;

		// �˺��û����ֻ���
		string tel;

		// �˺��û�������
		string email;

		//��ע
		string remarks;

		//��ɫ�µ�Ȩ���б�
		list<Out_LoginJurisdiction> jurisdictionlist;
	};
}
#endif