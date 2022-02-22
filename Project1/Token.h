#pragma once
#include "pkcs11.h"
#include "CryptoProvider.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

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

	CryptoProvider* GetProviderPtr() {
		return m_provider;
	}

	CK_TOKEN_INFO* GetInfo() {
		return m_info;
	}

	unsigned char* GetLabel() {
		return m_label;
	}

};