//#include "classes.h"

#include "pkcs11.h"
#include "tdef.h"
#include "PKCSExceptions.h"
#include "CryptoProvider.h"
#include "Slot.h"
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

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

void CryptoProvider::Initialize() {
	SetFunctionList();

	if (m_funcList == NULL)
		throw FuncListErr();

	CK_RV rv = m_funcList->C_Initialize(NULL_PTR);

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

void CryptoProvider::GetSlotCollection(CK_BBOOL tokenPresent, std::vector<Slot*>& slotStorage) {

	CK_ULONG pulCount;
	CK_RV rv = m_funcList->C_GetSlotList(tokenPresent, NULL_PTR, &pulCount);

	if (rv != CKR_OK)
		throw RetVal(rv);

	if (pulCount <= 0)
		return;

	std::vector<CK_SLOT_ID> slotCollection;
	slotCollection.resize(pulCount);
	rv = m_funcList->C_GetSlotList(tokenPresent, &slotCollection[0], &pulCount);

	if (rv != CKR_OK)
		throw RetVal(rv);

	for (size_t i = 0; i < pulCount; ++i) {
		slotStorage.push_back(new Slot(slotCollection[i], this));
	}

}

void CryptoProvider::CreateObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object, std::vector<CK_ATTRIBUTE>& objTemplate)
{
	CK_RV rv;
	rv = GetFuncListPtr()->C_CreateObject(h_session, &objTemplate[0], objTemplate.size(), h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

void CryptoProvider::DestroyObject(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE* h_object) {
	CK_RV rv;

	rv = GetFuncListPtr()->C_DestroyObject(h_session, *h_object);

	if (rv != CKR_OK)
		throw RetVal(rv);
}

CK_FUNCTION_LIST* CryptoProvider::GetFuncListPtr() {
	return m_funcList;
}

CryptoProvider::~CryptoProvider() {
	if (m_lib != NULL) {
		FreeLibrary(m_lib);
	}
}