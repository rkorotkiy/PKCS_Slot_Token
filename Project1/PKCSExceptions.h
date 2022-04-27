#ifndef PKCSEXCEPTIONS_H
#define PKCSEXCEPTIONS_H

#include <string>
#include <iostream>
#include "pkcs11.h"

class BaseException
{
protected:
	// ���������� � ���������� - ����� ��� ���� ����������� �������
	std::string text;
	int code = 0;


public:
	// ������ ����������� �������, ������� ������� ����� ����������
	virtual std::string what() = 0;
};

class PKCSExceptions : public BaseException {
public:
	PKCSExceptions()
	{
		text = "Error. PKCS Exception.";
		code = 1;
	}

	// ����������� � �������� �������
	PKCSExceptions(std::string _text) { text = _text; }

	// �������������� ����������� ������� what() - �����������
	std::string what() { return text; }
	int errcode() { return code; }
};

class RetVal final :public PKCSExceptions {
public:
	// �����������, ������� �������� ����� ����������� ������ ����������
	RetVal(unsigned long RV) : PKCSExceptions()
	{
		text = "Return value is not CKR_OK. Code " + std::to_string(RV); // ����� �� ���������
		code = 2;
	}

	std::string what()
	{
		return text;
	}
	int errcode() { return code; }
};

class LibLoadErr final : public PKCSExceptions {
public:
	LibLoadErr() : PKCSExceptions()
	{
		text = "Error loading library.";
		code = 3;
	}

	std::string what() 
	{
		return text;
	}
	int errcode() { return code; }
};

class FuncLoadErr final : public PKCSExceptions {
public:
	FuncLoadErr() : PKCSExceptions()
	{
		text = "Error loading function.";
		code = 4;
	}

	std::string what()
	{
		return text;
	}
	int errcode() { return code; }
};

class FuncListErr final : public PKCSExceptions {
public:
	FuncListErr() : PKCSExceptions() 
	{
		text = "CK_FUNCTION_LIST_PTR == NULL.";
		code = 5;
	}
	std::string what()
	{
		return text;
	}
	int errcode() { return code; }
};

#endif