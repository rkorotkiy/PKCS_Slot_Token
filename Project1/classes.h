//#include <iostream>
//#include <stdlib.h>
//#include <Windows.h>
//#include <libloaderapi.h>
//#include <vector>
//#include "tdef.h"
//#include "PKCSExceptions.h"
//#include "pkcs11.h"
//
//class CryptoProvider;
//class Slot;
//class Token;
//class Session;
//
//class CryptoProvider {
//private:
//	HINSTANCE m_lib;
//	CK_FUNCTION_LIST* m_funcList;
//
//	void* LoadProc(HINSTANCE, const char*);
//public:
//	CryptoProvider(const wchar_t*);
//	~CryptoProvider();
//
//	void SetFunctionList();
//	void GetSlotCollection(CK_BBOOL tokenPresent, std::vector<Slot*>& slotStorage);
//	void Initialize();  //   ???
//	void Finalize();
//
//	void CreateObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object, std::vector<CK_ATTRIBUTE>& objTemplate);
//	void DestroyObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object);
//
//	CK_FUNCTION_LIST* GetFuncListPtr();
//};
//
//
//
//class Slot {
//private:
//	CK_SLOT_ID m_id;
//	CryptoProvider* m_provider;
//public:
//	Slot(CK_SLOT_ID id, CryptoProvider* provider) : m_id(id), m_provider(provider) { }
//
//	Session* OpenSession(CK_BYTE application);
//
//	CK_TOKEN_INFO* GetTokenInfo();
//	void InitToken(unsigned char*, unsigned char* label);
//
//	CK_FUNCTION_LIST* GetFuncListPtr();
//	CK_SLOT_ID* GetSlotId();
//};
//
//
//
//class Token {
//private:
//	CryptoProvider* m_provider;
//	CK_TOKEN_INFO* m_info;
//	unsigned char* m_label;
//public:
//
//	Token(CryptoProvider* provider, CK_TOKEN_INFO* info) {
//		m_provider = provider;
//		m_info = info;
//		m_label = info->label;
//	}
//
//	CryptoProvider* GetProviderPtr();
//	CK_TOKEN_INFO* GetInfo();
//	unsigned char* GetLabel();
//};
//
//
//
//class Session {
//private:
//	CK_SESSION_HANDLE h_session;
//	Slot* m_slot;
//public:
//	Session(CK_SESSION_HANDLE h_Session, Slot* m_Slot) : h_session(h_Session), m_slot(m_Slot) { }
//	CK_SESSION_HANDLE GetHandle();
//
//	void Login(CK_USER_TYPE userType, unsigned char* PIN);
//	void Logout();
//
//	void InitPin(unsigned char* PIN);
//
//	void Close();
//};
//
//
//
//class BasicKey {                   // абстрактная виртуальная функция generate
//protected:
//	Session* m_session;
//	CryptoProvider* m_provider;
//public:
//	BasicKey(Session* m_Session, CryptoProvider* m_Provider) : m_session(m_Session), m_provider(m_Provider) { }
//	Session* GetSession() { return m_session; }
//	CryptoProvider* GetProvider() { return m_provider; };
//};
//
//
//
//class KeyAES : public BasicKey {
//private:
//	
//	CK_ATTRIBUTE* GetTemplatePtr(CK_ULONG valueLen, unsigned char* label);
//
//	CK_MECHANISM m_mechanism = {
//		CKM_AES_KEY_GEN, NULL_PTR, 0
//	};
//
//public:
//	KeyAES(BasicKey* bKey) : BasicKey(bKey->GetSession(), bKey->GetProvider()) {};
//	void Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle, unsigned char* label);
//};
//
//
//
//class KeysRSA : public BasicKey {
//private:
//
//	CK_ATTRIBUTE* GetPubTemplatePtr(
//		unsigned char* public_label,
//		unsigned char* public_modulusBits,
//		unsigned char* public_exponent
//	);
//
//	CK_ATTRIBUTE* GetPrivTemplatePtr(
//		unsigned char* private_label,
//		unsigned char* private_subject,
//		unsigned char* private_id
//	);
//
//	CK_MECHANISM mechanism = {
//		CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0
//	};
//
//public:
//	KeysRSA(BasicKey* bKey) : BasicKey(bKey->GetSession(), bKey->GetProvider()) {};
//	void Generate(
//		unsigned char* public_label,
//		unsigned char* public_modulusBits,
//		unsigned char* public_exponent,
//		unsigned char* private_label,
//		unsigned char* private_subject,
//		unsigned char* private_id,
//		CK_OBJECT_HANDLE privateKeyHandle,
//		CK_OBJECT_HANDLE publicKeyHandle);
//};