#pragma once
#ifndef Out_BuyCardNum_H_
#define Out_BuyCardNum_H_
#include <string>
#include "Out_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// ����֧��-������Ʒ����
	class Out_BuyCardNum :public Out_DataBaseArgs
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