#ifndef TOKEN_H
#define TOKEN_H

#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "CryptoProvider.h"

class Token {
private:
	CryptoProvider* m_provider;
	CK_TOKEN_INFO* m_info;
	unsigned char* m_label;
public:

	Token(CryptoProvider* provider, CK_TOKEN_INFO* info) {
		m_provider = provider;
		m_info = info;
		m_label = info->label;
	}

	CryptoProvider* GetProviderPtr();
	CK_TOKEN_INFO* GetInfo();
	unsigned char* GetLabel();
};

#endif