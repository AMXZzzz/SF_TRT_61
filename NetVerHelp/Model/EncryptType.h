#pragma once
#ifndef EncryptType_H_
#define EncryptType_H_
namespace Model
{
	//��������
	enum EncryptType
	{
		// �޼���
		NoEncrypt = 0,
		//// DES����(c#���ԣ������ô˼��ܷ�ʽ)
		//DES = 1,
		///// <summary>
		///// AES����
		///// </summary>
		//AES = 2,
		// RC4����(c++���ԣ������ô˼��ܷ�ʽ)
		RC4 = 3,
	};
}
#endif