#include "classes.h"

void KeyAES::Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle) {

	CK_RV rv;

	std::string LABEL;
	//CK_UTF8CHAR LABELBuff[32];

	std::cout << "¬ведите Label: ";
	std::cin >> LABEL;
	memcpy(aes_keyLabel, LABEL.c_str(), LABEL.size() + 1);

	aes_keyValueLen = valueLen;

	rv = m_provider->GetFuncListPtr()->C_GenerateKey(m_session->GetHandle(), &m_mechanism, AESTemplate, 7, &handle);
	
}