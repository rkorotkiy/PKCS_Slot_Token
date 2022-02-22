#pragma once
#include "pkcs11.h"
#include "CryptoProvider.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

class Session {
private:
	CK_SESSION_HANDLE h_session;
	Slot* m_slot;
public:
	Session(CK_SESSION_HANDLE h_Session, Slot* m_Slot) : h_session(h_Session), m_slot(m_Slot) { }
	CK_SESSION_HANDLE GetHandle();
	
	void Login(CK_USER_TYPE userType);
	void Logout();

	void InitPin();

	void Close();
};