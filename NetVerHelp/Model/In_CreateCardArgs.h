#pragma once
#ifndef In_CreateCardArgs_H_
#define In_CreateCardArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ���ɿ������
	class In_CreateCardArgs :public In_DataBaseArgs
	{
	public:
		// ����ǰ׺(1.ֻ����д���ֻ���ĸ 2.���Ȳ��ܴ���4)����ѡ
		string prefix;
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