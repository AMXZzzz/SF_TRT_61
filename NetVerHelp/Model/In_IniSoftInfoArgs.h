#pragma once
#ifndef In_IniSoftInfoArgs_H_
#define In_IniSoftInfoArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//�����ʼ�����
	class In_IniSoftInfoArgs :public In_DataBaseArgs
	{

	public:
		// ����汾��(����)
		// ע��:��д�������ǰ�İ汾��
		string versionname;
	};
}
#endif