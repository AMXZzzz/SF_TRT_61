#include "sf-trt.h"
#include <atltime.h>
#include "config-module.h"


std::shared_ptr<spdlog::logger> g_logger;	
BOOL InitSpdLog() {
	static char temp[MAX_PATH]{};

	CTime t = CTime::GetCurrentTime();
	_snprintf_s(temp, MAX_PATH, "logs/%d-%d-%d %d-%d-%d.txt", t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	g_logger = spdlog::basic_logger_mt("SF_TRT", temp);
	//g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");	// 不显示程序
	g_logger->flush_on(spdlog::level::trace);					// 事件触发立即刷新
	g_logger->info("SF_TRT 6.1，本程序为免费程序！！！");
	//g_logger->flush();							// 手动触发刷新	
	//spdlog::flush_every(std::chrono::seconds(5));	// 每5秒刷新缓存
	return TRUE;
}

/*
	编码转换
*/
static std::wstring UTF8ToUnicode(const char* strSrc) {
	std::wstring wstrRet;

	if (NULL != strSrc) {
		int len = MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, NULL, 0) * sizeof(WCHAR);
		WCHAR* strDst = new(std::nothrow) WCHAR[len + 1];
		if (NULL != strDst) {
			MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, strDst, len);
			wstrRet = strDst;;
			delete[]strDst;
		}
	}
	return wstrRet;
}
static std::string UnicodeToAnsi(const WCHAR* strSrc) {
	std::string strRet;

	if (NULL != strSrc) {
		int len = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
		char* strDst = new(std::nothrow) char[len + 1];
		if (NULL != strDst) {
			WideCharToMultiByte(CP_ACP, 0, strSrc, -1, strDst, len, NULL, NULL);
			strRet = strDst;
			delete[]strDst;
		}
	}
	return strRet;
}
std::string UTF8ToAnsi(const char* strSrc) {
	return UnicodeToAnsi(UTF8ToUnicode(strSrc).c_str());
}
/*
	将ANSII编码的string转为utf-8 等效u8
*/
std::string StringToUTF8(const std::string& str) {
	size_t nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	size_t nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}


std::string GetIniPath() {
	ConfigModule* config = &ConfigModule::Get();
	char exe_path[MAX_PATH];
	std::string comfig_name = config->Config_name;
	comfig_name = "\\" + comfig_name;
	GetModuleFileNameA(NULL, exe_path, MAX_PATH);
	std::string root_path = exe_path;
	root_path = (root_path.substr(0, root_path.find_last_of("\\"))) + comfig_name;
	return root_path;
}

const wchar_t* GetWC(const char* c) {
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}


std::wstring String2WString(const std::string& s)   //多字节转成宽字符
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}