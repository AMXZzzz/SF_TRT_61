#pragma once
#ifndef In_Out_DataBaseArgs_H_
#define In_Out_DataBaseArgs_H_
#include <string>
using std::string;
using namespace std;
namespace Model
{
	class Out_DataBaseArgs
	{

	public:
		/*
		����ӿ�ʱ�ύ������һ����ʶ����ԭ��ԭ���İѴ�ֵ����ȥ���������ȥ��ֵ�뵱ǰ�ύ��ֵ��һ�£�
		˵���ӿڷ��ص������п��ܱ����ƽ��ߡ��س֣���ô�����������ֱ�ӹرջ����κδ���
		*/
		string requestflag;

		//������ʱ���
		__int64 servertimestamp;
	};
}
#endif