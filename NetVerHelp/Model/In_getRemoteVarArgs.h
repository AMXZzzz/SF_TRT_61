#pragma once
#ifndef In_getRemoteVarArgs_H_
#define In_getRemoteVarArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ȡԶ�̱������
	class In_getRemoteVarArgs :public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		// ���ܻ��˺�(����)
		string cardnumorusername;
		//��������(����) �����������Ϊ�յĻ�����ô�ӿڷ��������Ǵ�������б�����������Ǵ˱���������Ӧ�ı���ֵ
		string varname;
	};
}
#endif