#pragma once
#ifndef In_DataBaseArgs_H_
#define In_DataBaseArgs_H_
#include <string>
using std::string;
using namespace std;
namespace Model
{
	class In_DataBaseArgs
	{

	public:

		//������(����)
		string maccode;

		/*
		�����ʶ�����Ȳ��ܴ���16���ַ�(����)
		�ӿڷ������ݵ�ʱ�򣬻�ԭ��ԭ���İѴ�ֵ��������
		ע�⣺�ӿڷ��ص����ֵ��������ӿ��ύ��ʱֵ�����һ����˵�������ƽ��ߡ��س��˷��ص����ݣ�
		��ô���������ǿ�ƹرջ����κδ���
		*/
		string requestflag;

		//ʱ���(����)
		__int64 timestamp;
	};
}
#endif