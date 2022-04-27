//#include "classes.h"
#include "Slot.h"

Session* Slot::OpenSession(CK_BYTE application) {
	CK_RV rv;
	CK_SESSION_HANDLE h_session;

	rv = m_provider->GetFuncListPtr()->C_OpenSession(m_id, CKF_SERIAL_SESSION | CKF_RW_SESSION, (CK_VOID_PTR)&application, NULL_PTR, &h_session);
	if (rv != CKR_OK)
		throw RetVal(rv);

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

void Slot::InitToken(unsigned char* pin, unsigned char* label) {
	CK_RV rv;

	
	/*CK_UTF8CHAR PINBuff[32];
	CK_UTF8CHAR labelBuff[32];

	memset(labelBuff, ' ', sizeof(labelBuff));
	memcpy(labelBuff, label.c_str(), label.size() + 1);
	memcpy(PINBuff, PIN.c_str(), PIN.size() + 1);*/

	CK_UTF8CHAR labelBuff[32];
	memset(labelBuff, ' ', sizeof(labelBuff));
	memcpy(labelBuff, label, sizeof(label));

	rv = m_provider->GetFuncListPtr()->C_InitToken(m_id, pin, sizeof(pin), labelBuff);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

CK_FUNCTION_LIST* Slot::GetFuncListPtr() {
	return m_provider->GetFuncListPtr();
}

CK_SLOT_ID* Slot::GetSlotId() {
	return &m_id;
}