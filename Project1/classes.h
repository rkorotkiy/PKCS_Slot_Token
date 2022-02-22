#pragma once





//class key_AES {
//private:
//	CK_MECHANISM m_mechanism = {          //class key -> наследование key aes
//	  CKM_AES_KEY_GEN, NULL_PTR, 0
//	};
//	CryptoProvider* m_provider;
//
//public:
//	key_AES(CK_SESSION_HANDLE h_session, CK_OBJECT_HANDLE h_key, CryptoProvider* provider,
//		std::vector<CK_ATTRIBUTE> objTemplate) : m_provider(provider)
//	{
//		CK_RV rv;
//		rv = m_provider->GetFuncListPtr()->C_GenerateKey(h_session, &m_mechanism, &objTemplate[0], objTemplate.size(), &h_key);
//
//		if (rv != CKR_OK)
//			throw RetVal(rv);
//
//	}
//
//};