#pragma once
#ifndef In_AccountLoginArgs_H_
#define In_AccountLoginArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	//ÕËºÅµÇÂ¼Èë²Î
	class In_AccountLoginArgs :public In_DataBaseArgs
	{
	public:
		//ÕËºÅ(±ØÌî)
		string username;
		// ÃÜÂë(±ØÌî)
		string userpwd;
	};
}
#endif