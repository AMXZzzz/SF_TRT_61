#include "../Help头文件/NetworkVerHelp.h"
namespace Help
{
	//瑞科网络验证：www.rukeyz.com

	static bool IsIniSoftInfo = false;

	//平台类型  1:网络验证平台  2:自动发货平台
	static Model::PlatformType platformtypeid;

	static Model::In_HeartBeatArgs dataArgsTemp;

	static string  endtimeTemp = "";
	static int	surpluspointvalueTemp = 0;

	// 网络验证服务器地址
	static string ApiAddress = "http://api.ruikeyz.com/NetVer/webapi";

	//订单查询地址
	static string SearchOrderAddress;

	// 加密的key
	// 注意：如果软件设置了加密通讯，那么此值必填
	static string encryptKey;

	// 签名盐(此参数是在软件列表里面进行设置)
	// 注意：如果软件设置了加密通讯，那么此值必填
	static string signSalt;

	//请求接口时，业务数据data参数加密码通讯类型(此参数是在软件列表里面进行设置)
	static Model::EncryptType encrypttypeid;

	//软件管理-->软件列表,里面找到相对应的goodscode,填写到此处
	static string goodscode;

	// 个人中心里面可以查看得到。代理商的话，那么在：代理管理--》代理商列表，可以查看得到
	// 注意：如果是作者自己就填写自己的platformUserCode,如果是代理商的，得填写代理商的Code
	static string platformUserCode;

	//初始化软件Key值(通过软件初始化接口【iniSoftInfo】获取，除了"软件初始化接口"不需要填写，其它接口必填写)
	static string inisoftkey;

	//软件初始化的结果对象
	Model::Out_iniSoftInfo iniSoftInfoData;

#pragma region GBKToUTF8
	string NetworkVerHelp::GBKToUTF8(const char* strGBK)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
		string strTemp = str;
		if (wstr) delete[] wstr;
		if (str) delete[] str;
		return strTemp;
	}
#pragma endregion

#pragma region UTF8ToGBK
	string NetworkVerHelp::UTF8ToGBK(const char* strUTF8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
		wchar_t* wszGBK = new wchar_t[len + 1];
		memset(wszGBK, 0, len * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
		char* szGBK = new char[len + 1];
		memset(szGBK, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
		string strTemp(szGBK);
		if (wszGBK) delete[] wszGBK;
		if (szGBK) delete[] szGBK;
		return strTemp;
	}
#pragma endregion

#pragma region 获取时间戳
	//获取时间戳
	__int64 NetworkVerHelp::GetTimeStamp()
	{
		__int64 result = 0;
		SYSTEMTIME tmSys;
		GetLocalTime(&tmSys);
		time_t curtime;
		time(&curtime);
		/*__int64 tmDst = __int64(curtime) * 1000 + tmSys.wMilliseconds;*/
		result = __int64(curtime) * 1000 + tmSys.wMilliseconds;
		string csTime;
		//_i64toa(tmDst, (char *)csTime.c_str(), 10);
		return result;
	}
#pragma endregion

#pragma region 获取机器码

	static	int GetMacValue(char * mac)
	{
		NCB ncb;
		typedef struct _ASTAT_
		{
			ADAPTER_STATUS   adapt;
			NAME_BUFFER   NameBuff[30];
		}ASTAT, *PASTAT;

		ASTAT Adapter;

		typedef struct _LANA_ENUM
		{
			UCHAR   length;
			UCHAR   lana[MAX_LANA];
		}LANA_ENUM;

		LANA_ENUM lana_enum;
		UCHAR uRetCode;
		memset(&ncb, 0, sizeof(ncb));
		memset(&lana_enum, 0, sizeof(lana_enum));
		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *)&lana_enum;
		ncb.ncb_length = sizeof(LANA_ENUM);
		uRetCode = Netbios(&ncb);

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		for (int lana = 0; lana < lana_enum.length; lana++)
		{
			ncb.ncb_command = NCBRESET;
			ncb.ncb_lana_num = lana_enum.lana[lana];
			uRetCode = Netbios(&ncb);
			if (uRetCode == NRC_GOODRET)
				break;
		}

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = lana_enum.lana[0];
		strcpy((char*)ncb.ncb_callname, "*");

		ncb.ncb_buffer = (unsigned char *)&Adapter;
		ncb.ncb_length = sizeof(Adapter);
		uRetCode = Netbios(&ncb);

		if (uRetCode != NRC_GOODRET)
			return uRetCode;

		sprintf(mac, "%02X%02X%02X%02X%02X%02X",
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]);
		return 0;
	}


	BOOL GetCpuByCmd(char *lpszCpu, int len/*=128*/)
	{
		const long MAX_COMMAND_SIZE = 10000; // 命令行输出缓冲大小	
		WCHAR szFetCmd[] = L"wmic cpu get processorid"; // 获取CPU序列号命令行	
		const string strEnSearch = "ProcessorId"; // CPU序列号的前导信息

		BOOL   bret = FALSE;
		HANDLE hReadPipe = NULL; //读取管道
		HANDLE hWritePipe = NULL; //写入管道	
		PROCESS_INFORMATION pi;   //进程信息	
		STARTUPINFO			si;	  //控制命令行窗口信息
		SECURITY_ATTRIBUTES sa;   //安全属性

		char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // 放置命令行结果的输出缓冲区
		string			strBuffer;
		unsigned long	count = 0;
		long			ipos = 0;

		memset(&pi, 0, sizeof(pi));
		memset(&si, 0, sizeof(si));
		memset(&sa, 0, sizeof(sa));

		pi.hProcess = NULL;
		pi.hThread = NULL;
		si.cb = sizeof(STARTUPINFO);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		//1.0 创建管道
		bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
		if (!bret)
		{
			//关闭所有的句柄
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return(bret);
		}

		//2.0 设置命令行窗口的信息为指定的读写管道
		GetStartupInfo(&si);
		si.hStdError = hWritePipe;
		si.hStdOutput = hWritePipe;
		si.wShowWindow = SW_HIDE; //隐藏命令行窗口
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//3.0 创建获取命令行的进程
		bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		if (!bret)
		{
			//关闭所有的句柄
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return(bret);
		}

		//4.0 读取返回的数据
		WaitForSingleObject(pi.hProcess, 500/*INFINITE*/);
		bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
		if (!bret)
		{
			//关闭所有的句柄
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return(bret);
		}

		//5.0 查找CPU序列号
		bret = FALSE;
		strBuffer = szBuffer;
		ipos = strBuffer.find(strEnSearch);

		if (ipos < 0) // 没有找到
		{
			//关闭所有的句柄
			CloseHandle(hWritePipe);
			CloseHandle(hReadPipe);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return(bret);
		}
		else
		{
			strBuffer = strBuffer.substr(ipos + strEnSearch.length());
		}

		memset(szBuffer, 0x00, sizeof(szBuffer));
		strcpy_s(szBuffer, strBuffer.c_str());

		//去掉中间的空格 \r \n
		int j = 0;
		for (int i = 0; i < strlen(szBuffer); i++)
		{
			if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
			{
				lpszCpu[j] = szBuffer[i];
				j++;
			}
		}

		bret = TRUE;

		return(bret);
	}


	//获取机器码
	string NetworkVerHelp::GetMac()
	{
		char lpszMac[128] = { 0 };
		if (GetCpuByCmd(lpszMac, 128))
		{
			string s = lpszMac;
			MD5 md5(s);
			s = md5.toString();
			s = (s.substr(11, 3) + s.substr(7, 3) + s.substr(3, 3) + s.substr(s.length() - 3));
			return s;
		}

		//初始化机器码
		char macbuf[24] = "\0";
		GetMacValue(macbuf);//获取mac当机器码
		return macbuf;

		/*return GetId();*/

	}






