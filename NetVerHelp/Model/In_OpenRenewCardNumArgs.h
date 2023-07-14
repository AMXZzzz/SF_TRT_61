#pragma once
#ifndef In_OpenRenewCardNumArgs_H_
#define In_OpenRenewCardNumArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 开通/续费卡密入参
	class In_OpenRenewCardNumArgs :public In_DataBaseArgs
	{
	public:
		//卡密(必填)
		string cardnum;
		//价格类型ID(必填)
		string priceid;
	};
}
#endif