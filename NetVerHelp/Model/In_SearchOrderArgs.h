#pragma once
#ifndef In_SearchOrderArgs_H_
#define In_SearchOrderArgs_H_
#include <string>
#include "EncryptType.h"
#include "PlatformType.h"
using namespace std;
namespace Model
{
	//����ӿڵ����
	class  In_SearchOrderArgs
	{
	public:
		// ƽ̨�û�Code(����)
		// ˵������������������Բ鿴�õ��������̵Ļ�����ô�ڣ��������--���������б����Բ鿴�õ�
		string platformusercode;

		//��ƷCode(����)
		string goodscode;

		/*
		�����ʶ�����Ȳ��ܴ���16���ַ�(����)
		ע�⣺�ӿڷ������ݵ�ʱ�򣬻�ԭ��ԭ���İѴ�ֵ�������������������ֵ�뵱ǰ�ύ��ֵ��һ�£�
		˵���ӿڷ��ص������п��ܱ����ƽ��ߡ��س֣���ô�����������ֱ�ӹرջ����κδ���
		*/
		string requestflag;

		//���ܷ�ʽ(����)
		EncryptType encrypttypeid;

		//��ʼ�����Keyֵ(ͨ�������ʼ���ӿڡ�iniSoftInfo����ȡ������"�����ʼ���ӿ�"����Ҫ��д�������ӿڱ���д)
		string inisoftkey;

		//������
		string orderid;

		//ƽ̨����(����) 1:������֤ƽ̨  2:�Զ�����ƽ̨
		PlatformType platformtypeid;
	};

}
#endif