#pragma once
#ifndef Out_OpenRenewCardNum_H_
#define Out_OpenRenewCardNum_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ����֧��--��ͨ���ѿ��ܳ���
	class Out_OpenRenewCardNum :public Out_DataBaseArgs
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