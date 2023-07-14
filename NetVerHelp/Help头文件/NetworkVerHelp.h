#pragma once


#ifndef NetworkVerHelp_H
#define NetworkVerHelp_H

#define _CRT_SECURE_NO_WARNINGS
#include "HttpRequestTool.h"
#include <windows.h>
#include <ctime>
#include <nb30.h>
#include <string>


#include "../Model/EncryptType.h"
#include "../Model/BusinessType.h"
#include "../Model/PlatformType.h"
#include "../Model/DisableCardOrAccountType.h"

#include "../Model/Out_DataBaseArgs.h"
#include "../Model/Out_iniSoftInfo.h"
#include "../Model/Out_iniGoodsInfo.h"
#include "../Model/Out_Login.h"
#include "../Model/Out_heartBeat.h"
#include "../Model/Out_bucklePoint.h"
#include "../Model/Out_getRemoteVar.h"
#include "../Model/Out_unbundMacArgs.h"
#include "../Model/Out_OpenRenewAccount.h"
#include "../Model/Out_BuyGoods.h"
#include "../Model/Out_SearchOrder.h"
#include "../Model/Out_CreateCard.h"
#include "../Model/Out_OpenRenewCardNum.h"
#include "../Model/Out_accountDetail.h"
#include "../Model/Out_cardDetail.h"
#include "../Model/Out_SoftPriceList.h"
#include "../Model/Out_GoodsPrice.h"
#include "../Model/Out_BuyCardNum.h"
#include "../Model/Out_rechCardNumDetail.h"

#include "../Model/In_IniSoftInfoArgs.h"
#include "../Model/In_HeartBeatArgs.h"
#include "../Model/In_CardLoginArgs.h"
#include "../Model/In_AccountRegisterArgs.h"
#include "../Model/In_AccountLoginArgs.h"
#include "../Model/In_UpdPwdArgs.h"
#include "../Model/In_BucklePointArgs.h"
#include "../Model/In_LoginOutArgs.h"
#include "../Model/In_getRemoteVarArgs.h"
#include "../Model/In_unbundMacArgs.h"
#include "../Model/In_OpenRenewAccountArgs.h"
#include "../Model/In_BuyGoodsArgs.h"
#include "../Model/In_SearchOrderArgs.h"
#include "../Model/In_UpdOrderFlagArgs.h"
#include "../Model/In_CreateCardArgs.h"
#include "../Model/In_OpenRenewCardNumArgs.h"
#include "../Model/In_accountDetailArgs.h"
#include "../Model/In_cardDetailArgs.h"
#include "../Model/In_getSoftPriceListArgs.h"
#include "../Model/In_getGoodsPriceArgs.h"
#include "../Model/In_BuyCardNumArgs.h"
#include "../Model/In_updRemarkArgs.h"
#include "../Model/In_RechCardRenewCardOrAccountArgs.h"
#include "../Model/In_rechCardDetailArgs.h"
#include "../Model/In_DisableCardOrAccountArgs.h"

#include "../Model/ApiRequestArgs.h"
#include "../Model/Result.h"

#include "CJsonObject.hpp"
#include "Rc4.h"
#include "Md5.h"


//#include "../../VMProtectSDK.h"
//#ifdef _WIN64
//#pragma comment(lib,"VMProtectSDK64.lib")
//#else
//#pragma comment(lib,"MProtectSDK32.lib")
//#endif



#pragma comment (lib,"netapi32.lib")
using namespace std;
namespace Help
{

	//软件初始化的结果对象
	extern  Model::Out_iniSoftInfo iniSoftInfoData;

	class NetworkVerHelp
	{
	public:

		//获取时间戳
		static	__int64 GetTimeStamp();

		//获取机器码
		static string GetMac();

		static void ini();

		//json字符串转基础返回类型
		static Model::Out_DataBaseArgs ConvertOutDataBaseArgs(string data);

		//初始化软件返的jsonData数据转化成对象
		static Model::Out_iniSoftInfo ConvertIniSoftInfoData(string data);

		//软件价格列表字符串转对象 
		static Model::Out_SoftPriceList ConvertSoftPriceListData(string data);

		//登录的jsonData数据转化成对象
		static Model::Out_Login ConvertLoginData(string data);



		//将心跳数据转化成对象
		static Model::Out_heartBeat ConvertHeartBeat(string data);