#pragma endregion



	void NetworkVerHelp::ini()
	{
		if (ApiAddress == "")
		{
			list<string> ApiArray;

			ApiArray.push_back("http://api.ruikeyz.com/");
			ApiArray.push_back("http://128.14.75.157:8080/");
			ApiArray.push_back("http://107.148.160.77:8080/");
			ApiArray.push_back("http://107.148.190.233:8080/");
			ApiArray.push_back("http://154.38.227.12:8080/");

			CJsonObject oJson;
			list<string>::iterator it = ApiArray.begin();
			while (it != ApiArray.end())
			{
				string address = *it;

				CHttpRequestTool http;
				list<string> head;
				head.push_back("Accept: application/json, text/javascript, */*;");
				head.push_back("Content-Type: application/json;charset=UTF-8");
				string ResponseResult = http.HttpGet(address, 1000 * 60);
				head.clear();


				//把接口返回来的数据反序列化成对象
				oJson.Clear();
				oJson.Parse(ResponseResult);
				Model::Result result;
				oJson.Get("code", result.code);
				oJson.Get("msg", result.msg);
				oJson.Get("data", result.data);

				if (result.code == 0)
				{
					ApiAddress = address + "NetVer/webapi";
					break;
				}

			}
		}

	}





#pragma region 请求接口获取数据

	Model::Result NetworkVerHelp::getReqResult(string address, string PostJsonData, CJsonObject oJson) {
		Model::Result result;
		result.code = -999;
		result.msg = "服务器访问失败，您本地网络不顺畅，请检查下（或许您用了代理IP了，当时的代理IP不稳定？）";

		CHttpRequestTool http;
		list<string> head;
		head.push_back("Accept: application/json, text/javascript, */*;");
		head.push_back("Content-Type: application/json;charset=UTF-8");

		string ResponseResult = http.HttpPost(address, PostJsonData, head, 1000 * 30);
		if (oJson.Parse(ResponseResult))
		{
			oJson.Get("code", result.code);
			oJson.Get("msg", result.msg);
			oJson.Get("data", result.data);
		}
		head.clear();
		http.~CHttpRequestTool();
		return result;
	}

	//请求接口获取数据
	Model::Result NetworkVerHelp::GetRequestResult(Model::BusinessType businessType, string data)
	{
		//VMProtectBegin("GetRequestResult");
		Model::Result result;

		string sign = "";//登录成功后，并且软件设置了加密码通讯，那么sign是必填项
		__int64 TimeStamp = GetTimeStamp();
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			//如果软件设置了加密码通讯
			if (signSalt == "")
			{
				result.code = -999;
				result.msg = "签名盐不能为空！";
				return result;
			}
			if (encryptKey == "")
			{
				result.code = -999;
				result.msg = "加密key不能为空！";
				return result;
			}
		}

		switch (encrypttypeid)
		{
		case Model::NoEncrypt:
			break;
		case Model::RC4:
			string	UTF8data = GBKToUTF8(data.c_str());
			string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
			Rc4 rc4;
			data = rc4.Encrypt(UTF8data.c_str(), UTF8encryptKey.c_str());

			//注意加密顺序不能变:业务类型+加密类型+平台用户Code+商品Code+inisoftkey+时间戳+业务数据+签名盐+平台类型
			string signData = to_string(businessType) + to_string(encrypttypeid) + platformUserCode + goodscode
				+ inisoftkey + to_string(TimeStamp) + data + signSalt + to_string(platformtypeid);
			signData = GBKToUTF8(signData.c_str());

			MD5 md5(signData);
			sign = md5.toString();
			break;
		}


		Model::ApiRequestArgs ApiArgs;
		ApiArgs.businessid = businessType;//必填
		ApiArgs.platformusercode = platformUserCode;//必填
		ApiArgs.encrypttypeid = encrypttypeid;//必填
		ApiArgs.goodscode = goodscode;//必填
		ApiArgs.timestamp = TimeStamp;//必填
		ApiArgs.data = data;//必填
		ApiArgs.sign = sign;//登录成功后，并且软件设置了加密码通讯，那么sign是必填项，否则可以为空
		ApiArgs.platformtypeid = platformtypeid;//必填


		CJsonObject oJson;
		oJson.Add("businessid", ApiArgs.businessid);
		oJson.Add("platformusercode", ApiArgs.platformusercode);
		oJson.Add("encrypttypeid", ApiArgs.encrypttypeid);
		oJson.Add("goodscode", ApiArgs.goodscode);
		oJson.Add("inisoftkey", inisoftkey);
		oJson.Add("timestamp", ApiArgs.timestamp);
		oJson.Add("data", ApiArgs.data);
		oJson.Add("sign", ApiArgs.sign);
		oJson.Add("platformtypeid", ApiArgs.platformtypeid);
		string PostJsonData = oJson.ToString();
		oJson.Clear();

		int ReqNum = 3;
		int repLogOutNum = 0;
		int SleepReqTime = 1;

		for (size_t i = 0; i < ReqNum; i++)
		{
			result = getReqResult(ApiAddress, PostJsonData, oJson);
			if (result.code == -999)
			{
				result = getReqResult("http://api2.ruikeyz.com/NetVer/webapi", PostJsonData, oJson);
				if (businessType == Model::BusinessType::heartBeat)
				{
					if ( result.code == 1012) {
						Model::Out_heartBeat out_HeartBeat1;
						out_HeartBeat1.heartbeatkey = dataArgsTemp.heartbeatkey;
						out_HeartBeat1.requestflag = dataArgsTemp.requestflag;
						out_HeartBeat1.endtime = endtimeTemp;
						out_HeartBeat1.surpluspointvalue = surpluspointvalueTemp;
						CJsonObject oJson2;
						oJson2.Add("heartbeatkey", out_HeartBeat1.heartbeatkey);
						oJson2.Add("requestflag", out_HeartBeat1.requestflag);
						oJson2.Add("endtime", out_HeartBeat1.endtime);
						oJson2.Add("surpluspointvalue", out_HeartBeat1.surpluspointvalue);
						string	data2 = oJson2.ToString();
						switch (encrypttypeid)
						{
						case Model::NoEncrypt:
							break;
						case Model::RC4:
							string	UTF8data2 = GBKToUTF8(data2.c_str());
							string	UTF8encryptKey2 = GBKToUTF8(encryptKey.c_str());
							Rc4 rc4;
							data2 = rc4.Encrypt(UTF8data2.c_str(), UTF8encryptKey2.c_str());
							break;
						}
						result.data = data2;
						result.code = 0;
						break;
					}
				}
			}
			if (businessType == Model::BusinessType::heartBeat)
			{
				if (result.code == -999 || result.code == 1001 || result.code == 1002) {
					Model::Out_heartBeat out_HeartBeat1;
					out_HeartBeat1.heartbeatkey = dataArgsTemp.heartbeatkey;
					out_HeartBeat1.requestflag = dataArgsTemp.requestflag;
					out_HeartBeat1.endtime = endtimeTemp;
					out_HeartBeat1.surpluspointvalue = surpluspointvalueTemp;
					CJsonObject oJson2;
					oJson2.Add("heartbeatkey", out_HeartBeat1.heartbeatkey);
					oJson2.Add("requestflag", out_HeartBeat1.requestflag);
					oJson2.Add("endtime", out_HeartBeat1.endtime);
					oJson2.Add("surpluspointvalue", out_HeartBeat1.surpluspointvalue);
					string	data2 = oJson2.ToString();
					switch (encrypttypeid)
					{
					case Model::NoEncrypt:
						break;
					case Model::RC4:
						string	UTF8data2 = GBKToUTF8(data2.c_str());
						string	UTF8encryptKey2 = GBKToUTF8(encryptKey.c_str());
						Rc4 rc4;
						data2 = rc4.Encrypt(UTF8data2.c_str(), UTF8encryptKey2.c_str());
						break;
					}
					result.data = data2;
					result.code = 0;
					break;
				}
			}
			if (result.code == -999)
			{
				result = getReqResult("http://api3.ruikeyz.com/NetVer/webapi", PostJsonData, oJson);
			}
			if (result.code != -999)
			{
				break;
			}
			Sleep(1000 * SleepReqTime);
		}

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region json字符串转基础返回类型
	//json字符串转基础返回类型
	Model::Out_DataBaseArgs NetworkVerHelp::ConvertOutDataBaseArgs(string data)
	{
		//VMProtectBegin("ConvertOutDataBaseArgs");
		Model::Out_DataBaseArgs result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}


		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 初始化软件业务数据结果转对象
	//初始化软件业务数据结果转对象
	Model::Out_iniSoftInfo NetworkVerHelp::ConvertIniSoftInfoData(string data)
	{
		//VMProtectBegin("ConvertIniSoftInfoData");
		Model::Out_iniSoftInfo result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}


		CJsonObject oJson(data);
		oJson.Get("inisoftkey", result.inisoftkey);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson["softInfo"].Get("softname", result.softInfo.softname);
		oJson["softInfo"].Get("consultwebsite", result.softInfo.consultwebsite);
		oJson["softInfo"].Get("consultqq", result.softInfo.consultqq);
		oJson["softInfo"].Get("consultwx", result.softInfo.consultwx);
		oJson["softInfo"].Get("consulttel", result.softInfo.consulttel);
		oJson["softInfo"].Get("logourl", result.softInfo.logourl);
		oJson["softInfo"].Get("notice", result.softInfo.notice);
		oJson["softInfo"].Get("opentestcount", result.softInfo.opentestcount);
		oJson["softInfo"].Get("opentestday", result.softInfo.opentestday);
		oJson["softInfo"].Get("basedata", result.softInfo.basedata);
		oJson["softInfo"].Get("newversionnum", result.softInfo.newversionnum);
		oJson["softInfo"].Get("networkdiskurl", result.softInfo.networkdiskurl);
		oJson["softInfo"].Get("diskpwd", result.softInfo.diskpwd);
		oJson["softInfo"].Get("isforceupd", result.softInfo.isforceupd);





		int PriceCount = oJson["softpricelist"].GetArraySize();
		for (size_t i = 0; i < PriceCount; i++)
		{
			Model::Out_softPrice softPrice;
			oJson["softpricelist"][i].Get("priceid", softPrice.priceid);
			oJson["softpricelist"][i].Get("pricetypename", softPrice.pricetypename);
			oJson["softpricelist"][i].Get("consumevalue", softPrice.consumevalue);
			oJson["softpricelist"][i].Get("price", softPrice.price);
			oJson["softpricelist"][i].Get("maxloginnum", softPrice.maxloginnum);
			oJson["softpricelist"][i].Get("roleid", softPrice.roleid);
			oJson["softpricelist"][i].Get("rolename", softPrice.rolename);
			result.softpricelist.push_back(softPrice);
		}

		return result;

		//VMProtectEnd();
	}
