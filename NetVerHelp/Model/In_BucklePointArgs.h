#pragma once
#ifndef In_BucklePointArgs_H_
#define In_BucklePointArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �۵����
	class In_BucklePointArgs :public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		// ���ܻ��˺�(����)
		string cardnumorusername;
		// �۳�����(���Ϊ0����۳�������õ�ֵ)
		int bucklevalue;
	};
}
#endif