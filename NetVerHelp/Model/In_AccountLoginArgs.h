#pragma once
#ifndef In_AccountLoginArgs_H_
#define In_AccountLoginArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//�˺ŵ�¼���
	class In_AccountLoginArgs :public In_DataBaseArgs
	{
	public:
		//�˺�(����)
		string username;
		// ����(����)
		string userpwd;
	};
}
#endif