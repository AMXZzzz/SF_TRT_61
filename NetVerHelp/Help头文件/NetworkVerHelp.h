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

	//�����ʼ���Ľ������
	extern  Model::Out_iniSoftInfo iniSoftInfoData;

	class NetworkVerHelp
	{
	public:

		//��ȡʱ���
		static	__int64 GetTimeStamp();

		//��ȡ������
		static string GetMac();

		static void ini();

		//json�ַ���ת������������
		static Model::Out_DataBaseArgs ConvertOutDataBaseArgs(string data);

		//��ʼ���������jsonData����ת���ɶ���
		static Model::Out_iniSoftInfo ConvertIniSoftInfoData(string data);

		//����۸��б��ַ���ת���� 
		static Model::Out_SoftPriceList ConvertSoftPriceListData(string data);

		//��¼��jsonData����ת���ɶ���
		static Model::Out_Login ConvertLoginData(string data);



		//����������ת���ɶ���
		static Model::Out_heartBeat ConvertHeartBeat(string data);

		//�۵�����ת���ɶ���
		static Model::Out_bucklePoint ConvertBucklePoint(string data);

		//�������ת���ɶ���
		static Model::Out_getRemoteVar ConvertRemoteVar(string data);

		//����daga���ת���ɶ���
		static Model::Out_unbundMacArgs ConvertUnbundMac(string data);

		//����֧�������ܽ��ת����
		static Model::Out_BuyCardNum ConvertBuyCardNum(string data);

		//����֧����ͨ/���ѿ��ܽ��ת����
		static Model::Out_OpenRenewCardNum ConvertOpenRenewCardNum(string data);

		//����֧����ͨ�����˺Ž��ת����
		static Model::Out_OpenRenewAccount ConvertOpenRenewAccount(string data);

		//����������ת����
		static Model::Out_cardDetail ConvertCardDetail(string data);

		//�˺�������ת����
		static Model::Out_accountDetail ConvertAccountDetail(string data);

		//������ѯ���ת����
		static Model::Out_SearchOrder ConvertSearchOrder(string data);

		//��ֵ��������ת����
		static Model::Out_rechCardNumDetail ConvertRechCardDetail(string data);






		//��ʼ�����
		static Model::Result GetIniSoftInfo(Model::In_IniSoftInfoArgs dataArgs, Model::PlatformType _platformtypeid, string _encryptKey,
			string _signSalt, Model::EncryptType _encrypttypeid, string _softcode, string _platformUserCode);


		//��ȡ����۸��б�
		static Model::Result getSoftPriceList(Model::In_getSoftPriceListArgs dataArgs);


		//��¼�ɹ��󣬿�ʼ����(�������ʱ�䲻��С��5����)
		static Model::Result GetHeartBeat(Model::In_HeartBeatArgs dataArgs);



		//���ܵ�¼
		static Model::Result GetLoginByCard(Model::In_CardLoginArgs dataArgs);

		//����֧��������
		static Model::Result GetBuyCardNum(Model::In_BuyCardNumArgs dataArgs);

		//����֧����ͨ/���ѿ���
		static Model::Result GeOpenRenewCardNum(Model::In_OpenRenewCardNumArgs dataArgs);

		//��������
		static Model::Result GeCardDetail(Model::In_cardDetailArgs dataArgs);



		//ע���˺�
		static Model::Result GeAccountRegister(Model::In_AccountRegisterArgs dataArgs);

		//�˺������¼
		static Model::Result GeAccountLogin(Model::In_AccountLoginArgs dataArgs);

		//�޸��û�����
		static Model::Result GeUpdPwd(Model::In_UpdPwdArgs dataArgs);

		//����֧����ͨ/�����˺�
		static Model::Result GeOpenRenewAccount(Model::In_OpenRenewAccountArgs dataArgs);

		//�˺�����
		static Model::Result GeAccountDetail(Model::In_accountDetailArgs dataArgs);

		//�۵�
		static Model::Result GeBucklePoint(Model::In_BucklePointArgs dataArgs);

		//�˳���¼
		static Model::Result GeLoginOut(Model::In_LoginOutArgs dataArgs);

		//��ȡ�������
		static Model::Result getRemoteVar(Model::In_getRemoteVarArgs dataArgs);

		//��������(���������ý��۳���Ӧ��ʱ����������ô���ɹ�����Զ��۳�)
		static Model::Result unbundMac(Model::In_unbundMacArgs dataArgs);

		//������ѯ
		static Model::Result GetSearchOrder(string orderid, string requestflag);

		//�޸Ŀ���/�˺ű�ע
		static Model::Result GetUpdRemark(Model::In_updRemarkArgs dataArgs);

		//��ֵ����ֵ���ܻ��˺�
		static Model::Result GetRechCardRenew(Model::In_RechCardRenewCardOrAccountArgs dataArgs);

		//��ֵ������
		static Model::Result GetRechCardDetail(Model::In_rechCardDetailArgs dataArgs);

		//���û���ɾ����������˺�
		static Model::Result DisableCardOrAccount(Model::In_DisableCardOrAccountArgs dataArgs);


	private:
		static Model::Result getReqResult(string address, string PostJsonData, CJsonObject oJson);
		//����ӿڻ�ȡ����
		static Model::Result GetRequestResult(Model::BusinessType businessType, string data);

		static string GBKToUTF8(const char* strGBK);

		static	string UTF8ToGBK(const char* strUTF8);


	};

}
#endif