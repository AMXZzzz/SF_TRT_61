#pragma once
#ifndef In_BuyCardNumArgs_H_
#define In_BuyCardNumArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ����֧�����������
	class In_BuyCardNumArgs :public In_DataBaseArgs
	{
	public:
		//�۸�ID(����)
		string priceid;
	};
}
#endif