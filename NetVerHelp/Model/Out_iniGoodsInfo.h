#pragma once
#ifndef Out_iniGoodsInfo_H_
#define Out_iniGoodsInfo_H_
#include <string>
#include "Out_goodsInfo.h"
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ��ʼ����Ʒ���������
	class Out_iniGoodsInfo :public Out_DataBaseArgs
	{
	public:
		//��ʼ����Ʒ���Keyֵ(ͨ�������ʼ���ӿڡ�iniSoftInfo����ȡ)
		string inisoftkey;

		Out_goodsInfo goodsinfo;
	};
}
#endif