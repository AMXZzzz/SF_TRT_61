#pragma once
#ifndef In_UpdOrderFlagArgs_H_
#define In_UpdOrderFlagArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// �޸Ķ�������������
	class In_UpdOrderFlagArgs :public In_DataBaseArgs
	{
	public:
		//����,��Ʒ������
		string orderid;
		// ����,������ʶ����(ʹ�ó����������ն˿ͻ��ѳɹ����������ѷ�����ϣ���ô���Խ���ֵ�޸ģ�˵���˶����ѷ���)
		//�ַ����Ȳ��ܴ���500
		string orderflag;
	};
}
#endif