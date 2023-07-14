#pragma once
#ifndef In_UpdOrderFlagArgs_H_
#define In_UpdOrderFlagArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 修改订单标记内容入参
	class In_UpdOrderFlagArgs :public In_DataBaseArgs
	{
	public:
		//必填,商品订单号
		string orderid;
		// 必填,订单标识内容(使用场景：比如终端客户已成功付完款，并且已发货完毕，那么可以将此值修改，说明此订单已发货)
		//字符长度不能大于500
		string orderflag;
	};
}
#endif