#pragma once
#ifndef Out_softPrice_H_
#define Out_softPrice_H_
#include <string>
using namespace std;
namespace Model
{
	//����۸�
	class  Out_softPrice
	{
	public:
		//�۸�����ID
		string priceid;
		//�۸���������
		string pricetypename;
		//��ʹ��ֵ
		int consumevalue;
		//�ۼ�
		double price;
		// ͬһ���ܻ��˺�����¼������
		int maxloginnum;
		// ��ɫID
		string roleid;
		// ��ɫ��
		string rolename;
	};
}
#endif