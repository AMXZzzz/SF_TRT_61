#pragma once
#ifndef In_updRemarkArgs_H_
#define In_updRemarkArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �޸Ŀ���/�˺ű�ע�����
	class In_updRemarkArgs :public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		// ���ܻ��˺�(����)
		string cardnumorusername;
		// ��ע
		string remarks;
	};
}
#endif