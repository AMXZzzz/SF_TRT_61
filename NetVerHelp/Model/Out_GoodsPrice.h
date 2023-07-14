#pragma once
#ifndef Out_GoodsPrice_H_
#define Out_GoodsPrice_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 商品价格列表出参
	class Out_GoodsPrice :public Out_DataBaseArgs
	{
	public:
		//商品价格
		double price;
	};
}
#endif