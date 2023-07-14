#pragma once
#ifndef In_cardDetailArgs_H_
#define In_cardDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 账号详情入参
	class In_cardDetailArgs :public In_DataBaseArgs
	{
	public:
		// 卡密(必填)
		string cardnum;
	};
}
#endif