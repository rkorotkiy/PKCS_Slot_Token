#ifndef KEYAES_H
#define KEYAES_H

#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "BasicKey.h"

class KeyAES : public BasicKey {
private:

	CK_ATTRIBUTE* GetTemplatePtr(CK_ULONG valueLen, unsigned char* label);

	CK_MECHANISM m_mechanism = {
		CKM_AES_KEY_GEN, NULL_PTR, 0
	};

public:
	KeyAES(BasicKey* bKey) : BasicKey(bKey->GetSession(), bKey->GetProvider()) {};
	void Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle, unsigned char* label);
};

#endif