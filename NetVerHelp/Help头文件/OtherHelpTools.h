#pragma once
#ifndef OtherHelp_H_
#define OtherHelp_H_
#include <afxinet.h>
#include <string>
#include <list>
#include<locale.h>
using namespace std;
namespace OtherHelp
{
	//其它帮助类
	class OtherHelpTools
	{
	public:
		//string转wstring
		static std::wstring StoWs(const std::string& s);


		//强行结束程序
		static void TerminateApp();

		// LPVOID转CString
		static string PVOIDToString(LPVOID lp);

		//wstring=>string
		static std::string WString2String(const std::wstring& ws);

		//string => wstring
		static std::wstring String2WString(const std::string& s);
	};
}
#endif