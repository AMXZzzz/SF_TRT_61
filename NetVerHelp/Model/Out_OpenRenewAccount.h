#pragma once
#ifndef Out_OpenRenewAccount_H_
#define Out_OpenRenewAccount_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ����֧��--��ͨ�����˺ų���
	class Out_OpenRenewAccount :public Out_DataBaseArgs
	{
	public:
		//������(ǿ�ҽ��飬���˶����ű��浽���أ�������������쳣������������û����Ŷ���������ѯ�Ե�)
		string orderid;
		//����֧��ҳ���ַ(�����ĳ����У�ֱ����WebBrowser����������������򿪴�֧��ҳ�棬���ɣ��û�֧���ɹ��󣬻�ѿ���ֱ����ʾ��ҳ����)
		string payaddress;
		//������ѯҳ���ַ(����ͨ�������Ų�ѯ���ѹ�����Ŀ���)
		string ordersearchaddress;
	};
}
#endif