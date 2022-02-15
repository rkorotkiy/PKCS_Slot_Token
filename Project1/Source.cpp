#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>
#include "pkcs11.h"
#include "tdef.h"
#include "PKCSExceptions.h"



class CryptoProvider {
private:
	HINSTANCE m_lib;
	CK_FUNCTION_LIST* m_funcList = NULL_PTR;

	void* LoadProc(HINSTANCE, const char*);
public:
	CryptoProvider(const wchar_t*);
	~CryptoProvider();

	void SetFunctionList();
	void GetSlotCollection(CK_BBOOL tokenPresent, std::vector<CK_SLOT_ID>& slotCollection);
	void Initialize(void*);  //   ???
	void Finalize();

	void InitPin(CK_SESSION_HANDLE& h_session, CK_UTF8CHAR pin);

	void OpenSession(CK_SLOT_ID slotID, CK_SESSION_HANDLE& h_session, CK_BYTE application);
	void CloseSession(CK_SESSION_HANDLE& h_session);

	void CreateObject(CK_SESSION_HANDLE& h_session, CK_OBJECT_HANDLE& h_object, std::vector<CK_ATTRIBUTE>& objTemplate);
	void DestroyObject(CK_SESSION_HANDLE& h_session, CK_OBJECT_HANDLE& h_object);
	
	CK_FUNCTION_LIST_PTR GetFuncListPtr() {
		return m_funcList;
	}
};

CryptoProvider::CryptoProvider(const wchar_t* PATH_TO_DLL) {
	m_lib = LoadLibrary(PATH_TO_DLL);
}

void* CryptoProvider::LoadProc(HINSTANCE hLib, const char* FUNC_NAME) {
	return (void*)GetProcAddress(hLib, FUNC_NAME);
}

