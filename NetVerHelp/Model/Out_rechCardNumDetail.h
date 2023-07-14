#pragma once
#ifndef Out_rechCardNumDetail_H_
#define Out_rechCardNumDetail_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 充值卡详情出参
	class Out_rechCardNumDetail :public Out_DataBaseArgs
	{
	public:
		/*
		*所属软件
		*/
		string softname;

		/*
		*可使用值名称
		*/
		string consumevaluename;

		/*消耗类型名称 时间，点数*/
		string consumetypename;

		//使用状态名称
		string rechcardstatename;
	};
}
#endif