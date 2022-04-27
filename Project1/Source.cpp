#include <iostream>
#include <stdlib.h>
#include <vector>

//#include "classes.h"
#include "CryptoProvider.h"
#include "Slot.h"
#include "Token.h"
#include "Session.h"
#include "BasicKey.h"
#include "KeyAES.h"
#include "KeysRSA.h"


void PrintSlots(std::vector<Slot*> slotStorage) {
	for (size_t i = 0; i < slotStorage.size(); ++i) {
		std::cout << slotStorage[i]->GetSlotId() << std::endl;
	}
}


int main() {

	setlocale(0, "");

	try {

		CryptoProvider provider(L"D:\\SoftHSM2\\lib\\softhsm2-x64.dll");

		CK_BYTE app = 1;

		CK_C_INITIALIZE_ARGS initArgs;

		provider.Initialize();

		std::vector<Slot*> slotStorage;
		provider.GetSlotCollection(true, slotStorage);

		std::vector<Token*> tokenCollection;

		unsigned char LABEL[32];
		unsigned char SOPIN[32];

		std::cout << "������� PIN S/O: ";
		std::cin >> SOPIN;

		for (size_t i = 0; i < slotStorage.size(); ++i) {
			std::cout << "������� �������� label ��� ������: ";
			std::cin >> LABEL;
			slotStorage[i]->InitToken(SOPIN, LABEL);
			Token token(&provider, slotStorage[i]->GetTokenInfo());
			tokenCollection.push_back(&token);
		}

		PrintSlots(slotStorage);

		Session* session;

		session = slotStorage[0]->OpenSession(1);

		unsigned char LoginPIN[255];
		std::cout << "������� PIN S/O: ";
		std::cin >> LoginPIN;

		session->Login(CKU_SO, LoginPIN);

		unsigned char UserInitPIN[256];
		std::cout << "������� PIN ������������ (�������������): ";
		std::cin >> UserInitPIN;

		session->InitPin(UserInitPIN);

		session->Logout();

		std::cout << "������� PIN ������������: ";
		std::cin >> LoginPIN;

		session->Login(CKU_USER, LoginPIN);

		BasicKey basickey(session, &provider);
		KeyAES AES(&basickey);
		CK_OBJECT_HANDLE h_AES = NULL_PTR;

		unsigned char AESKeyLabel[256];

		std::cout << "������� Label ��� AES secret key: ";
		std::cin >> AESKeyLabel;

		AES.Generate(16, h_AES, AESKeyLabel);

		KeysRSA RSA(&basickey);

		CK_OBJECT_HANDLE h_RSA_pr = NULL_PTR;
		CK_OBJECT_HANDLE h_RSA_pub = NULL_PTR;
		unsigned char RSA_pubKeyLabel[256];
		unsigned char RSA_modulusBits[256];
		unsigned char RSA_exponent[256];
		unsigned char RSA_prKeylabel[256];
		unsigned char RSA_subject[256];
		unsigned char RSA_id[256];

		std::cout << "������� public key label: ";
		std::cin >> RSA_pubKeyLabel;
		std::cout << "������� modulus bits: ";
		std::cin >> RSA_modulusBits;
		std::cout << "������� exponent: ";
		std::cin >> RSA_exponent;
		std::cout << "������� private key label: ";
		std::cin >> RSA_prKeylabel;
		std::cout << "������� subject: ";
		std::cin >> RSA_subject;
		std::cout << "������� id: ";
		std::cin >> RSA_id;

		RSA.Generate(
			RSA_pubKeyLabel,
			RSA_modulusBits,
			RSA_exponent,
			RSA_prKeylabel,
			RSA_subject,
			RSA_id,
			h_RSA_pr, h_RSA_pub
		);

		session->Close();

		provider.Finalize();
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
		std::cout << RetEx.what();
		return RetEx.errcode();
	}
}
































































































