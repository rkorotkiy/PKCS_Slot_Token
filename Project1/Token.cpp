//#include "classes.h"
#include "Token.h"

CryptoProvider* Token::GetProviderPtr() {
	return m_provider;
}

CK_TOKEN_INFO* Token::GetInfo() {
	return m_info;
}

unsigned char* Token::GetLabel() {
	return m_label;
}