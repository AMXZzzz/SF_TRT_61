#pragma once
#ifndef In_BucklePointArgs_H_
#define In_BucklePointArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 扣点入参
	class In_BucklePointArgs :public In_DataBaseArgs
	{
	public:
		//登录令牌(必填)
		string token;
		// 卡密或账号(必填)
		string cardnumorusername;
		// 扣除点数(如果为0，则扣除软件设置的值)
		int bucklevalue;
	};
}
#endif