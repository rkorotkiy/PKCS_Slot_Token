//#include "classes.h"
#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "Token.h"
#include "CryptoProvider.h"

CryptoProvider* Token::GetProviderPtr() {
	return m_provider;
}

CK_TOKEN_INFO* Token::GetInfo() {
	return m_info;
}

unsigned char* Token::GetLabel() {
	return m_label;
}