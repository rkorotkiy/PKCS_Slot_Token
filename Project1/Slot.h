#pragma once
#include "CryptoProvider.h"
#include "Session.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

class Slot {
private:
	CK_SLOT_ID m_id;
	CryptoProvider* m_provider;
public:
	Slot(CK_SLOT_ID id, CryptoProvider* provider) : m_id(id), m_provider(provider) { }

	Session* OpenSession(CK_BYTE application);

	CK_TOKEN_INFO* GetTokenInfo();
	void InitToken();

	CK_FUNCTION_LIST* GetFuncListPtr();
	CK_SLOT_ID* GetSlotId();
};