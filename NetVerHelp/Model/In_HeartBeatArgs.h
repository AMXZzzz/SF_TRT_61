#pragma once
#ifndef In_HeartBeatArgs_H_
#define In_HeartBeatArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �������
	class In_HeartBeatArgs : public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		//����Key,ÿ��������ֵ�����
		string heartbeatkey;
		// ���ܻ��˺�(����)
		string cardnumorusername;
	};
}
#endif