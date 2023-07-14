#pragma once
#ifndef In_OpenRenewAccountArgs_H_
#define In_OpenRenewAccountArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 开通/续费账号入参
	class In_OpenRenewAccountArgs :public In_DataBaseArgs
	{
	public:
		//账号(必填)
		string username;
		// 密码(必填)
		string userpwd;
		//价格类型ID(必填)
		string priceid;
	};
}
#endif