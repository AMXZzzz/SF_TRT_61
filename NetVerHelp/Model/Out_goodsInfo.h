#pragma once
#ifndef Out_goodsInfo_H_
#define Out_goodsInfo_H_
#include <string>
using namespace std;
namespace Model
{
	// ��Ʒ��Ϣ
	class Out_goodsInfo 
	{
	public:
		//��Ʒ����
		string goodsname;

		//��Ʒ�۸�
		double price;

		//�����ѯ����
		string consultwebsite;

		//�����ѯqq
		string consultqq;

		//�����ѯ΢��
		string consultwx;

		//�����ѯ�绰
		string consulttel;

		//����
		string notice;

		//�����������
		string basedata;
	};
}
#endif