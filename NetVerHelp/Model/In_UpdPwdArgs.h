#pragma once
#ifndef In_UpdPwdArgs_H_
#define In_UpdPwdArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �޸��û�����
	class In_UpdPwdArgs :public In_DataBaseArgs
	{
	public:
		// �˺�(����)
		string username;

		// ����(����)
		string userpwd;

		// ������
		string newpwd;
	};
}
#endif