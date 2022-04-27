//#include "classes.h"
#include "KeysRSA.h"

void KeysRSA::Generate(
	unsigned char* public_label,
	unsigned char* public_modulusBits,
	unsigned char* public_exponent,
	unsigned char* private_label,
	unsigned char* private_subject,
	unsigned char* private_id,
	CK_OBJECT_HANDLE privateKeyHandle,
	CK_OBJECT_HANDLE publicKeyHandle) {

	CK_RV rv;

	

	rv = m_provider->GetFuncListPtr()->C_GenerateKeyPair(
		m_session->GetHandle(), 
		&mechanism, 
		GetPubTemplatePtr(public_label, public_modulusBits, public_exponent), 8,
		GetPrivTemplatePtr(private_label, private_subject, private_id), 9,
		&publicKeyHandle, &privateKeyHandle
	);

}

CK_ATTRIBUTE* KeysRSA::GetPubTemplatePtr(
	unsigned char* public_label,
	unsigned char* public_modulusBits,
	unsigned char* public_exponent
) {

	CK_OBJECT_CLASS objClass = CKO_PUBLIC_KEY;
	CK_KEY_TYPE keyType = CKK_RSA;
	CK_UTF8CHAR* pub_label;
	CK_BYTE* modulusBits;
	CK_BYTE* exponent;
	CK_BBOOL True = CK_TRUE;

	CK_ATTRIBUTE RSAPublicKeyTemplate[8] = {
		{CKA_CLASS, &objClass, sizeof(objClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &True, sizeof(true)},
		{CKA_LABEL, &pub_label, sizeof(pub_label) - 1},
		{CKA_WRAP, &True, sizeof(true)},
		{CKA_ENCRYPT, &True, sizeof(true)},
		{CKA_MODULUS_BITS, &modulusBits, sizeof(modulusBits)},
		{CKA_PUBLIC_EXPONENT, &exponent, sizeof(exponent)}
	};

	/*memcpy(pub_label, public_label.c_str(), public_label.size() + 1);
	memcpy(modulusBits, public_modulusBits.c_str(), public_modulusBits.size() + 1);
	memcpy(exponent, public_exponent.c_str(), public_exponent.size() + 1);*/

	pub_label = public_label;
	modulusBits = public_modulusBits;
	exponent = public_exponent;

	return RSAPublicKeyTemplate;

}

CK_ATTRIBUTE* KeysRSA::GetPrivTemplatePtr(
	unsigned char* private_label,
	unsigned char* private_subject,
	unsigned char* private_id
) {

	CK_OBJECT_CLASS objClass = CKO_PRIVATE_KEY;
	CK_KEY_TYPE keyType = CKK_RSA;
	CK_UTF8CHAR* pr_label;
	CK_BYTE* subject;
	CK_BYTE* id;
	CK_BBOOL True = CK_TRUE;

	CK_ATTRIBUTE RSAPrivateKeyTemplate[9] = {
		{CKA_CLASS, &objClass, sizeof(objClass)},
		{CKA_KEY_TYPE, &keyType, sizeof(keyType)},
		{CKA_TOKEN, &True, sizeof(true)},
		{CKA_LABEL, &pr_label, sizeof(pr_label) - 1},
		{CKA_SUBJECT, &subject, sizeof(subject)},
		{CKA_ID, &id, sizeof(id)},
		{CKA_SENSITIVE, &True, sizeof(true)},
		{CKA_DECRYPT, &True, sizeof(true)},
		{CKA_SIGN, &True, sizeof(true)},
	};

	/*memcpy(pr_label, private_label.c_str(), private_label.size() + 1);
	memcpy(subject, private_subject.c_str(), private_subject.size() + 1);
	memcpy(id, private_id.c_str(), private_id.size() + 1);*/

	pr_label = private_label;
	subject = private_subject;
	id = private_id;

	return RSAPrivateKeyTemplate;

}