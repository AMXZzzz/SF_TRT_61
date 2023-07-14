#pragma once
#ifndef In_updRemarkArgs_H_
#define In_updRemarkArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 修改卡密/账号备注的入参
	class In_updRemarkArgs :public In_DataBaseArgs
	{
	public:
		//登录令牌(必填)
		string token;
		// 卡密或账号(必填)
		string cardnumorusername;
		// 备注
		string remarks;
	};
}
#endif