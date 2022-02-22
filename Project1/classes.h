#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>
#include "tdef.h"
#include "PKCSExceptions.h"
#include "pkcs11.h"

class CryptoProvider;
class Slot;
class Token;
class Session;

class CryptoProvider {
private:
	HINSTANCE m_lib;
	CK_FUNCTION_LIST* m_funcList;

	void* LoadProc(HINSTANCE, const char*);
public:
	CryptoProvider(const wchar_t*);
	~CryptoProvider();

	void SetFunctionList();
	void GetSlotCollection(CK_BBOOL tokenPresent, std::vector<Slot*>& slotStorage);
	void Initialize();  //   ???
	void Finalize();

	void CreateObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object, std::vector<CK_ATTRIBUTE>& objTemplate);
	void DestroyObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object);

	CK_FUNCTION_LIST* GetFuncListPtr();
};



class Slot {
private:
	CK_SLOT_ID m_id;
	CryptoProvider* m_provider;
public:
	Slot(CK_SLOT_ID id, CryptoProvider* provider) : m_id(id), m_provider(provider) { }

	Session* OpenSession(CK_BYTE application);

	CK_TOKEN_INFO* GetTokenInfo();
	void InitToken();

	CK_FUNCTION_LIST* GetFuncListPtr();
	CK_SLOT_ID* GetSlotId();
};



class Token {
private:
	CryptoProvider* m_provider;
	CK_TOKEN_INFO* m_info;
	unsigned char* m_label;
public:

	Token(CryptoProvider* provider, CK_TOKEN_INFO* info) {
		m_provider = provider;
		m_info = info;
		m_label = info->label;
	}

	CryptoProvider* GetProviderPtr();
	CK_TOKEN_INFO* GetInfo();
	unsigned char* GetLabel();
};



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


















































//class key_AES {
//private:
//	CK_MECHANISM m_mechanism = {          //class key -> наследование key aes
//	  CKM_AES_KEY_GEN, NULL_PTR, 0
//	};
//	CryptoProvider* m_provider;
//
//public:
//	key_AES(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_key, CryptoProvider* provider,
//		std::vector<CK_ATTRIBUTE> objTemplate) : m_provider(provider)
//	{
//		CK_RV rv;
//		rv = m_provider->GetFuncListPtr()->C_GenerateKey(h_session, &m_mechanism, &objTemplate[0], objTemplate.size(), &h_key);
//
//		if (rv != CKR_OK)
//			throw RetVal(rv);
//
//	}
//
//};