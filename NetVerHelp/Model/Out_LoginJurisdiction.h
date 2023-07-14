#pragma once
#ifndef Out_LoginJurisdiction_H_
#define Out_LoginJurisdiction_H_
#include <string>
using namespace std;
namespace Model
{
	//此账号或卡密的权限(在角色管理-->角色列表里面添加的权限)
	class Out_LoginJurisdiction
	{
		// 权限ID
	public:
		string jurisdictionid;

		// 权限名称
		string jurisdictionname;

		// 权限值
		string jurisdictvalue;
	};
}
#endif