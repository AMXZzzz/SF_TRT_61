#pragma once
#ifndef Out_rechCardNumDetail_H_
#define Out_rechCardNumDetail_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ֵ���������
	class Out_rechCardNumDetail :public Out_DataBaseArgs
	{
	public:
		/*
		*�������
		*/
		string softname;

		/*
		*��ʹ��ֵ����
		*/
		string consumevaluename;

		/*������������ ʱ�䣬����*/
		string consumetypename;

		//ʹ��״̬����
		string rechcardstatename;
	};
}
#endif