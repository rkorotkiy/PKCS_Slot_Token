//#include "classes.h"
#include "KeyAES.h"

void KeyAES::Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle, unsigned char* label) {

	CK_RV rv;

	rv = m_provider->GetFuncListPtr()->C_GenerateKey(m_session->GetHandle(), &m_mechanism, GetTemplatePtr(valueLen, label), 7, &handle);
	
}

CK_ATTRIBUTE* KeyAES::GetTemplatePtr(CK_ULONG valueLen, unsigned char* label) {

	CK_OBJECT_CLASS objClass = CKO_SECRET_KEY;
	CK_KEY_TYPE keyType = CKK_AES;
	CK_BBOOL True = CK_TRUE;
	CK_UTF8CHAR* keyLabel;
	CK_ULONG keyValueLen;

	CK_ATTRIBUTE AESTemplate[6] = {          
		{CKA_CLASS, &objClass, sizeof(objClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &True, sizeof(True)},
		{CKA_LABEL, &keyLabel, sizeof(keyLabel)},
		{CKA_VALUE_LEN, &keyValueLen, sizeof(keyValueLen)},
		{CKA_ENCRYPT, &True, sizeof(true)}
	};

	//memcpy(keyLabel, label.c_str(), label.size() + 1);

	keyLabel = label;
	keyValueLen = valueLen;

	return AESTemplate;

}