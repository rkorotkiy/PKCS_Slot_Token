//#include "classes.h"
#include "Session.h"



void Session::Close() {
	CK_RV rv;

	rv = m_slot->GetFuncListPtr()->C_CloseSession(h_session);
	if (rv != CKR_OK)
		throw RetVal(rv);

}

CK_SESSION_HANDLE Session::GetHandle() { 
	return h_session; 
}

void Session::Login(CK_USER_TYPE userType, unsigned char* PIN) {
	CK_RV rv;

	//CK_UTF8CHAR PINBuff[32];
	//memcpy(PINBuff, PIN.c_str(), PIN.size() + 1);

	rv = m_slot->GetFuncListPtr()->C_Login(h_session, userType, PIN, sizeof(PIN));

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void Session::Logout() {
	CK_RV rv;

	rv = m_slot->GetFuncListPtr()->C_Logout(h_session);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void Session::InitPin(unsigned char* PIN) {
	
	CK_RV rv;
	
	/*CK_UTF8CHAR PINBuff[32];
	memcpy(PINBuff, PIN.c_str(), PIN.size() + 1);*/

	rv = m_slot->GetFuncListPtr()->C_InitPIN(h_session, PIN, sizeof(PIN));

	if (rv != CKR_OK)
		throw RetVal(rv);
}