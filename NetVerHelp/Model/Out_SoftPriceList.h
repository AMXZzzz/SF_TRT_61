#pragma once
#ifndef Out_SoftPriceList_H_
#define Out_SoftPriceList_H_
#include <string>
#include <list>
#include "Out_DataBaseArgs.h"
#include "Out_softPrice.h"
using namespace std;
namespace Model
{
	// 软件价格列表出参
	class Out_SoftPriceList :public Out_DataBaseArgs
	{
	public:
		//软件价格列表
		list<Out_softPrice> softpricelist;
	};
}
#endif