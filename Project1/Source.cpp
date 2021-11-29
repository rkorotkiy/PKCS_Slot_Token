#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include "pkcs11.h"
#include "tdef.h"
#include "PKCSExceptions.h"
#include <string>

class CryptoToken {
private:
	HINSTANCE hLib;
	void* LoadFunc = NULL_PTR;
protected:
	CryptoToken(const wchar_t*);
	void LoadProc(HINSTANCE, const char*);
	CK_FUNCTION_LIST_PTR FuncList = NULL_PTR;
	CK_FLAGS Flags = 0;
	CK_VOID_PTR pReserved;
	CK_SLOT_ID_PTR SlotList = NULL_PTR; /*Не совсем уверен, что стоит засовывать это поле в базовый класс,
										но не знаю как по нормальному реализовать доступ к нему из метода GetTokenInfo класса Token :/ 
										*/
public:
	int GetFunctionList();
	int Initialize();
	int Initialize(CK_C_INITIALIZE_ARGS);
	int Finalize();
};

class Slot : public CryptoToken {
private:
	
	CK_ULONG ListCount;
	CK_SLOT_INFO SlotInfo;
public:
	int GetSlotList(CK_BBOOL);
	int GetSlotInfo(unsigned int);
	int WaitForSlotEvent();
};

class Token : public CryptoToken {
private:
	CK_TOKEN_INFO TokenInfo;
public:
	int GetTokenInfo(unsigned int);
};

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
	if (SlotList != NULL_PTR) {
		delete SlotList;
		SlotList = NULL_PTR;
	}
	if (hLib != NULL) {
		FreeLibrary(hLib);
	}
}

int Slot::GetSlotList(CK_BBOOL token_present) {
	if (FuncList == NULL) throw FuncListErr();
	CK_C_GetSlotList pC_GetSlotList = FuncList->C_GetSlotList;
	CK_RV rv = pC_GetSlotList(token_present, NULL_PTR, &ListCount);
	if (rv != CKR_OK) throw RetVal(rv);
	SlotList = new CK_SLOT_ID;
	rv = pC_GetSlotList(token_present, SlotList, &ListCount);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int Slot::GetSlotInfo(unsigned int slot) {
	if (FuncList == NULL) throw FuncListErr();
	if (slot == 0) throw PKCSExceptions();
	CK_C_GetSlotInfo pC_GetSlotInfo = FuncList->C_GetSlotInfo;
	CK_RV rv = pC_GetSlotInfo(SlotList[slot - 1], &SlotInfo);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int Slot::WaitForSlotEvent() {
	if (FuncList == NULL) throw FuncListErr();
	CK_C_WaitForSlotEvent pC_WaitForSlotEvent = FuncList->C_WaitForSlotEvent;
	CK_RV rv = pC_WaitForSlotEvent(Flags, SlotList, pReserved);
	if (rv != CKR_OK) throw RetVal(rv);
	return rv;
}

int Token::GetTokenInfo(unsigned int slot) {
	if (FuncList == NULL) throw FuncListErr();
	if (slot == 0) throw PKCSExceptions();
	CK_C_GetTokenInfo pC_GetTokenInfo = FuncList->C_GetTokenInfo;
	CK_RV rv = pC_GetTokenInfo(SlotList[slot - 1], &TokenInfo);
	if (rv != CKR_OK)throw RetVal(rv);
	return rv;
}