#ifndef SESSION_H
#define SESSION_H

#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "Slot.h"

class Session {
private:
	CK_SESSION_HANDLE h_session;
	Slot* m_slot;
public:
	Session(CK_SESSION_HANDLE h_Session, Slot* m_Slot) : h_session(h_Session), m_slot(m_Slot) { }
	CK_SESSION_HANDLE GetHandle();

	void Login(CK_USER_TYPE userType, unsigned char* PIN);
	void Logout();

	void InitPin(unsigned char* PIN);

	void Close();
};

#endif