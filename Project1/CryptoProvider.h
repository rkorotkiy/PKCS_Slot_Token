#ifndef CRYPTOPROVIDER_H
#define CRYPTOPROVIDER_H

#include "pkcs11.h"
#include "tdef.h"
#include "PKCSExceptions.h"
#include "Slot.h"
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

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

#endif