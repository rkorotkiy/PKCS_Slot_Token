#include <string>
#include <iostream>

class BaseException
{
protected:
	// ���������� � ���������� - ����� ��� ���� ����������� �������
	std::string text;

public:
	// ������ ����������� �������, ������� ������� ����� ����������
	virtual std::string what() = 0;
};

class PKCSExceptions : public BaseException {
public:
	PKCSExceptions()
	{
		text = "Error. PKCS Exception.";
	}

	// ����������� � �������� �������
	PKCSExceptions(std::string _text) { text = _text; }

	// �������������� ����������� ������� what() - �����������
	std::string what() { return text; }
};

class RetVal final :public PKCSExceptions {
public:
	// �����������, ������� �������� ����� ����������� ������ ����������
	RetVal(int RV) : PKCSExceptions()
	{
		text = "Return value is not CKR_OK. Code " + std::to_string(RV); // ����� �� ���������
	}

	std::string what()
	{
		return text;
	}
};

class LibLoadErr final : public PKCSExceptions {
public:
	LibLoadErr() : PKCSExceptions()
	{
		text = "Error loading library.";
	}

	std::string what() 
	{
		return text;
	}
};

class FuncLoadErr final : public PKCSExceptions {
public:
	FuncLoadErr() : PKCSExceptions()
	{
		text = "Error loading function.";
	}

	std::string what()
	{
		return text;
	}
};

class FuncListErr final : public PKCSExceptions {
public:
	FuncListErr() : PKCSExceptions() 
	{
		text = "CK_FUNCTION_LIST_PTR == NULL.";
	}
	std::string what()
	{
		return text;
	}
};