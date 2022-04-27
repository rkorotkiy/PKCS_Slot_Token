#ifndef SLOT_H
#define SLOT_H

#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "CryptoProvider.h"
#include "Session.h"

class Slot {
private:
	CK_SLOT_ID m_id;
	CryptoProvider* m_provider;
public:
	Slot(CK_SLOT_ID id, CryptoProvider* provider) : m_id(id), m_provider(provider) { }

	Session* OpenSession(CK_BYTE application);

	CK_TOKEN_INFO* GetTokenInfo();
	void InitToken(unsigned char* pin, unsigned char* label);

	CK_FUNCTION_LIST* GetFuncListPtr();
	CK_SLOT_ID* GetSlotId();
};

#endif