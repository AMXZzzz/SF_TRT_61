#pragma once
#ifndef Out_GoodsPrice_H_
#define Out_GoodsPrice_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��Ʒ�۸��б����
	class Out_GoodsPrice :public Out_DataBaseArgs
	{
	public:
		//��Ʒ�۸�
		double price;
	};
}
#endif