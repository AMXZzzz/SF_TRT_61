#pragma once
#ifndef Out_bucklePoint_H_
#define Out_bucklePoint_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 扣点出参
	class Out_bucklePoint :public Out_DataBaseArgs
	{
	public:
		// 剩余点数
		int surpluspointvalue;
	};
}
#endif