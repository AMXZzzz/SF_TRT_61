#pragma once
#ifndef In_cardDetailArgs_H_
#define In_cardDetailArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �˺��������
	class In_cardDetailArgs :public In_DataBaseArgs
	{
	public:
		// ����(����)
		string cardnum;
	};
}
#endif