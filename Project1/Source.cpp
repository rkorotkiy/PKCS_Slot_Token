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
		m_provider->GetFuncListPtr()->C_GetTokenInfo(m_id, info);
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

















































//class CryptoToken {
//private:
//	HINSTANCE m_Lib;
//	void* LoadFunc = NULL_PTR;
//	void LoadProc(HINSTANCE, const char*);
//	CK_FUNCTION_LIST_PTR FuncList = NULL_PTR;
//	CK_FLAGS Flags = 0;
//	CK_VOID_PTR pReserved = NULL_PTR;
//	std::vector<Slot*> SlotCollection;
//public:
//	CryptoToken(const wchar_t*);
//	~CryptoToken();
//	int GetFunctionList();
//	int Initialize();
//	int Initialize(CK_C_INITIALIZE_ARGS);
//	int Finalize();
//
//	CK_FUNCTION_LIST_PTR GetFuncListPtr() {
//		return FuncList;
//	}
//
//	std::vector<Slot*> GetSlotColPtr() {
//		return this->SlotCollection;
//	}
//
//	void PrintSlots() {
//		for (int i = 0; i < SlotCollection.size(); i++) {
//			std::cout << SlotCollection[i] << std::endl;
//		}
//	}
//
//};
//
//class Slot{
//
//private:
//	static CK_SLOT_ID_PTR SlotList;
//	static CK_ULONG ListCount;
//	CK_SLOT_INFO SlotInfo;
//public:
//	int GetSlotList(CK_BBOOL, CryptoToken);
//	int GetSlotInfo(unsigned int, CryptoToken CrTok);
//
//	void PushToSlotCollection(CryptoToken Cr) {
//		Cr.GetSlotColPtr().push_back(this);
//	}
//
//	static CK_SLOT_ID_PTR GetSlotListPtr() {
//		return SlotList;
//	}
//
//};
//
//class Token{
//private:
//	CK_TOKEN_INFO TokenInfo;
//public:
//	int GetTokenInfo(unsigned int, CryptoToken, Slot);
//};
//
//
//
///******************************************************************************************************************************************/
//
//
//
//CryptoToken::CryptoToken(const wchar_t* PATH_TO_DLL) {
//	hLib = LoadLibrary(PATH_TO_DLL);
//}
//
//void CryptoToken::LoadProc(HINSTANCE hLib, const char* FUNC_NAME) {
//	LoadFunc = (void*)GetProcAddress(hLib, FUNC_NAME);
//}
//
//int CryptoToken::GetFunctionList() {
//	if (hLib == NULL) throw  LibLoadErr();
//	LoadProc(hLib, "C_GetFunctionList");
//	if (LoadFunc == NULL) throw FuncLoadErr();
//	int (*C_GetFuncList)(CK_FUNCTION_LIST_PTR_PTR);
//	C_GetFuncList = (C_GetFunctionList_decl)LoadFunc;
//	CK_RV rv = C_GetFuncList(&FuncList);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
//int CryptoToken::Initialize() {
//	if (FuncList == NULL) throw FuncListErr();
//	CK_C_Initialize pC_Initialize = FuncList->C_Initialize;
//	CK_RV rv = pC_Initialize(NULL_PTR);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
//int CryptoToken::Initialize(CK_C_INITIALIZE_ARGS InitArgs = NULL_PTR) {
//	if (FuncList == NULL) throw FuncListErr();
//	CK_C_Initialize pC_Initialize = FuncList->C_Initialize;
//	CK_RV rv = pC_Initialize((CK_VOID_PTR)&InitArgs);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
//int CryptoToken::Finalize() {
//	if (FuncList == NULL) throw FuncListErr();
//	CK_C_Finalize pC_Finalize = FuncList->C_Finalize;
//	CK_RV rv = pC_Finalize(NULL_PTR);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
//CryptoToken::~CryptoToken() {
//	/*if (SlotList != NULL_PTR) {
//		delete SlotList;
//		SlotList = NULL_PTR;
//	}
//	if (hLib != NULL) {
//		FreeLibrary(hLib);
//	}*/
//}
//
///******************************************************************************************************************************************/
//
//int Slot::GetSlotList(CK_BBOOL token_present, CryptoToken CrTok) {
//	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
//	CK_C_GetSlotList pC_GetSlotList = CrTok.GetFuncListPtr()->C_GetSlotList;
//	CK_RV rv = pC_GetSlotList(token_present, NULL_PTR, &ListCount);
//	if (rv != CKR_OK) throw RetVal(rv);
//	SlotList = new CK_SLOT_ID;
//	rv = pC_GetSlotList(token_present, SlotList, &ListCount);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
//int Slot::GetSlotInfo(unsigned int slot, CryptoToken CrTok) {
//	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
//	if (slot == 0) throw PKCSExceptions();
//	CK_C_GetSlotInfo pC_GetSlotInfo = CrTok.GetFuncListPtr()->C_GetSlotInfo;
//	CK_RV rv = pC_GetSlotInfo(SlotList[slot - 1], &SlotInfo);
//	if (rv != CKR_OK) throw RetVal(rv);
//	return rv;
//}
//
///******************************************************************************************************************************************/
//
//int Token::GetTokenInfo(unsigned int slot, CryptoToken CrTok, Slot Sl) {
//	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
//	if (slot == 0) throw PKCSExceptions();
//	CK_C_GetTokenInfo pC_GetTokenInfo = CrTok.GetFuncListPtr()->C_GetTokenInfo;
//	CK_RV rv = pC_GetTokenInfo(Sl.GetSlotListPtr()[slot], &TokenInfo);
//	if (rv != CKR_OK)throw RetVal(rv);
//	return rv;
//}
//
///******************************************************************************************************************************************/
//
//int main() {
//
//
//
//
//}











