#pragma once
#ifndef Out_LoginJurisdiction_H_
#define Out_LoginJurisdiction_H_
#include <string>
using namespace std;
namespace Model
{
	//���˺Ż��ܵ�Ȩ��(�ڽ�ɫ����-->��ɫ�б�������ӵ�Ȩ��)
	class Out_LoginJurisdiction
	{
		// Ȩ��ID
	public:
		string jurisdictionid;

		// Ȩ������
		string jurisdictionname;

		// Ȩ��ֵ
		string jurisdictvalue;
	};
}
#endif