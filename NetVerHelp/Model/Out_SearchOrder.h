#pragma once
#ifndef Out_SearchOrder_H_
#define Out_SearchOrder_H_
#include <string>
#include "PayType.h"
#include "OrderState.h"
#include "BusinessCompleteType.h"
#include "Out_DataBaseArgs.h"
using std::string;
using namespace std;
namespace Model
{
	//������ѯ����
	class Out_SearchOrder :public Out_DataBaseArgs
	{

	public:

		// ����״̬ 0:�ȴ�֧��  1:֧���ɹ� 2:֧��δ��� 3:֧��ʧ��
		// ��ע�⣺֧���ɹ���˵���ն��û��ѳɹ�������ˡ�
		OrderState orderstate;

		// ����ҵ�����״̬ 0:�ȴ���� 1:�ɹ�  2:ʧ��
		// ��ע�⣺�������״̬��֧���ɹ�����ҵ�����״̬�ǳɹ�����Ҫ���嵽��̨�ҵ��˶����鿴δ�ɹ���ԭ��
		BusinessCompleteType businesscompletetype;

		// ֧������ 0:΢�� 1��֧����
		PayType paytypeid;

		// �ۼ�
		double price;

		// ʵ�ս��
		double revicemoney;

		// ���� 
		// ��ע�⣺����˶��������ڡ�������֤ƽ̨�������ģ������ǹ����ܣ���ô֧��״̬�Ͷ���ҵ�����״̬���ɹ��󣬻�ѹ���Ŀ��ܷ�������
		string cardnum;

		// ��Ʒ���ص����̵�ַ
		// ������˶��������ڡ��Զ�����ƽ̨�������ģ���ô֧��״̬�Ͷ���ҵ�����״̬���ɹ��󣬻����Ʒ�����ص�ַ����ȡ�뷵������
		string goodsnetworkdiskurl;
		// ������ȡ��
		string diskpwd;

		//�����롾����˶��������ڡ��Զ�����ƽ̨�������ģ���ô�˶�������̨�豸�����ɵĻ�����᷵������
		string maccode;

		//������ʶ���˶����Ƿ񷢻���һ�����
		//������˶��������ڡ��Զ�����ƽ̨�������ģ���ô�˶�����ʶ�᷵������
		string orderflag;

		//����������ʱ��
		string createtime;
			 
	};
}
#endif