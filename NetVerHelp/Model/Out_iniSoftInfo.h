#pragma once
#ifndef Out_iniSoftInfo_H_
#define Out_iniSoftInfo_H_
#include <string>
#include <list>
#include "Out_softInfo.h"
#include "Out_softPrice.h"
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//�����ʼ����data����
	class  Out_iniSoftInfo :public Out_DataBaseArgs
	{
	public:
		//��ʼ�����Keyֵ(ͨ�������ʼ���ӿڡ�iniSoftInfo����ȡ)
		string inisoftkey;
		//�����Ϣ
		Out_softInfo softInfo;
		//����۸��б�
		list<Out_softPrice> softpricelist;
	};
}
#endif