#include "classes.h"


void KeyAES::Generate(CK_UTF8CHAR* label, CK_BYTE value, CK_ULONG valueLen, CK_OBJECT_HANDLE handle) {

	CK_RV rv;

	rv = m_provider->GetFuncListPtr()->C_GenerateKey(m_session->GetHandle(), &m_mechanism, AESTemplate, 7, &handle);
	
}