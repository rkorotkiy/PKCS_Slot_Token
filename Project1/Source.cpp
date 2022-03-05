#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <vector>

#include "classes.h"


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
		session->Login(CKU_SO);           // пропадают значени€ полей
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

	CK_UTF8CHAR label;
	std::cout << "¬ведите label дл€ AESKey";
	std::cin >> label;

	CK_OBJECT_HANDLE AESKey = NULL_PTR;

	KeyAES AES;
	
	try {
		AES.Generate(&label, 16, 16, AESKey);
	}
	catch (RetVal RetEx) {
		std::cout << "Generate\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	try {
		session->Close();
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