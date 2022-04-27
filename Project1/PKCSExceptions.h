#ifndef PKCSEXCEPTIONS_H
#define PKCSEXCEPTIONS_H

#include <string>
#include <iostream>
#include "pkcs11.h"

class BaseException
{
protected:
	// объ€снение к исключению - общее дл€ всех производных классов
	std::string text;
	int code = 0;


public:
	// чиста€ виртуальна€ функци€, котора€ выводит текст исключени€
	virtual std::string what() = 0;
};

class PKCSExceptions : public BaseException {
public:
	PKCSExceptions()
	{
		text = "Error. PKCS Exception.";
		code = 1;
	}

	// конструктор с заданным текстом
	PKCSExceptions(std::string _text) { text = _text; }

	// переопредел€ем виртуальную функцию what() - об€зательно
	std::string what() { return text; }
	int errcode() { return code; }
};

class RetVal final :public PKCSExceptions {
public:
	// конструктор, который содержит текст описывающий данное исключение
	RetVal(unsigned long RV) : PKCSExceptions()
	{
		text = "Return value is not CKR_OK. Code " + std::to_string(RV); // текст по умолчанию
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