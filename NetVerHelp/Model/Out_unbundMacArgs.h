#pragma once
#ifndef Out_unbundMacArgs_H_
#define Out_unbundMacArgs_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �����������
	class Out_unbundMacArgs :public Out_DataBaseArgs
	{
	public:
		//�˺ŵ���ʱ��
		string endtime;
		// ʣ�����
		int surpluspointvalue;
	};
}
#endif