#pragma endregion

#pragma region 软件价格列表字符串转对象 
	//软件价格列表字符串转对象 
	Model::Out_SoftPriceList NetworkVerHelp::ConvertSoftPriceListData(string data)
	{
		//VMProtectBegin("ConvertSoftPriceListData");
		Model::Out_SoftPriceList result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}


		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);

		int PriceCount = oJson["softpricelist"].GetArraySize();
		for (size_t i = 0; i < PriceCount; i++)
		{
			Model::Out_softPrice softPrice;
			oJson["softpricelist"][i].Get("priceid", softPrice.priceid);
			oJson["softpricelist"][i].Get("pricetypename", softPrice.pricetypename);
			oJson["softpricelist"][i].Get("consumevalue", softPrice.consumevalue);
			oJson["softpricelist"][i].Get("price", softPrice.price);
			oJson["softpricelist"][i].Get("maxloginnum", softPrice.maxloginnum);
			oJson["softpricelist"][i].Get("roleid", softPrice.roleid);
			oJson["softpricelist"][i].Get("rolename", softPrice.rolename);
			result.softpricelist.push_back(softPrice);
		}

		return result;

		//VMProtectEnd();
	}
#pragma endregion

#pragma region 登录业务结果转转对象
	//登录业务结果转转对象
	Model::Out_Login NetworkVerHelp::ConvertLoginData(string data)
	{
		//VMProtectBegin("ConvertLoginData");
		Model::Out_Login result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}

		CJsonObject oJson(data);
		int ispay = 0;
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("token", result.token);
		oJson.Get("heartbeatkey", result.heartbeatkey);
		oJson.Get("endtime", result.endtime);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);
		oJson.Get("currentloginnum", result.currentloginnum);
		oJson.Get("priceid", result.priceid);
		oJson.Get("pricename", result.pricename);
		oJson.Get("ispay", ispay);
		oJson.Get("roleid", result.roleid);
		oJson.Get("rolename", result.rolename);
		oJson.Get("maxloginnum", result.maxloginnum);
		oJson.Get("qq", result.qq);
		oJson.Get("wx", result.wx);
		oJson.Get("alipay", result.alipay);
		oJson.Get("tel", result.tel);
		oJson.Get("email", result.email);
		oJson.Get("remarks", result.remarks);
		result.ispay = (Model::IsPay)ispay;

		int JurisdictionCount = oJson["jurisdictionlist"].GetArraySize();
		for (size_t i = 0; i < JurisdictionCount; i++)
		{
			Model::Out_LoginJurisdiction Jurisdiction;
			oJson["jurisdictionlist"][i].Get("jurisdictionid", Jurisdiction.jurisdictionid);
			oJson["jurisdictionlist"][i].Get("jurisdictionname", Jurisdiction.jurisdictionname);
			oJson["jurisdictionlist"][i].Get("jurisdictvalue", Jurisdiction.jurisdictvalue);
			result.jurisdictionlist.push_back(Jurisdiction);
		}

		endtimeTemp = result.endtime;
		surpluspointvalueTemp = result.surpluspointvalue;

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 心跳业务结果转对象
	//心跳业务结果转对象
	Model::Out_heartBeat NetworkVerHelp::ConvertHeartBeat(string data)
	{
		//VMProtectBegin("ConvertHeartBeat");
		Model::Out_heartBeat result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("heartbeatkey", result.heartbeatkey);
		oJson.Get("endtime", result.endtime);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 扣点业务结果转对象
	//扣点业务结果转对象
	Model::Out_bucklePoint NetworkVerHelp::ConvertBucklePoint(string data)
	{
		//VMProtectBegin("ConvertBucklePoint");
		Model::Out_bucklePoint result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 软件变量业务结果转化成对象
	//软件变量业务结果转化成对象
	Model::Out_getRemoteVar NetworkVerHelp::ConvertRemoteVar(string data)
	{
		//VMProtectBegin("ConvertRemoteVar");
		Model::Out_getRemoteVar result;

		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}
		}

		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		int varlistCount = oJson["varlist"].GetArraySize();
		for (size_t i = 0; i < varlistCount; i++)
		{
			Model::Out_Var VarItem;
			oJson["varlist"][i].Get("varvalue", result.varValue);
			break;

			//oJson["varlist"][i].Get("varname", VarItem.varname);
			//oJson["varlist"][i].Get("varvalue", VarItem.varvalue);
			//result.varlist.push_back(VarItem);
		}

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 解绑机器码结果转化成对象
	//解绑机器码结果转化成对象
	Model::Out_unbundMacArgs NetworkVerHelp::ConvertUnbundMac(string data)
	{
		//VMProtectBegin("ConvertUnbundMac");
		Model::Out_unbundMacArgs result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("endtime", result.endtime);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 在线支付购买卡密结果转对象
	//在线支付购买卡密结果转对象
	Model::Out_BuyCardNum NetworkVerHelp::ConvertBuyCardNum(string data)
	{
		//VMProtectBegin("ConvertBuyGoods");
		Model::Out_BuyCardNum result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("orderid", result.orderid);
		oJson.Get("payaddress", result.payaddress);
		oJson.Get("ordersearchaddress", result.ordersearchaddress);
		return result;
		//VMProtectEnd();
	}
#pragma endregion


#pragma region 在线支付开通/续费卡密结果转对象
	//在线支付开通/续费卡密结果转对象
	Model::Out_OpenRenewCardNum NetworkVerHelp::ConvertOpenRenewCardNum(string data)
	{
		//VMProtectBegin("ConvertOpenRenewCardNum");
		Model::Out_OpenRenewCardNum result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("orderid", result.orderid);
		oJson.Get("payaddress", result.payaddress);
		oJson.Get("ordersearchaddress", result.ordersearchaddress);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 在线支付开通续费账号结果转对象
	//在线支付开通续费账号结果转对象
	Model::Out_OpenRenewAccount NetworkVerHelp::ConvertOpenRenewAccount(string data)
	{
		//VMProtectBegin("ConvertOpenRenewAccount");
		Model::Out_OpenRenewAccount result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("orderid", result.orderid);
		oJson.Get("payaddress", result.payaddress);
		oJson.Get("ordersearchaddress", result.ordersearchaddress);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 卡密详情结果转对象
	//卡密详情结果转对象
	Model::Out_cardDetail NetworkVerHelp::ConvertCardDetail(string data)
	{
		//VMProtectBegin("ConvertCardDetail");
		Model::Out_cardDetail result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		int ispay = 0;
		int openstate = 0;
		int activestate = 0;
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("endtime", result.endtime);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);
		oJson.Get("maccode", result.maccode);
		oJson.Get("priceid", result.priceid);
		oJson.Get("pricename", result.pricename);
		oJson.Get("ispay", ispay);
		oJson.Get("qq", result.qq);
		oJson.Get("wx", result.wx);
		oJson.Get("alipay", result.alipay);
		oJson.Get("tel", result.tel);
		oJson.Get("email", result.email);
		oJson.Get("regtime", result.regtime);
		oJson.Get("openstate", openstate);
		oJson.Get("activestate", activestate);
		oJson.Get("remarks", result.remarks);
		result.ispay = (Model::IsPay)ispay;
		result.openstate = (Model::OpenState)openstate;
		result.activestate = (Model::ActiveState)activestate;
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 账号详情结果转对象
	//账号详情结果转对象
	Model::Out_accountDetail NetworkVerHelp::ConvertAccountDetail(string data)
	{
		//VMProtectBegin("ConvertAccountDetail");
		Model::Out_accountDetail result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		int ispay = 0;
		int openstate = 0;
		int activestate = 0;
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("endtime", result.endtime);
		oJson.Get("surpluspointvalue", result.surpluspointvalue);
		oJson.Get("maccode", result.maccode);
		oJson.Get("priceid", result.priceid);
		oJson.Get("pricename", result.pricename);
		oJson.Get("ispay", ispay);
		oJson.Get("qq", result.qq);
		oJson.Get("wx", result.wx);
		oJson.Get("alipay", result.alipay);
		oJson.Get("tel", result.tel);
		oJson.Get("email", result.email);
		oJson.Get("regtime", result.regtime);
		oJson.Get("openstate", openstate);
		oJson.Get("activestate", activestate);
		oJson.Get("remarks", result.remarks);
		result.ispay = (Model::IsPay)ispay;
		result.openstate = (Model::OpenState)openstate;
		result.activestate = (Model::ActiveState)activestate;
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 订单查询结果转对象
	//订单查询结果转对象
	Model::Out_SearchOrder NetworkVerHelp::ConvertSearchOrder(string data)
	{
		//VMProtectBegin("ConvertSearchOrder");
		Model::Out_SearchOrder result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		int orderstate = 0;
		int businesscompletetype = 0;
		int paytypeid = 0;
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("orderstate", orderstate);
		oJson.Get("businesscompletetype", businesscompletetype);
		oJson.Get("paytypeid", paytypeid);
		oJson.Get("price", result.price);
		oJson.Get("revicemoney", result.revicemoney);
		oJson.Get("cardnum", result.cardnum);
		oJson.Get("goodsnetworkdiskurl", result.goodsnetworkdiskurl);
		oJson.Get("diskpwd", result.diskpwd);
		oJson.Get("maccode", result.maccode);
		oJson.Get("orderflag", result.orderflag);
		oJson.Get("createtime", result.createtime);
		result.orderstate = (Model::OrderState)orderstate;
		result.businesscompletetype = (Model::BusinessCompleteType)businesscompletetype;
		result.paytypeid = (Model::PayType)paytypeid;

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 充值卡详情结果转对象
	//充值卡详情结果转对象
	Model::Out_rechCardNumDetail NetworkVerHelp::ConvertRechCardDetail(string data)
	{
		//VMProtectBegin("ConvertCardDetail");
		Model::Out_rechCardNumDetail result;
		if (data == "")
		{
			throw exception("data不能为空！");
		}
		if (encrypttypeid != Model::EncryptType::NoEncrypt)
		{
			if (encryptKey == "")
			{
				throw exception("加密key不能为空！");
			}

			switch (encrypttypeid)
			{
			case Model::NoEncrypt:
				break;
			case Model::RC4:
				//如果软件设置是加密通讯，那么此处需要把data解密出来
				string	UTF8encryptKey = GBKToUTF8(encryptKey.c_str());
				Rc4 rc4;
				data = rc4.Decrypt(data.c_str(), UTF8encryptKey.c_str());
				data = UTF8ToGBK(data.c_str());
				break;
			}

		}
		CJsonObject oJson(data);
		oJson.Get("requestflag", result.requestflag);
		oJson.Get("servertimestamp", result.servertimestamp);
		oJson.Get("softname", result.softname);
		oJson.Get("consumevaluename", result.consumevaluename);
		oJson.Get("consumetypename", result.consumetypename);
		oJson.Get("rechcardstatename", result.rechcardstatename);
		return result;
		//VMProtectEnd();
	}
#pragma endregion




#pragma region 初始化软件
	//初始化软件
	Model::Result NetworkVerHelp::GetIniSoftInfo(Model::In_IniSoftInfoArgs dataArgs, Model::PlatformType _platformtypeid, string _encryptKey,
		string _signSalt, Model::EncryptType _encrypttypeid, string _goodscode, string _platformUserCode)
	{
		//VMProtectBegin("GetIniSoftInfo");
		Model::Result result;
		if (IsIniSoftInfo == false)
		{
			//NetworkVerHelp::ini();

			platformtypeid = _platformtypeid;
			encryptKey = _encryptKey;
			signSalt = _signSalt;
			encrypttypeid = _encrypttypeid;
			goodscode = _goodscode;
			platformUserCode = _platformUserCode;

			size_t length = ApiAddress.find_last_of("/");
			if (length + 1 == ApiAddress.size())
			{
				ApiAddress = ApiAddress.substr(0, length - 1);
			}
			SearchOrderAddress = ApiAddress.substr(0, ApiAddress.find_last_of("/")) + "/SearchOrder";


			string data = "";
			CJsonObject oJson;
			oJson.Add("requestflag", dataArgs.requestflag);
			oJson.Add("versionname", dataArgs.versionname);
			oJson.Add("maccode", dataArgs.maccode);
			oJson.Add("timestamp", dataArgs.timestamp);
			data = oJson.ToString();

			result = GetRequestResult(Model::BusinessType::iniSoftInfo, data);
			if (result.code == 0)
			{
				switch (platformtypeid)
				{
				case Model::NetVer:
					iniSoftInfoData = ConvertIniSoftInfoData(result.data);
					inisoftkey = iniSoftInfoData.inisoftkey;//初始化完软件后，把此Key保留下来，其它接口此Key必填写的
					break;
				}

			}


		}
		else
		{
			result.msg = "软件已初始化过了，不能再次进行初始化";
		}


		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 获取软件价格列表
	//获取软件价格列表
	Model::Result NetworkVerHelp::getSoftPriceList(Model::In_getSoftPriceListArgs dataArgs)
	{
		//VMProtectBegin("getSoftPriceList");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		data = oJson.ToString();

		result = GetRequestResult(Model::BusinessType::softPriceList, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 登录成功后，开始心跳(心跳间隔时间不能小于5分钟)
	//登录成功后，开始心跳(心跳间隔时间不能小于5分钟)
	Model::Result NetworkVerHelp::GetHeartBeat(Model::In_HeartBeatArgs dataArgs)
	{
		//VMProtectBegin("GetHeartBeat");
		Model::Result result;
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("token", dataArgs.token);
		oJson.Add("heartbeatkey", dataArgs.heartbeatkey);
		data = oJson.ToString();

		dataArgsTemp = dataArgs;
		result = GetRequestResult(Model::BusinessType::heartBeat, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 卡密登录
	//卡密登录
	Model::Result NetworkVerHelp::GetLoginByCard(Model::In_CardLoginArgs dataArgs)
	{
		//VMProtectBegin("GetLoginByCard");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnum", dataArgs.cardnum);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::cardLogin, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 在线支付购买卡密
	//在线支付购买卡密
	Model::Result NetworkVerHelp::GetBuyCardNum(Model::In_BuyCardNumArgs dataArgs)
	{
		//VMProtectBegin("GetLoginByCard");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("priceid", dataArgs.priceid);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::buyCardNum, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 在线支付开通/续费卡密

	//在线支付开通/续费卡密
	Model::Result NetworkVerHelp::GeOpenRenewCardNum(Model::In_OpenRenewCardNumArgs dataArgs)
	{
		//VMProtectBegin("GeOpenRenewCardNum");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnum", dataArgs.cardnum);
		oJson.Add("priceid", dataArgs.priceid);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::openRenewCardNum, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 卡密详情

	//卡密详情
	Model::Result NetworkVerHelp::GeCardDetail(Model::In_cardDetailArgs dataArgs)
	{
		//VMProtectBegin("GeCardDetail");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnum", dataArgs.cardnum);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::cardDetail, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 注册账号
	//注册账号
	Model::Result NetworkVerHelp::GeAccountRegister(Model::In_AccountRegisterArgs dataArgs)
	{
		//VMProtectBegin("GeCardDetail");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("username", dataArgs.username);
		oJson.Add("userpwd", dataArgs.userpwd);
		oJson.Add("qq", dataArgs.qq);
		oJson.Add("wx", dataArgs.wx);
		oJson.Add("alipay", dataArgs.alipay);
		oJson.Add("tel", dataArgs.tel);
		oJson.Add("email", dataArgs.email);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::accountRegister, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 账号密码登录
	//账号密码登录
	Model::Result NetworkVerHelp::GeAccountLogin(Model::In_AccountLoginArgs dataArgs)
	{
		//VMProtectBegin("GeAccountLogin");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("username", dataArgs.username);
		oJson.Add("userpwd", dataArgs.userpwd);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::accountLogin, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 修改用户密码
	//修改用户密码
	Model::Result NetworkVerHelp::GeUpdPwd(Model::In_UpdPwdArgs dataArgs)
	{
		//VMProtectBegin("GeUpdPwd");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("username", dataArgs.username);
		oJson.Add("userpwd", dataArgs.userpwd);
		oJson.Add("newpwd", dataArgs.newpwd);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::updPwd, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 在线支付开通续费账号
	//在线支付开通续费账号
	Model::Result NetworkVerHelp::GeOpenRenewAccount(Model::In_OpenRenewAccountArgs dataArgs)
	{
		//VMProtectBegin("GeOpenRenewAccount");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("username", dataArgs.username);
		oJson.Add("userpwd", dataArgs.userpwd);
		oJson.Add("priceid", dataArgs.priceid);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::openRenewAccount, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 账号详情
	//账号详情
	Model::Result NetworkVerHelp::GeAccountDetail(Model::In_accountDetailArgs dataArgs)
	{
		//VMProtectBegin("GeAccountDetail");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("username", dataArgs.username);
		oJson.Add("userpwd", dataArgs.userpwd);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::accountDetail, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 扣点
	//扣点
	Model::Result NetworkVerHelp::GeBucklePoint(Model::In_BucklePointArgs dataArgs)
	{
		//VMProtectBegin("GeBucklePoint");
		Model::Result result;
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("bucklevalue", dataArgs.bucklevalue);
		oJson.Add("token", dataArgs.token);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::bucklePoint, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 退出登录
	//退出登录
	Model::Result NetworkVerHelp::GeLoginOut(Model::In_LoginOutArgs dataArgs)
	{
		//VMProtectBegin("GeLoginOut");
		Model::Result result;
		string data = "";
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		CJsonObject oJson;
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("token", dataArgs.token);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::loginOut, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 获取软件变量
	//获取软件变量
	Model::Result NetworkVerHelp::getRemoteVar(Model::In_getRemoteVarArgs dataArgs)
	{
		//VMProtectBegin("getRemoteVar");
		Model::Result result;
		string data = "";
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("token", dataArgs.token);
		oJson.Add("varname", dataArgs.varname);//注意:如果变量名称为空的话，那么接口返回来的是此软件所有变量，否则就是此变量名所以应的变量值
		data = oJson.ToString();

		result = GetRequestResult(Model::BusinessType::getremoteVar, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 解绑机器码(如果软件设置解绑扣除相应的时间或点数，那么解绑成功后会自动扣除)
	//解绑机器码(如果软件设置解绑扣除相应的时间或点数，那么解绑成功后会自动扣除)
	Model::Result NetworkVerHelp::unbundMac(Model::In_unbundMacArgs dataArgs)
	{
		//VMProtectBegin("unbundMac");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::unbundMac, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 订单查询
	//订单查询
	Model::Result NetworkVerHelp::GetSearchOrder(string orderid, string requestflag)
	{
		//VMProtectBegin("GetSearchOrder");
		Model::Result result;
		switch (platformtypeid)
		{
		case Model::NetVer://网络验证平台
			if (iniSoftInfoData.inisoftkey == "")
			{
				result.code = -999;
				result.msg = "软件尚未初始化";
				return result;
			}
			break;
		}





		Model::In_SearchOrderArgs ApiArgs;
		ApiArgs.encrypttypeid = encrypttypeid;//必填
		ApiArgs.goodscode = goodscode;//必填
		ApiArgs.inisoftkey = inisoftkey;//必填
		ApiArgs.orderid = orderid;//必填
		ApiArgs.platformtypeid = platformtypeid;//必填
		ApiArgs.platformusercode = platformUserCode;//必填
		ApiArgs.requestflag = requestflag;//必填

		CJsonObject oJson;
		oJson.Add("requestflag", ApiArgs.requestflag);
		oJson.Add("encrypttypeid", ApiArgs.encrypttypeid);
		oJson.Add("goodscode", ApiArgs.goodscode);
		oJson.Add("inisoftkey", ApiArgs.inisoftkey);
		oJson.Add("orderid", ApiArgs.orderid);
		oJson.Add("platformtypeid", ApiArgs.platformtypeid);
		oJson.Add("platformusercode", ApiArgs.platformusercode);
		string PostJsonData = oJson.ToString();


		CHttpRequestTool http;
		list<string> head;
		head.push_back("Accept: application/json, text/javascript, */*;");
		head.push_back("Content-Type: application/json;charset=UTF-8");
		//接口返回来的数据(注意，这个请求的工具类，没有设置超时时间，如果接口返回的数据有些慢的话，此请求的工具类会有问题，最好自己找一个可以设置超时时间的请求工具类)
		string ResponseResult = http.HttpPost(SearchOrderAddress, PostJsonData, head, 1000 * 60 * 3);//必须是post请求，并且是提交json数据的
		int temp = ResponseResult.find("servertimestamp");
		if (temp <= 0)
		{
			result.code = -999;
			result.msg = "服务器访问失败";
			return result;
		}
		head.clear();


		//把接口返回来的数据反序列化成对象
		oJson.Clear();
		oJson.Parse(ResponseResult);
		oJson.Get("code", result.code);
		oJson.Get("msg", result.msg);
		oJson.Get("data", result.data);

		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 修改卡密/账号备注
	//修改卡密/账号备注
	Model::Result NetworkVerHelp::GetUpdRemark(Model::In_updRemarkArgs dataArgs)
	{
		//VMProtectBegin("GeUpdRemark");
		Model::Result result;
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("remarks", dataArgs.remarks);
		oJson.Add("token", dataArgs.token);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::updRemark, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 充值卡充值卡密或账号
	//充值卡充值卡密或账号
	Model::Result NetworkVerHelp::GetRechCardRenew(Model::In_RechCardRenewCardOrAccountArgs dataArgs)
	{
		//VMProtectBegin("GeUpdRemark");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("rechcardnum", dataArgs.rechcardnum);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::rechCardRenewCardOrAccount, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion

#pragma region 充值卡详情
	//充值卡详情
	Model::Result NetworkVerHelp::GetRechCardDetail(Model::In_rechCardDetailArgs dataArgs)
	{
		//VMProtectBegin("GeUpdRemark");
		Model::Result result;
		string data = "";
		CJsonObject oJson;
		oJson.Add("requestflag", dataArgs.requestflag);
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("rechcardnum", dataArgs.rechcardnum);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::rechCardNumDetail, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion


#pragma region 禁用还是删除：单码或账号
	//禁用还是删除：单码或账号
	Model::Result NetworkVerHelp::DisableCardOrAccount(Model::In_DisableCardOrAccountArgs dataArgs)
	{
		//VMProtectBegin("GeLoginOut");
		Model::Result result;
		string data = "";
		if (dataArgs.token == "")
		{
			throw exception("token登录令牌不能为空！");
		}
		CJsonObject oJson;
		oJson.Add("maccode", dataArgs.maccode);
		oJson.Add("timestamp", dataArgs.timestamp);
		oJson.Add("cardnumorusername", dataArgs.cardnumorusername);
		oJson.Add("token", dataArgs.token);
		oJson.Add("disablecardoraccounttype", dataArgs.disablecardoraccounttype);
		data = oJson.ToString();
		result = GetRequestResult(Model::BusinessType::disableCardOrAccount, data);
		return result;
		//VMProtectEnd();
	}
#pragma endregion



}