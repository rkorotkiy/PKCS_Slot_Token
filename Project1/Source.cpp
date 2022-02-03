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

	void OpenSession(CK_SLOT_ID slotID, CK_SESSION_HANDLE h_session);
	void CloseSession(CK_SESSION_HANDLE h_session);

	

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

void CryptoProvider::OpenSession(CK_SLOT_ID slotID, CK_SESSION_HANDLE h_session) {
	CK_RV rv;
	CK_BYTE application = 1;
	
	rv = m_funcList->C_OpenSession(slotID, CKF_SERIAL_SESSION | CKF_RW_SESSION, (CK_VOID_PTR)&application, NULL_PTR, &h_session);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::CloseSession(CK_SESSION_HANDLE h_session) {
	CK_RV rv;

	rv = m_funcList->C_CloseSession(h_session);

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



class Object {
private:
	CryptoProvider* m_provider;

public:
	Object(CryptoProvider* provider) : m_provider(provider) { };
	void CreateObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_object, 
		CK_OBJECT_CLASS objClass, std::vector<CK_ATTRIBUTE> objTemplate);
	void DestroyObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_object);
};



void Object::CreateObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_object, 
	CK_OBJECT_CLASS objClass, std::vector<CK_ATTRIBUTE> objTemplate) {

	CK_RV rv;
	rv = m_provider->GetFuncListPtr()->C_CreateObject(h_session, &objTemplate[0], objTemplate.size(), &h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void Object::DestroyObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_object) {
	CK_RV rv;

	rv = m_provider->GetFuncListPtr()->C_DestroyObject(h_session, h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}


class key_AES {
private:
	CK_MECHANISM m_mechanism = {
	  CKM_AES_KEY_GEN, NULL_PTR, 0
	};
	CryptoProvider* m_provider;

public:
	key_AES(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_key, CryptoProvider* provider, 
		std::vector<CK_ATTRIBUTE> objTemplate) : m_provider(provider) {
		CK_RV rv;
		rv = m_provider->GetFuncListPtr()->C_GenerateKey(h_session, &m_mechanism, &objTemplate[0], objTemplate.size(), &h_key);

		if (rv != CKR_OK)
			throw RetVal(rv);

	}

};



int main() {

	CryptoProvider provider(L"D:\\SoftHSM2\\lib\\softhsm2-x64.dll");

	provider.Initialize();

	std::vector<CK_SLOT_ID> slotCollection;
	provider.GetSlotCollection(true, slotCollection);

	std::vector<Token*> tokenCollection;
	for (size_t i = 0; i < slotCollection.size(); ++i) {
		Slot slot(slotCollection[i], &provider);
		Token token(&provider, slot.GetTokenInfo());
		tokenCollection.push_back(&token);
	}

	provider.Finalize();

}