#pragma once
#ifndef Out_CreateCard_H_
#define Out_CreateCard_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//���ɿ��ܳ���
	class Out_CreateCard :public Out_DataBaseArgs
	{

	public:
		// ���ɵĿ��ܽ��
		string cardnum;
	};
}
#endif