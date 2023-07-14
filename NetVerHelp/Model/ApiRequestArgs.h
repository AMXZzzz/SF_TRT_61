#pragma once
#ifndef ApiRequestArgs_H_
#define ApiRequestArgs_H_
#include <string>
#include "BusinessType.h"
#include "EncryptType.h"
#include "PlatformType.h"
using namespace std;
namespace Model
{
	//����ӿڵ����
	class  ApiRequestArgs
	{
	public:
		//ҵ��ID(����)
		BusinessType businessid;
		//���ܷ�ʽ(����)
		EncryptType encrypttypeid;
		// ƽ̨�û�Code(����)
		// ˵������������������Բ鿴�õ��������̵Ļ�����ô�ڣ��������--���������б����Բ鿴�õ�
		string platformusercode;
		//��ƷCode(����)
		string goodscode;
		//��ʼ�����Keyֵ(ͨ�������ʼ���ӿڡ�iniSoftInfo����ȡ������"�����ʼ���ӿ�"����Ҫ��д�������ӿڱ���д)
		string inisoftkey;
		//ʱ���(����)
		__int64 timestamp;
		//�����������Json�ַ���(�������API�ĵ�)
		string data;
		//ǩ��
		//ע�⣺
		//1.���������õ��ǡ��޼���ͨѶ������ô��ֵ����Ϊ�գ�������õ��ǡ�����ͨѶ������ô��ֵ����д
		//2.ǩ���㷨��md5(businessID+encrypttypeid+platformusercode+goodscode+inisoftkey+timestamp+data+signSalt+platformtypeid)
		string sign;

		//ƽ̨����(����) 1:������֤ƽ̨  2:�Զ�����ƽ̨
		PlatformType platformtypeid;
	};

}
#endif