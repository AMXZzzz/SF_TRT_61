#pragma once
#ifndef In_RechCardRenewCardOrAccountArgs_H_
#define In_RechCardRenewCardOrAccountArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ֵ����ֵ���ܻ��˺�
	class In_RechCardRenewCardOrAccountArgs :public In_DataBaseArgs
	{
	public:
		// ����ֵ�Ŀ��ܻ��˺�(����)
		string cardnumorusername;
		// ��ֵ��(����)
		string rechcardnum;
	};
}
#endif