//CryptoProvider provider(L"D:\\SoftHSM2\\lib\\softhsm2-x64.dll");

	//CK_BYTE app = 1;

	//CK_C_INITIALIZE_ARGS initArgs;

	//try {
	//	provider.Initialize();
	//}
	//catch (LibLoadErr LibEx) {
	//	std::cout << LibEx.what();
	//	return LibEx.errcode();
	//}
	//catch (FuncListErr FuncListEx) {
	//	std::cout << FuncListEx.what();
	//	return FuncListEx.errcode();
	//}
	//catch (FuncLoadErr FuncLoadEx) {
	//	std::cout << FuncLoadEx.what();
	//	return FuncLoadEx.errcode();
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "Initialize\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//std::vector<Slot*> slotStorage;
	//try {
	//	provider.GetSlotCollection(true, slotStorage);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "GetSlotCollection\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//std::vector<Token*> tokenCollection;

	///*std::string LABEL;
	//std::string SOPIN;*/

	//unsigned char LABEL[32];
	//unsigned char SOPIN[32];

	//std::cout << "������� PIN S/O: ";
	//std::cin >> SOPIN;

	//for (size_t i = 0; i < slotStorage.size(); ++i) {
	//	std::cout << "������� �������� label ��� ������: ";
	//	std::cin >> LABEL;
	//	try {
	//		slotStorage[i]->InitToken(SOPIN, LABEL);
	//	}
	//	catch (RetVal RetEx) {
	//		std::cout << "InitToken\n";
	//		std::cout << RetEx.what();
	//		return RetEx.errcode();
	//	}
	//	Token token(&provider, slotStorage[i]->GetTokenInfo());
	//	tokenCollection.push_back(&token);
	//}

	//PrintSlots(slotStorage);

	//Session* session;

	//try {	
	//	session = slotStorage[0]->OpenSession(1);  
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "OpenSession\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}
	//
	//unsigned char LoginPIN[255];
	//std::cout << "������� PIN S/O: ";
	//std::cin >> LoginPIN;

	//try {
	//	session->Login(CKU_SO, LoginPIN);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "Login\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}
	//
	//unsigned char UserInitPIN[256];
	//std::cout << "������� PIN ������������ (�������������): ";
	//std::cin >> UserInitPIN;

	//try {
	//	session->InitPin(UserInitPIN);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "InitPin\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//try {
	//	session->Logout();
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "Login\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//std::cout << "������� PIN ������������: ";
	//std::cin >> LoginPIN;

	//try {
	//	session->Login(CKU_USER, LoginPIN);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "Login\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//BasicKey basickey(session, &provider);
	//KeyAES AES(&basickey);
	//CK_OBJECT_HANDLE h_AES = NULL_PTR;

	//unsigned char AESKeyLabel[256];

	//std::cout << "������� Label ��� AES secret key: ";
	//std::cin >> AESKeyLabel;

	//try {
	//	AES.Generate(16, h_AES, AESKeyLabel);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "AES Generate\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}



	//KeysRSA RSA(&basickey);

	//CK_OBJECT_HANDLE h_RSA_pr = NULL_PTR;
	//CK_OBJECT_HANDLE h_RSA_pub = NULL_PTR;
	//unsigned char RSA_pubKeyLabel[256];
	//unsigned char RSA_modulusBits[256];
	//unsigned char RSA_exponent[256];
	//unsigned char RSA_prKeylabel[256];
	//unsigned char RSA_subject[256];
	//unsigned char RSA_id[256];

	//std::cout << "������� public key label";
	//std::cin >> RSA_pubKeyLabel;
	//std::cout << "������� modulus bits";
	//std::cin >> RSA_modulusBits;
	//std::cout << "������� exponent";
	//std::cin >> RSA_exponent;
	//std::cout << "������� private key label";
	//std::cin >> RSA_prKeylabel;
	//std::cout << "������� subject";
	//std::cin >> RSA_subject;
	//std::cout << "������� id";
	//std::cin >> RSA_id;

	//try {
	//	RSA.Generate(
	//		RSA_pubKeyLabel,
	//		RSA_modulusBits,
	//		RSA_exponent,
	//		RSA_prKeylabel,
	//		RSA_subject,
	//		RSA_id,
	//		h_RSA_pr, h_RSA_pub
	//	);
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "RSA Generate\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//try {
	//	session->Close();
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "CloseSession\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}

	//try {
	//	provider.Finalize();
	//}
	//catch (RetVal RetEx) {
	//	std::cout << "Finalize\n";
	//	std::cout << RetEx.what();
	//	return RetEx.errcode();
	//}
