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
	void InitToken(std::string PIN, std::string label);

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

	void Login(CK_USER_TYPE userType, std::string PIN);
	void Logout();

	void InitPin(std::string PIN);

	void Close();
};



class BasicKey {
protected:
	Session* m_session;
	CryptoProvider* m_provider;
public:
	BasicKey(Session* m_Session, CryptoProvider* m_Provider) : m_session(m_Session), m_provider(m_Provider) {

	}
	Session* GetSession() { return m_session; }
	CryptoProvider* GetProvider() { return m_provider; };
};



class KeyAES : public BasicKey{
private:
	
	//AES key attributes

	CK_OBJECT_CLASS objClass = CKO_SECRET_KEY;
	CK_KEY_TYPE keyType = CKK_AES;
	CK_BBOOL True = CK_TRUE;
	CK_UTF8CHAR keyLabel[32];
	CK_ULONG keyValueLen;

	CK_ATTRIBUTE AESTemplate[6] = {
		{CKA_CLASS, &objClass, sizeof(objClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &True, sizeof(True)},
		{CKA_LABEL, &keyLabel, sizeof(keyLabel)},
		{CKA_VALUE_LEN, &keyValueLen, sizeof(keyValueLen)},
		{CKA_ENCRYPT, &True, sizeof(true)}
	};

	/*------------------------------------------------------------------------*/

	CK_MECHANISM m_mechanism = {
		CKM_AES_KEY_GEN, NULL_PTR, 0
	};

public:
	KeyAES(BasicKey* bKey) : BasicKey(bKey->GetSession(), bKey->GetProvider()) {};
	void Generate(CK_ULONG valueLen, CK_OBJECT_HANDLE handle, std::string label);
};



class KeysRSA {
private:

	//RSA public key template

	CK_OBJECT_CLASS objClass = CKO_PUBLIC_KEY;
	CK_KEY_TYPE keyType = CKK_RSA;
	CK_UTF8CHAR label;
	CK_BYTE modulusBits;
	CK_BYTE exponent;
	CK_BBOOL True = CK_TRUE;
	CK_ATTRIBUTE RSAPublicKeyTemplate[8] = {
		{CKA_CLASS, &objClass, sizeof(objClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &True, sizeof(true)},
		{CKA_LABEL, &label, sizeof(label) - 1},
		{CKA_WRAP, &True, sizeof(true)},
		{CKA_ENCRYPT, &True, sizeof(true)},
		{CKA_MODULUS_BITS, &modulusBits, sizeof(modulusBits)},
		{CKA_PUBLIC_EXPONENT, &exponent, sizeof(exponent)}
	};

	/*------------------------------------------------------------------------*/

	//RSA private key template

	CK_OBJECT_CLASS objClass = CKO_PRIVATE_KEY;
	CK_KEY_TYPE keyType = CKK_RSA;
	CK_UTF8CHAR label;
	CK_BYTE subject;
	CK_BYTE id;
	//CK_BYTE modulus;
	//CK_BYTE publicExponent;
	//CK_BYTE privateExponent;
	//CK_BYTE prime1;
	//CK_BYTE prime2;
	//CK_BYTE exponent1;
	//CK_BYTE exponent2;
	//CK_BYTE coefficient;
	CK_BBOOL True = CK_TRUE;

	CK_ATTRIBUTE RSAPrivateKeyTemplate[9] = {

	{CKA_CLASS, &objClass, sizeof(objClass)},
	{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
	{CKA_TOKEN, &True, sizeof(true)},
	{CKA_LABEL, &label, sizeof(label) - 1},
	{CKA_SUBJECT, &subject, sizeof(subject)},
	{CKA_ID, &id, sizeof(id)},
	{CKA_SENSITIVE, &True, sizeof(true)},
	{CKA_DECRYPT, &True, sizeof(true)},
	{CKA_SIGN, &True, sizeof(true)},
	/*{CKA_MODULUS, modulus, sizeof(modulus)},*/
	/*{CKA_PUBLIC_EXPONENT, publicExponent, sizeof(publicExponent)},*/
	/*{CKA_PRIVATE_EXPONENT, privateExponent, sizeof(privateExponent)},*/
	/*{CKA_PRIME_1, prime1, sizeof(prime1)},*/
	/*{CKA_PRIME_2, prime2, sizeof(prime2)},*/
	/*{CKA_EXPONENT_1, exponent1, sizeof(exponent1)},*/
	/*{CKA_EXPONENT_2, exponent2, sizeof(exponent2)},*/
	/*{CKA_COEFFICIENT, coefficient, sizeof(coefficient)}*/
	};

	/*------------------------------------------------------------------------*/

public:

};