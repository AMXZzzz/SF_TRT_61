#pragma once
#ifndef DisableCardOrAccountArgs_H_
#define DisableCardOrAccountArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
#include "DisableCardOrAccountType.h"
using namespace std;
namespace Model
{
	// ���û���ɾ����������˺�
	class In_DisableCardOrAccountArgs :public In_DataBaseArgs
	{
	public:
		//��¼����(����)
		string token;
		// ���ܻ��˺�(����)
		string cardnumorusername;
		//���û���ɾ����������˺�(����)
		DisableCardOrAccountType disablecardoraccounttype;
	};
}
#endif
