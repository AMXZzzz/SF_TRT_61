#pragma once
#ifndef In_rechCardDetailArgs_H_
#define In_rechCardDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ֵ������
	class In_rechCardDetailArgs :public In_DataBaseArgs
	{
	public:
		// ��ֵ��(����)
		string rechcardnum;
	};
}
#endif