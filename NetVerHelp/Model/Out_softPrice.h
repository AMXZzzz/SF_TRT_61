#pragma once
#ifndef Out_softPrice_H_
#define Out_softPrice_H_
#include <string>
using namespace std;
namespace Model
{
	//软件价格
	class  Out_softPrice
	{
	public:
		//价格类型ID
		string priceid;
		//价格类型名称
		string pricetypename;
		//可使用值
		int consumevalue;
		//售价
		double price;
		// 同一卡密或账号最大登录的数量
		int maxloginnum;
		// 角色ID
		string roleid;
		// 角色名
		string rolename;
	};
}
#endif