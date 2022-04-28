#ifndef KEYSRSA_H
#define KEYSRSA_H

class KeysRSA : public BasicKey {
private:

	CK_ATTRIBUTE* GetPubTemplatePtr(
		unsigned char* public_label,
		unsigned char* public_modulusBits,
		unsigned char* public_exponent
	);

	CK_ATTRIBUTE* GetPrivTemplatePtr(
		unsigned char* private_label,
		unsigned char* private_subject,
		unsigned char* private_id
	);

	CK_MECHANISM mechanism = {
		CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0
	};

public:
	KeysRSA(BasicKey* bKey) : BasicKey(bKey->GetSession(), bKey->GetProvider()) {};
	void Generate(
		unsigned char* public_label,
		unsigned char* public_modulusBits,
		unsigned char* public_exponent,
		unsigned char* private_label,
		unsigned char* private_subject,
		unsigned char* private_id,
		CK_OBJECT_HANDLE privateKeyHandle,
		CK_OBJECT_HANDLE publicKeyHandle);
};

#endif