void CryptoProvider::SetFunctionList() {

	if (m_lib == NULL)
		throw  LibLoadErr();

	void* func = LoadProc(m_lib, "C_GetFunctionList");

	if (func == NULL)
		throw FuncLoadErr();

	int (*C_GetFuncList)(CK_FUNCTION_LIST**);
	C_GetFuncList = (C_GetFunctionList_decl)func;

	CK_RV rv = C_GetFuncList(&m_funcList);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::Initialize(void* initArgs = NULL_PTR) {
	SetFunctionList();

	if (m_funcList == NULL)
		throw FuncListErr();

	CK_RV rv = m_funcList->C_Initialize(initArgs); //TODO: Check 

	if (rv != CKR_OK)
		throw RetVal(rv);

}

void CryptoProvider::Finalize() {
	if (m_funcList == NULL)
		throw FuncListErr();

	CK_RV rv = m_funcList->C_Finalize(NULL_PTR);

	if (rv != CKR_OK)
		throw RetVal(rv);

}

void CryptoProvider::GetSlotCollection(CK_BBOOL tokenPresent, std::vector<CK_SLOT_ID>& slotCollection) {

	CK_ULONG pulCount;
	CK_RV rv = m_funcList->C_GetSlotList(tokenPresent, NULL_PTR, &pulCount);

	if (rv != CKR_OK)
		throw RetVal(rv);

	if (pulCount <= 0)
		return;

	slotCollection.resize(pulCount);
	rv = m_funcList->C_GetSlotList(tokenPresent, &slotCollection[0], &pulCount);

	if (rv != CKR_OK)
		throw RetVal(rv);

}

void CryptoProvider::OpenSession(CK_SLOT_ID slotID, CK_SESSION_HANDLE& h_session, CK_BYTE application) {
	CK_RV rv;
	
	rv = m_funcList->C_OpenSession(slotID, CKF_SERIAL_SESSION | CKF_RW_SESSION, (CK_VOID_PTR)&application, NULL_PTR, &h_session);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::CloseSession(CK_SESSION_HANDLE& h_session) {
	CK_RV rv;

	rv = m_funcList->C_CloseSession(h_session);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::CreateObject(CK_SESSION_HANDLE& h_session, CK_OBJECT_HANDLE& h_object, std::vector<CK_ATTRIBUTE>& objTemplate)
{
	CK_RV rv;
	rv = GetFuncListPtr()->C_CreateObject(h_session, &objTemplate[0], objTemplate.size(), &h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::DestroyObject(CK_SESSION_HANDLE& h_session, CK_OBJECT_HANDLE& h_object) {
	CK_RV rv;

	rv = GetFuncListPtr()->C_DestroyObject(h_session, h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::InitPin(CK_SESSION_HANDLE& h_session, CK_UTF8CHAR pin) {
	CK_RV rv;

	rv = GetFuncListPtr()->C_InitPIN(h_session, &pin, sizeof(pin) - 1);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

CryptoProvider::~CryptoProvider() {
	if (m_lib != NULL) {
		FreeLibrary(m_lib);
	}
}

void PrintSlots(std::vector<CK_SLOT_ID>& slotCollection) {
	for (size_t i = 0; i < slotCollection.size(); ++i) {
		std::cout << slotCollection[i] << std::endl;
	}
}



class Slot{
private:
	CK_SLOT_ID m_id;
	CryptoProvider* m_provider;
public:
	Slot(CK_SLOT_ID id, CryptoProvider* provider) : m_id(id), m_provider(provider) { }

	CK_TOKEN_INFO* GetTokenInfo() {
		CK_TOKEN_INFO* info = new CK_TOKEN_INFO();
		CK_RV rv;

		rv = m_provider->GetFuncListPtr()->C_GetTokenInfo(m_id, info);

		if (rv != CKR_OK)
			throw RetVal(rv);
		return info;
	}
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

	CryptoProvider* GetProviderPtr() {
		return m_provider;
	}

	CK_TOKEN_INFO* GetInfo() {
		return m_info;
	}

	unsigned char* GetLabel() {
		return m_label;
	}

};



class key_AES {
private:
	CK_MECHANISM m_mechanism = {
	  CKM_AES_KEY_GEN, NULL_PTR, 0
	};
	CryptoProvider* m_provider;

public:
	key_AES(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_key, CryptoProvider* provider, 
		std::vector<CK_ATTRIBUTE> objTemplate) : m_provider(provider) 
	{
		CK_RV rv;
		rv = m_provider->GetFuncListPtr()->C_GenerateKey(h_session, &m_mechanism, &objTemplate[0], objTemplate.size(), &h_key);

		if (rv != CKR_OK)
			throw RetVal(rv);

	}

};



int main() {

	CryptoProvider provider(L"D:\\SoftHSM2\\lib\\softhsm2-x64.dll");

	CK_BYTE app = 1;

	CK_C_INITIALIZE_ARGS initArgs;

	try {
		provider.Initialize();
	}
	catch (LibLoadErr LibEx) {
		std::cout << LibEx.what();
		return LibEx.errcode();
	}
	catch (FuncListErr FuncListEx) {
		std::cout << FuncListEx.what();
		return FuncListEx.errcode();
	}
	catch (FuncLoadErr FuncLoadEx) {
		std::cout << FuncLoadEx.what();
		return FuncLoadEx.errcode();
	}
	catch (RetVal RetEx) {
		std::cout << "Initialize\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	std::vector<CK_SLOT_ID> slotCollection;
	try {
		provider.GetSlotCollection(true, slotCollection);
	}
	catch (RetVal RetEx) {
		std::cout << "GetSlotCollection\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}
	std::vector<Token*> tokenCollection;
	for (size_t i = 0; i < slotCollection.size(); ++i) {
		Slot slot(slotCollection[i], &provider);
		Token token(&provider, slot.GetTokenInfo());
		tokenCollection.push_back(&token);
	}

	PrintSlots(slotCollection);

	CK_SESSION_HANDLE h_Session;

	try {
		provider.OpenSession(slotCollection[0], h_Session, app);
	}
	catch (RetVal RetEx) {
		std::cout << "OpenSession\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	CK_UTF8CHAR PIN[] = { "1234" };

	provider.InitPin(h_Session, *PIN);


	CK_OBJECT_HANDLE h_Key;

	CK_OBJECT_CLASS KeyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE KeyType = CKK_AES;
	CK_BBOOL True = CK_TRUE;
	
	std::vector<CK_ATTRIBUTE> keyTemplate
	{
		{CKA_CLASS, &KeyClass, sizeof(KeyClass)},
		{CKA_KEY_TYPE, &KeyType, sizeof(KeyType)},
		{CKA_WRAP, &True, sizeof(True)},
		{CKA_UNWRAP, &True, sizeof(True)},
		{CKA_ENCRYPT, &True, sizeof(True)},
		{CKA_DECRYPT, &True, sizeof(True)},
	};

	try {
		provider.CreateObject(h_Session, h_Key, keyTemplate);
	}
	catch (RetVal RetEx) {
		std::cout << "CreateObject\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		provider.DestroyObject(h_Session, h_Key);
	}
	catch (RetVal RetEx) {
		std::cout << "DestroyObject\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		provider.CloseSession(h_Session);
	}
	catch (RetVal RetEx) {
		std::cout << "CloseSession\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		provider.Finalize();
	}
	catch (RetVal RetEx) {
		std::cout << "Finalize\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}
}