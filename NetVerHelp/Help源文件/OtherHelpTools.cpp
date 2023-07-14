#include "../Helpͷ�ļ�/OtherHelpTools.h"
namespace OtherHelp
{
	std::wstring OtherHelpTools::StoWs(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}


	//ǿ�н�������
	void OtherHelpTools::TerminateApp()
	{
		HANDLE hself = GetCurrentProcess();
		TerminateProcess(hself, 0);
	}

	// LPVOIDתCString
	string OtherHelpTools::PVOIDToString(LPVOID lp)
	{
		return (char * &)lp;
	}


	//wstring=>string
	std::string OtherHelpTools::WString2String(const std::wstring& ws)
	{
		std::string strLocale = setlocale(LC_ALL, "");
		const wchar_t* wchSrc = ws.c_str();
		size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
		char *chDest = new char[nDestSize];
		memset(chDest, 0, nDestSize);
		wcstombs(chDest, wchSrc, nDestSize);
		std::string strResult = chDest;
		delete[]chDest;
		setlocale(LC_ALL, strLocale.c_str());
		return strResult;
	}
	// string => wstring
	std::wstring OtherHelpTools::String2WString(const std::string& s)
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


}