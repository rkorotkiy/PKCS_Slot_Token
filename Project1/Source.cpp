#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>
#include "tdef.h"
#include "PKCSExceptions.h"
#include "pkcs11.h"
#include "CryptoProvider.h"
#include "Slot.h"
#include "Session.h"
#include "Token.h"


void PrintSlots(std::vector<Slot*> slotStorage) {
	for (size_t i = 0; i < slotStorage.size(); ++i) {
		std::cout << slotStorage[i]->GetSlotId() << std::endl;
	}
}


int main() {

	setlocale(0, "");

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

	std::vector<Slot*> slotStorage;
	try {
		provider.GetSlotCollection(true, slotStorage);
	}
	catch (RetVal RetEx) {
		std::cout << "GetSlotCollection\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	std::vector<Token*> tokenCollection;
	for (size_t i = 0; i < slotStorage.size(); ++i) {
		try {
			slotStorage[i]->InitToken();
		}
		catch (RetVal RetEx) {
			std::cout << "InitToken\n";
			std::cout << RetEx.what();
			return RetEx.errcode();
		}
		Token token(&provider, slotStorage[i]->GetTokenInfo());
		tokenCollection.push_back(&token);
	}

	PrintSlots(slotStorage);

	Session* session;

	try {	
		session = slotStorage[0]->OpenSession(1);
	}
	catch (RetVal RetEx) {
		std::cout << "OpenSession\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}



	try {
		session->Login(CKU_SO);
	}
	catch (RetVal RetEx) {
		std::cout << "Login\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		session->InitPin();
	}
	catch (RetVal RetEx) {
		std::cout << "InitPin\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		session->Logout();
	}
	catch (RetVal RetEx) {
		std::cout << "Login\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		session->Login(CKU_USER);
	}
	catch (RetVal RetEx) {
		std::cout << "Login\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}



	CK_OBJECT_HANDLE h_Key;

	CK_OBJECT_CLASS KeyClass = CKO_SECRET_KEY;
	CK_KEY_TYPE KeyType = CKK_AES;
	CK_BBOOL True = CK_TRUE;
	CK_BBOOL False = CK_FALSE;

	std::vector<CK_ATTRIBUTE> keyTemplate
	{
		{CKA_CLASS, &KeyClass, sizeof(KeyClass)},
		{CKA_KEY_TYPE, &KeyType, sizeof(KeyType)},
		{CKA_VALUE}
	};

	try {
		provider.CreateObject(session->GetHandle(), h_Key, keyTemplate);
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

	/*try {
		provider.CloseSession(h_Session);
	}
	catch (RetVal RetEx) {
		std::cout << "CloseSession\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}*/

	try {
		provider.Finalize();
	}
	catch (RetVal RetEx) {
		std::cout << "Finalize\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}
}