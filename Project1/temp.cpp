//try {
//
//	CryptoProvider provider(L"D:\\SoftHSM2\\lib\\softhsm2-x64.dll");
//
//	CK_BYTE app = 1;
//
//	CK_C_INITIALIZE_ARGS initArgs;
//
//	provider.Initialize();
//
//	std::vector<Slot*> slotStorage;
//	provider.GetSlotCollection(true, slotStorage);
//
//	std::vector<Token*> tokenCollection;
//
//	unsigned char LABEL[32];
//	unsigned char SOPIN[32];
//
//	std::cout << "¬ведите PIN S/O: ";
//	std::cin >> SOPIN;
//
//	for (size_t i = 0; i < slotStorage.size(); ++i) {
//		std::cout << "¬ведите значение label дл€ токена: ";
//		std::cin >> LABEL;
//		slotStorage[i]->InitToken(SOPIN, LABEL);
//		Token token(&provider, slotStorage[i]->GetTokenInfo());
//		tokenCollection.push_back(&token);
//	}
//
//	PrintSlots(slotStorage);
//
//	Session* session;
//
//	session = slotStorage[0]->OpenSession(1);
//
//	unsigned char LoginPIN[255];
//	std::cout << "¬ведите PIN S/O: ";
//	std::cin >> LoginPIN;
//
//	session->Login(CKU_SO, LoginPIN);
//
//	unsigned char UserInitPIN[256];
//	std::cout << "¬ведите PIN пользовател€ (инициализаци€): ";
//	std::cin >> UserInitPIN;
//
//	session->InitPin(UserInitPIN);
//
//	session->Logout();
//
//	std::cout << "¬ведите PIN пользовател€: ";
//	std::cin >> LoginPIN;
//
//	session->Login(CKU_USER, LoginPIN);
//
//	BasicKey basickey(session, &provider);
//	KeyAES AES(&basickey);
//	CK_OBJECT_HANDLE h_AES = NULL_PTR;
//
//	unsigned char AESKeyLabel[256];
//
//	std::cout << "¬ведите Label дл€ AES secret key: ";
//	std::cin >> AESKeyLabel;
//
//	AES.Generate(16, h_AES, AESKeyLabel);
//
//	KeysRSA RSA(&basickey);
//
//	CK_OBJECT_HANDLE h_RSA_pr = NULL_PTR;
//	CK_OBJECT_HANDLE h_RSA_pub = NULL_PTR;
//	unsigned char RSA_pubKeyLabel[256];
//	unsigned char RSA_modulusBits[256];
//	unsigned char RSA_exponent[256];
//	unsigned char RSA_prKeylabel[256];
//	unsigned char RSA_subject[256];
//	unsigned char RSA_id[256];
//
//	std::cout << "¬ведите public key label";
//	std::cin >> RSA_pubKeyLabel;
//	std::cout << "¬ведите modulus bits";
//	std::cin >> RSA_modulusBits;
//	std::cout << "¬ведите exponent";
//	std::cin >> RSA_exponent;
//	std::cout << "¬ведите private key label";
//	std::cin >> RSA_prKeylabel;
//	std::cout << "¬ведите subject";
//	std::cin >> RSA_subject;
//	std::cout << "¬ведите id";
//	std::cin >> RSA_id;
//
//	RSA.Generate(
//		RSA_pubKeyLabel,
//		RSA_modulusBits,
//		RSA_exponent,
//		RSA_prKeylabel,
//		RSA_subject,
//		RSA_id,
//		h_RSA_pr, h_RSA_pub
//	);
//
//	session->Close();
//
//	provider.Finalize();
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
//	std::cout << RetEx.what();
//	return RetEx.errcode();
//}