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

	std::string LABEL;
	std::string SOPIN;

	std::cout << "������� PIN S/O: ";
	std::cin >> SOPIN;

	for (size_t i = 0; i < slotStorage.size(); ++i) {
		std::cout << "������� �������� label ��� ������: ";
		std::cin >> LABEL;
		try {
			slotStorage[i]->InitToken(SOPIN, LABEL);
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
	
	std::string LoginPIN;
	std::cout << "������� PIN S/O: ";
	std::cin >> LoginPIN;

	try {
		session->Login(CKU_SO, LoginPIN);
	}
	catch (RetVal RetEx) {
		std::cout << "Login\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}
	
	std::string UserInitPIN;
	std::cout << "������� PIN ������������ (�������������): ";
	std::cin >> UserInitPIN;

	try {
		session->InitPin(UserInitPIN);
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

	std::cout << "������� PIN ������������: ";
	std::cin >> LoginPIN;

	try {
		session->Login(CKU_USER, LoginPIN);
	}
	catch (RetVal RetEx) {
		std::cout << "Login\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	BasicKey basickey(session, &provider);
	KeyAES AES(&basickey);
	CK_OBJECT_HANDLE h_AES = NULL_PTR;

	std::string AESKeyLabel;

	std::cout << "������� Label ��� AES secret key: ";
	std::cin >> AESKeyLabel;

	try {
		AES.Generate(16, h_AES, AESKeyLabel);
	}
	catch (RetVal RetEx) {
		std::cout << "AES Generate\n";
		std::cout << RetEx.what();
		return RetEx.errcode();
	}

	KeysRSA RSA(&basickey);

	CK_OBJECT_HANDLE h_RSA_pr = NULL_PTR;
	CK_OBJECT_HANDLE h_RSA_pub = NULL_PTR;
	std::string RSA_pubKeyLabel;
	std::string RSA_modulusBits;
	std::string RSA_exponent;
	std::string RSA_prKeylabel;
	std::string RSA_subject;
	std::string RSA_id;

	std::cout << "������� public key label";
	std::cin >> RSA_pubKeyLabel;
	std::cout << "������� modulus bits";
	std::cin >> RSA_modulusBits;
	std::cout << "������� exponent";
	std::cin >> RSA_exponent;
	std::cout << "������� private key label";
	std::cin >> RSA_prKeylabel;
	std::cout << "������� subject";
	std::cin >> RSA_subject;
	std::cout << "������� id";
	std::cin >> RSA_id;

	try {
		RSA.Generate(
			RSA_pubKeyLabel,
			RSA_modulusBits,
			RSA_exponent,
			RSA_prKeylabel,
			RSA_subject,
			RSA_id,
			h_RSA_pr, h_RSA_pub
		);
	}
	catch (RetVal RetEx) {
		std::cout << "RSA Generate\n";
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