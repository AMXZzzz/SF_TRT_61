#pragma once
#ifndef Out_heartBeat_H_
#define Out_heartBeat_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//��������
	class Out_heartBeat :public Out_DataBaseArgs
	{
	public:
		//����Key,ÿ��������ֵ�����
		string	heartbeatkey;
		//�˺Ż��ܵ���ʱ��
		string endtime;

		//�˺Ż���ʣ�����
		int surpluspointvalue;
	};
}
#endif