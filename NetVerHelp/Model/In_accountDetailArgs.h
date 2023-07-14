#pragma once
#ifndef In_accountDetailArgs_H_
#define In_accountDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 账号详情入参
	class In_accountDetailArgs :public In_DataBaseArgs
	{
	public:
		// 账号(必填)
		string username;
		// 密码(必填)
		string userpwd;
	};
}
#endif