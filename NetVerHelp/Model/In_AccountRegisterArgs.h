#pragma once
#ifndef In_AccountRegisterArgs_H_
#define In_AccountRegisterArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 账号注册入参
	class In_AccountRegisterArgs :public In_DataBaseArgs
	{
	public:
		// 账号(必填)
		string username;
		// 密码(必填)
		string userpwd;
		// qq(可选)
		string qq;
		// wx(可选)
		string wx;
		// 支付宝(可选)
		string alipay;
		// 电话(可选)
		string tel;
		// 邮箱(可选)
		string email;
	};
}
#endif