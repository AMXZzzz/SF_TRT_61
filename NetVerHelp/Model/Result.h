#pragma once
#ifndef Result_H_
#define Result_H_
#include <string>
using namespace std;
namespace Model
{
	//�ӿڷ�����������
	class  Result
	{
	public:
		//�ӿڷ���������Ϣ
		string msg;

		//�ӿڷ���ҵ�����͵ľ�������
		string data;

		//�ɹ���0  ʧ�ܣ�����0����ʧ��
		int code;

	};
}
#endif