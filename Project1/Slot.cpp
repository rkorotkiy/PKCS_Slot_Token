#include "classes.h"

Session* Slot::OpenSession(CK_BYTE application) {
	CK_RV rv;
	CK_SESSION_HANDLE h_session;

	rv = m_provider->GetFuncListPtr()->C_OpenSession(m_id, CKF_SERIAL_SESSION | CKF_RW_SESSION, (CK_VOID_PTR)&application, NULL_PTR, &h_session);
	if (rv != CKR_OK)
		throw RetVal(rv);

	//Session* session(h_session, this);
	//return &session;

	Session* session = new Session(h_session, this);
	return session;
}

CK_TOKEN_INFO* Slot::GetTokenInfo() {
	CK_TOKEN_INFO* info = new CK_TOKEN_INFO();
	CK_RV rv;

	rv = m_provider->GetFuncListPtr()->C_GetTokenInfo(m_id, info);

	if (rv != CKR_OK)
		throw RetVal(rv);
	return info;
}

void Slot::InitToken() {
	CK_RV rv;

	std::string PIN;
	CK_UTF8CHAR PINBuff[32];
	std::string label;
	CK_UTF8CHAR labelBuff[32];
	std::cout << "Введите PIN Администратора: ";
	std::cin >> PIN;
	std::cout << "Введите значение label для токена: ";
	std::cin >> label;

	memset(labelBuff, ' ', sizeof(labelBuff));
	memcpy(labelBuff, label.c_str(), label.size() + 1);
	memcpy(PINBuff, PIN.c_str(), PIN.size() + 1);

	rv = m_provider->GetFuncListPtr()->C_InitToken(m_id, PINBuff, strlen(PIN.c_str()), labelBuff);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

CK_FUNCTION_LIST* Slot::GetFuncListPtr() {
	return m_provider->GetFuncListPtr();
}

CK_SLOT_ID* Slot::GetSlotId() {
	return &m_id;
}