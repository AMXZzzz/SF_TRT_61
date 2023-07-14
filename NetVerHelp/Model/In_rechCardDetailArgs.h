#pragma once
#ifndef In_rechCardDetailArgs_H_
#define In_rechCardDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 充值卡详情
	class In_rechCardDetailArgs :public In_DataBaseArgs
	{
	public:
		// 充值卡(必填)
		string rechcardnum;
	};
}
#endif