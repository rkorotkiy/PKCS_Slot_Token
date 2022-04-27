#ifndef BASICKEY_H
#define BASICKEY_H

#include "pkcs11.h"
#include "PKCSExceptions.h"
#include "CryptoProvider.h"
#include "Session.h"

class BasicKey {                   // абстрактная виртуальная функция generate
protected:
	Session* m_session;
	CryptoProvider* m_provider;
public:
	BasicKey(Session* m_Session, CryptoProvider* m_Provider) : m_session(m_Session), m_provider(m_Provider) { }
	Session* GetSession() { return m_session; }
	CryptoProvider* GetProvider() { return m_provider; };
};

#endif