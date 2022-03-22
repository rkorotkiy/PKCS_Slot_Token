#include "classes.h"

void KeysRSA::Generate(
	std::string public_label,
	std::string public_modulusBits,
	std::string public_exponent,
	std::string private_label,
	std::string private_subject,
	std::string private_id,
	CK_OBJECT_HANDLE privateKeyHandle,
	CK_OBJECT_HANDLE publicKeyHandle) {

	CK_RV rv;

	memcpy(pub_label, public_label.c_str(), public_label.size() + 1);
	memcpy(modulusBits, public_modulusBits.c_str(), public_modulusBits.size() + 1);
	memcpy(exponent, public_exponent.c_str(), public_exponent.size() + 1);
	memcpy(pr_label, private_label.c_str(), private_label.size() + 1);
	memcpy(subject, private_subject.c_str(), private_subject.size() + 1);
	memcpy(id, private_id.c_str(), private_id.size() + 1);

	rv = m_provider->GetFuncListPtr()->C_GenerateKeyPair(
		m_session->GetHandle(), 
		&mechanism, 
		RSAPublicKeyTemplate, 8, 
		RSAPrivateKeyTemplate, 9, 
		&publicKeyHandle, &privateKeyHandle
	);

}