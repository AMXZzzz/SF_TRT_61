#pragma once
#ifndef In_AccountRegisterArgs_H_
#define In_AccountRegisterArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �˺�ע�����
	class In_AccountRegisterArgs :public In_DataBaseArgs
	{
	public:
		// �˺�(����)
		string username;
		// ����(����)
		string userpwd;
		// qq(��ѡ)
		string qq;
		// wx(��ѡ)
		string wx;
		// ֧����(��ѡ)
		string alipay;
		// �绰(��ѡ)
		string tel;
		// ����(��ѡ)
		string email;
	};
}
#endif