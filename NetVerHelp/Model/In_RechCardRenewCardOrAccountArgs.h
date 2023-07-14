#pragma once
#ifndef In_RechCardRenewCardOrAccountArgs_H_
#define In_RechCardRenewCardOrAccountArgs_H_
#include <string>
#include "In_DataBaseArgs.h"
using namespace std;
namespace Model
{
	// 充值卡充值卡密或账号
	class In_RechCardRenewCardOrAccountArgs :public In_DataBaseArgs
	{
	public:
		// 被充值的卡密或账号(必填)
		string cardnumorusername;
		// 充值卡(必填)
		string rechcardnum;
	};
}
#endif