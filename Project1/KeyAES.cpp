#include "classes.h"

void KeyAES::Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle, std::string label) {

	CK_RV rv;

	memcpy(keyLabel, label.c_str(), label.size() + 1);

	keyValueLen = valueLen;

	rv = m_provider->GetFuncListPtr()->C_GenerateKey(m_session->GetHandle(), &m_mechanism, AESTemplate, 7, &handle);
	
}