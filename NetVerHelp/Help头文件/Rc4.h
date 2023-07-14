#pragma once
#ifndef _ENCRYPT_RC4_
#define _ENCRYPT_RC4_
#define BOX_LEN 256
namespace Help
{
	class Rc4
	{
	public:
		// ���ܣ����ؼ��ܽ��
		char* Encrypt(const char* szSource, const char* szPassWord); 
		// ���ܣ����ؽ��ܽ��
		char* Decrypt(const char* szSource, const char* szPassWord); 
	private:
		int GetKey(const unsigned char* pass, int pass_len, unsigned char *out);
		int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len);
		static void swap_byte(unsigned char* a, unsigned char* b);
		char* ByteToHex(const unsigned char* vByte, const int vLen); // ���ֽ���pbBufferתΪʮ�������ַ��������㴫��
		unsigned char* HexToByte(const char* szHex); // ��ʮ�������ַ���תΪ�ֽ���pbBuffer������
	};
}
#endif // #ifndef _ENCRYPT_RC4_