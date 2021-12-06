#include <string>
#include <iostream>

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
		int code = 1;
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
	RetVal(int RV) : PKCSExceptions()
	{
		text = "Return value is not CKR_OK. Code " + std::to_string(RV); // текст по умолчанию
		int code = 2;
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
		int code = 3;
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
		int code = 4;
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
		int code = 5;
	}
	std::string what()
	{
		return text;
	}
	int errcode() { return code; }
};