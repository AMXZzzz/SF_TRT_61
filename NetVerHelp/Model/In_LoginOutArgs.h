#pragma once
#ifndef In_LoginOutArgs_H_
#define In_LoginOutArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �˳���¼
	class In_LoginOutArgs :public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		// ���ܻ��˺�(����)
		string cardnumorusername;
	};
}
#endif