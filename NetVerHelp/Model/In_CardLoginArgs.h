#pragma once
#ifndef In_CardLoginArgs_H_
#define In_CardLoginArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//����������ࣺ���ܵ�¼
	class In_CardLoginArgs :public In_DataBaseArgs
	{

	public:
		//����(����)
		string cardnum;
	};
}
#endif