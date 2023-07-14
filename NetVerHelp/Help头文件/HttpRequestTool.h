#pragma once

#ifndef CHttpRequestTool_H_
#define CHttpRequestTool_H_
#include <afxinet.h>
#include<windows.h>
#include <string>
#include <list>
#include <cstring>
using namespace std;
namespace Help
{
	class CHttpRequestTool
	{
	public:
		CHttpRequestTool(string strAgent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.108 Safari/537.36");
		virtual ~CHttpRequestTool(void);

	public:
		/**
		 * @brief				-- Http Get ����
		 * @param url		-- input,�����url��ַ
		 * @param timeOut	-- ���շ��������ݵĳ�ʱʱ��
		 * @return				--- ����Ľ��
		 */
		string HttpGet(string url, int timeOut);

		/**
		 * @brief				-- Http Post ����
		 * @param url		-- input,�����url��ַ
		 * @param postData	-- Post������
		 * @param head	-- ͷ��
		 * @param timeOut	-- ���շ��������ݵĳ�ʱʱ��
		 * @return				--- ����Ľ��
		 */
		string HttpPost(string url, string postData, list<string> head, int timeOut);


	private:
		/**
		 * @brief				-- ��urlת��Ϊutf_8
		 * @param strText		-- CString reference
		 */
		void ConvertUnicodeToUtf8(CString& strText);

		/**
		 * @brief				-- ����ȡ������ҳmsgת��ΪUnicode
		 * @param strText		-- string reference
		 */
		void ConvertUtf8ToUnicode(string& strText);

		/**
		 * @brief				-- Http Get ����
		 * @param strUrl		-- input,�����url��ַ
		 * @param timeOut	-- ���շ��������ݵĳ�ʱʱ��
		 * @return				-- ����Ľ��
		 */
		string ExecuteGetRequest(CString strUrl, int timeOut);

		/**
		 * @brief				-- Http Post ����
		 * @param strUrl		-- input,�����url��ַ
		 * @param m_strPostData	-- Post������
		 * @param head	-- ͷ��
		 * @param timeOut	-- ���շ��������ݵĳ�ʱʱ��
		 * @return				--- ����Ľ��
		 */
		string ExecutePostRequest(CString strUrl, CString m_strPostData, list<string> head, int timeOut);

		/**
		 * @brief				-- recycle CHttpConnection* and CHttpFile*
		 */
		void Clear();

	private:
		CInternetSession*		m_pSession;
		CHttpConnection*		m_pConnection;
		CHttpFile*				m_pFile;
	};
}
#endif