#pragma once
#ifndef Out_softInfo_H_
#define Out_softInfo_H_
#include <string>
using namespace std;
namespace Model
{
	//�����Ϣ
	class  Out_softInfo
	{
	public:
		//�������
		string softname;
		//�����ѯ����
		string consultwebsite;

		//�����ѯqq
		string consultqq;

		//�����ѯ΢��
		string consultwx;

		//�����ѯ�绰
		string consulttel;
		// ���logo���ص�ַ
		string logourl;
		//��ͨ���Կ��ܻ��˺ţ�N����ֻ�ܿ�ͨ���Ժŵ�������
		int opentestcount;
		//��ͨ���Կ��ܻ��˺ţ�����
		int opentestday;
		//����
		string notice;
		//�����������
		string basedata;
		//���°汾��
		string newversionnum;
		//������ص����̵�ַ
		string networkdiskurl;
		//������ȡ��
		string diskpwd;
		//�Ƿ�ǿ�Ƹ���  0����ǿ�� 1:ǿ��
		int isforceupd;
	};
}
#endif