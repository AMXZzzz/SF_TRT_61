#pragma once
#ifndef In_accountDetailArgs_H_
#define In_accountDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �˺��������
	class In_accountDetailArgs :public In_DataBaseArgs
	{
	public:
		// �˺�(����)
		string username;
		// ����(����)
		string userpwd;
	};
}
#endif