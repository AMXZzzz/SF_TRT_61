#pragma once
#ifndef In_OpenRenewCardNumArgs_H_
#define In_OpenRenewCardNumArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ͨ/���ѿ������
	class In_OpenRenewCardNumArgs :public In_DataBaseArgs
	{
	public:
		//����(����)
		string cardnum;
		//�۸�����ID(����)
		string priceid;
	};
}
#endif