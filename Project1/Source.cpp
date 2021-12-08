#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>
#include "pkcs11.h"
#include "tdef.h"
#include "PKCSExceptions.h"



class CryptoToken {
private:
	HINSTANCE hLib;
	void* LoadFunc = NULL_PTR;
	void LoadProc(HINSTANCE, const char*);
	CK_FUNCTION_LIST_PTR FuncList = NULL_PTR;
	CK_FLAGS Flags = 0;
	CK_VOID_PTR pReserved = NULL_PTR;
	std::vector<Slot*> SlotCollection;
public:
	CryptoToken(const wchar_t*);
	~CryptoToken();
	int GetFunctionList();
	int Initialize();
	int Initialize(CK_C_INITIALIZE_ARGS);
	int Finalize();

	CK_FUNCTION_LIST_PTR GetFuncListPtr() {
		return FuncList;
	}

	std::vector<Slot*> GetSlotColPtr() {
		return this->SlotCollection;
	}

	void PrintSlots() {
		for (int i = 0; i < SlotCollection.size(); i++) {
			std::cout << SlotCollection[i] << std::endl;
		}
	}

};

class Slot{

private:
	static CK_SLOT_ID_PTR SlotList;
	static CK_ULONG ListCount;
	CK_SLOT_INFO SlotInfo;
public:
	int GetSlotList(CK_BBOOL, CryptoToken);
	int GetSlotInfo(unsigned int, CryptoToken CrTok);

	void PushToSlotCollection(CryptoToken Cr) {
		Cr.GetSlotColPtr().push_back(this);
	}

	CK_SLOT_ID_PTR GetSlotListPtr() {
		return SlotList;
	}

};

class Token{
private:
	CK_TOKEN_INFO TokenInfo;
public:
	int GetTokenInfo(unsigned int, CryptoToken, Slot);
};



/******************************************************************************************************************************************/



CryptoToken::CryptoToken(const wchar_t* PATH_TO_DLL) {
	hLib = LoadLibrary(PATH_TO_DLL);
}

void CryptoToken::LoadProc(HINSTANCE hLib, const char* FUNC_NAME) {
	LoadFunc = (void*)GetProcAddress(hLib, FUNC_NAME);
}

int CryptoToken::GetFunctionList() {
	if (hLib == NULL) throw  LibLoadErr();
	LoadProc(hLib, "C_GetFunctionList");
	if (LoadFunc == NULL) throw FuncLoadErr();
	int (*C_GetFuncList)(CK_FUNCTION_LIST_PTR_PTR);
	C_GetFuncList = (C_GetFunctionList_decl)LoadFunc;
	CK_RV rv = C_GetFuncList(&FuncList);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int CryptoToken::Initialize() {
	if (FuncList == NULL) throw FuncListErr();
	CK_C_Initialize pC_Initialize = FuncList->C_Initialize;
	CK_RV rv = pC_Initialize(NULL_PTR);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int CryptoToken::Initialize(CK_C_INITIALIZE_ARGS InitArgs) {
	if (FuncList == NULL) throw FuncListErr();
	CK_C_Initialize pC_Initialize = FuncList->C_Initialize;
	CK_RV rv = pC_Initialize((CK_VOID_PTR)&InitArgs);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int CryptoToken::Finalize() {
	if (FuncList == NULL) throw FuncListErr();
	CK_C_Finalize pC_Finalize = FuncList->C_Finalize;
	CK_RV rv = pC_Finalize(NULL_PTR);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

CryptoToken::~CryptoToken() {
	/*if (SlotList != NULL_PTR) {
		delete SlotList;
		SlotList = NULL_PTR;
	}
	if (hLib != NULL) {
		FreeLibrary(hLib);
	}*/
}

/******************************************************************************************************************************************/

int Slot::GetSlotList(CK_BBOOL token_present, CryptoToken CrTok) {
	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
	CK_C_GetSlotList pC_GetSlotList = CrTok.GetFuncListPtr()->C_GetSlotList;
	CK_RV rv = pC_GetSlotList(token_present, NULL_PTR, &ListCount);
	if (rv != CKR_OK) throw RetVal(rv);
	SlotList = new CK_SLOT_ID;
	rv = pC_GetSlotList(token_present, SlotList, &ListCount);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int Slot::GetSlotInfo(unsigned int slot, CryptoToken CrTok) {
	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
	if (slot == 0) throw PKCSExceptions();
	CK_C_GetSlotInfo pC_GetSlotInfo = CrTok.GetFuncListPtr()->C_GetSlotInfo;
	CK_RV rv = pC_GetSlotInfo(SlotList[slot - 1], &SlotInfo);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

/******************************************************************************************************************************************/

int Token::GetTokenInfo(unsigned int slot, CryptoToken CrTok, Slot Sl) {
	if (CrTok.GetFuncListPtr() == NULL) throw FuncListErr();
	if (slot == 0) throw PKCSExceptions();
	CK_C_GetTokenInfo pC_GetTokenInfo = CrTok.GetFuncListPtr()->C_GetTokenInfo;
	CK_RV rv = pC_GetTokenInfo(Sl.GetSlotListPtr()[slot], &TokenInfo);
	if (rv != CKR_OK)throw RetVal(rv);
	return rv;
}

/******************************************************************************************************************************************/

int main() {




}