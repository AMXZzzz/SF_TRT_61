#pragma once
#ifndef In_unbundMacArgs_H_
#define In_unbundMacArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �����������
	class In_unbundMacArgs :public In_DataBaseArgs
	{
	public:
		// ���ܻ��˺�(����)
		string cardnumorusername;
	};
}
#endif