#pragma once
#ifndef In_UpdPwdArgs_H_
#define In_UpdPwdArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 修改用户密码
	class In_UpdPwdArgs :public In_DataBaseArgs
	{
	public:
		// 账号(必填)
		string username;

		// 密码(必填)
		string userpwd;

		// 新密码
		string newpwd;
	};
}
#endif