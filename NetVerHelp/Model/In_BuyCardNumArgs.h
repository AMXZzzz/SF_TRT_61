#pragma once
#ifndef In_BuyCardNumArgs_H_
#define In_BuyCardNumArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 在线支付购买卡密入参
	class In_BuyCardNumArgs :public In_DataBaseArgs
	{
	public:
		//价格ID(必填)
		string priceid;
	};
}
#endif