#pragma once
#ifndef Result_H_
#define Result_H_
#include <string>
using namespace std;
namespace Model
{
	//接口返回来的数据
	class  Result
	{
	public:
		//接口返回来的消息
		string msg;

		//接口返回业务类型的具体数据
		string data;

		//成功：0  失败：不是0都是失败
		int code;

	};
}
#endif