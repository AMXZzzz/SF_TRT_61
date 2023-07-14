#pragma once
#ifndef Out_goodsInfo_H_
#define Out_goodsInfo_H_
#include <string>
using namespace std;
namespace Model
{
	// 商品信息
	class Out_goodsInfo 
	{
	public:
		//商品名称
		string goodsname;

		//商品价格
		double price;

		//软件咨询官网
		string consultwebsite;

		//软件咨询qq
		string consultqq;

		//软件咨询微信
		string consultwx;

		//软件咨询电话
		string consulttel;

		//公告
		string notice;

		//软件基础数据
		string basedata;
	};
}
#endif