#pragma once
#ifndef Out_CreateCard_H_
#define Out_CreateCard_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//生成卡密出参
	class Out_CreateCard :public Out_DataBaseArgs
	{

	public:
		// 生成的卡密结果
		string cardnum;
	};
}
#endif