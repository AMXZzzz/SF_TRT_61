#pragma once
#ifndef In_OpenRenewAccountArgs_H_
#define In_OpenRenewAccountArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ͨ/�����˺����
	class In_OpenRenewAccountArgs :public In_DataBaseArgs
	{
	public:
		//�˺�(����)
		string username;
		// ����(����)
		string userpwd;
		//�۸�����ID(����)
		string priceid;
	};
}
#endif