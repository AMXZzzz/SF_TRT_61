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
	// ����۸��б����
	class Out_SoftPriceList :public Out_DataBaseArgs
	{
	public:
		//����۸��б�
		list<Out_softPrice> softpricelist;
	};
}
#endif