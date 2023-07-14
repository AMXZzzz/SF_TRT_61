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
	//����������
	class OtherHelpTools
	{
	public:
		//stringתwstring
		static std::wstring StoWs(const std::string& s);


		//ǿ�н�������
		static void TerminateApp();

		// LPVOIDתCString
		static string PVOIDToString(LPVOID lp);

		//wstring=>string
		static std::string WString2String(const std::wstring& ws);

		//string => wstring
		static std::wstring String2WString(const std::string& s);
	};
}
#endif