#pragma once
#ifndef Out_iniGoodsInfo_H_
#define Out_iniGoodsInfo_H_
#include <string>
#include "Out_goodsInfo.h"
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 初始化商品软件出参数
	class Out_iniGoodsInfo :public Out_DataBaseArgs
	{
	public:
		//初始化商品软件Key值(通过软件初始化接口【iniSoftInfo】获取)
		string inisoftkey;

		Out_goodsInfo goodsinfo;
	};
}
#endif