		//扣点数据转化成对象
		static Model::Out_bucklePoint ConvertBucklePoint(string data);

		//软件变量转化成对象
		static Model::Out_getRemoteVar ConvertRemoteVar(string data);

		//换绑daga结果转化成对象
		static Model::Out_unbundMacArgs ConvertUnbundMac(string data);

		//在线支付购买卡密结果转对象
		static Model::Out_BuyCardNum ConvertBuyCardNum(string data);

		//在线支付开通/续费卡密结果转对象
		static Model::Out_OpenRenewCardNum ConvertOpenRenewCardNum(string data);

		//在线支付开通续费账号结果转对象
		static Model::Out_OpenRenewAccount ConvertOpenRenewAccount(string data);

		//卡密详情结果转对象
		static Model::Out_cardDetail ConvertCardDetail(string data);

		//账号详情结果转对象
		static Model::Out_accountDetail ConvertAccountDetail(string data);

		//订单查询结果转对象
		static Model::Out_SearchOrder ConvertSearchOrder(string data);

		//充值卡详情结果转对象
		static Model::Out_rechCardNumDetail ConvertRechCardDetail(string data);






		//初始化软件
		static Model::Result GetIniSoftInfo(Model::In_IniSoftInfoArgs dataArgs, Model::PlatformType _platformtypeid, string _encryptKey,
			string _signSalt, Model::EncryptType _encrypttypeid, string _softcode, string _platformUserCode);


		//获取软件价格列表
		static Model::Result getSoftPriceList(Model::In_getSoftPriceListArgs dataArgs);


		//登录成功后，开始心跳(心跳间隔时间不能小于5分钟)
		static Model::Result GetHeartBeat(Model::In_HeartBeatArgs dataArgs);



		//卡密登录
		static Model::Result GetLoginByCard(Model::In_CardLoginArgs dataArgs);

		//在线支付购买卡密
		static Model::Result GetBuyCardNum(Model::In_BuyCardNumArgs dataArgs);

		//在线支付开通/续费卡密
		static Model::Result GeOpenRenewCardNum(Model::In_OpenRenewCardNumArgs dataArgs);

		//卡密详情
		static Model::Result GeCardDetail(Model::In_cardDetailArgs dataArgs);



		//注册账号
		static Model::Result GeAccountRegister(Model::In_AccountRegisterArgs dataArgs);

		//账号密码登录
		static Model::Result GeAccountLogin(Model::In_AccountLoginArgs dataArgs);

		//修改用户密码
		static Model::Result GeUpdPwd(Model::In_UpdPwdArgs dataArgs);

		//在线支付开通/续费账号
		static Model::Result GeOpenRenewAccount(Model::In_OpenRenewAccountArgs dataArgs);

		//账号详情
		static Model::Result GeAccountDetail(Model::In_accountDetailArgs dataArgs);

		//扣点
		static Model::Result GeBucklePoint(Model::In_BucklePointArgs dataArgs);

		//退出登录
		static Model::Result GeLoginOut(Model::In_LoginOutArgs dataArgs);

		//获取软件变量
		static Model::Result getRemoteVar(Model::In_getRemoteVarArgs dataArgs);

		//解绑机器码(如果软件设置解绑扣除相应的时间或点数，那么解绑成功后会自动扣除)
		static Model::Result unbundMac(Model::In_unbundMacArgs dataArgs);

		//订单查询
		static Model::Result GetSearchOrder(string orderid, string requestflag);

		//修改卡密/账号备注
		static Model::Result GetUpdRemark(Model::In_updRemarkArgs dataArgs);

		//充值卡充值卡密或账号
		static Model::Result GetRechCardRenew(Model::In_RechCardRenewCardOrAccountArgs dataArgs);

		//充值卡详情
		static Model::Result GetRechCardDetail(Model::In_rechCardDetailArgs dataArgs);

		//禁用还是删除：单码或账号
		static Model::Result DisableCardOrAccount(Model::In_DisableCardOrAccountArgs dataArgs);


	private:
		static Model::Result getReqResult(string address, string PostJsonData, CJsonObject oJson);
		//请求接口获取数据
		static Model::Result GetRequestResult(Model::BusinessType businessType, string data);

		static string GBKToUTF8(const char* strGBK);

		static	string UTF8ToGBK(const char* strUTF8);


	};

}
#endif