#pragma once
#ifndef BusinessType_H_
#define BusinessType_H_
namespace Model
{
	//ҵ������
	enum BusinessType
	{
		//�����ʼ��
		iniSoftInfo = 1,
		//�˺�ע��
		accountRegister = 2,
		//�˺ŵ�¼
		accountLogin = 3,
		//���ܵ�¼
		cardLogin = 4,
		//����
		heartBeat = 5,
		//�۵�
		bucklePoint = 6,
		//�˳���¼
		loginOut = 7,
		//��ȡ�������
		getremoteVar = 8,
		//��������
		unbundMac = 9,
		//�޸��û�����
		updPwd = 10,

		//���ɿ���
		createCard = 11,

		// ��ͨ/���ѿ���
		openRenewCardNum = 12,

		//��ͨ/�����˺�
		openRenewAccount = 13,

		//��������
		cardDetail = 14,

		//�˺�����
		accountDetail = 15,

		//��ȡ����۸��б�
		softPriceList = 16,

		// ������
		buyCardNum = 17,

		//�޸Ŀ���/�˺ű�ע
		updRemark = 18,

		//��ֵ����ֵ
		rechCardRenewCardOrAccount = 19,

		//��ֵ������
		rechCardNumDetail = 20,

		//Զ���㷨ת��
		remoteCalculate = 21,

		//���û���ɾ����������˺�
		disableCardOrAccount = 22,
	};
}
#endif