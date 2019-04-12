#include "std_lib_facilities.h"

//------------------------------------------------------------------------------------------------

class Token { //����� ��� �������� � ����������� ������
public:
	char kind;
	double value;
	Token(char ch): kind(ch), value(0){ }
	Token(char ch, double val): kind(ch), value(val){ }
};

//------------------------------------------------------------------------------------------------

class Token_stream { //����� - ����� ������
public: //���������������� ���������
	Token_stream(); //������ ������ ������ Token_stream, ����������� ������ �� ������ cin 
	Token get(); //�������� ������ Token �� ������ 
	void putback(Token t); //���������� Token ������� � �����
private: //������ ����������� ��� ���������� ������
	bool full {false}; //������� ������� � ������(�������-���)
	Token buffer; //����� ��� �������� ������� ����� �������� � ������� putback()
};
//------------------------------------------------------------------------------------------------

Token_stream::Token_stream(): full(false), buffer(0){ } //����������� ��������������� ��������� ���������� ������ � ����. false � ������ ������ �����

//------------------------------------------------------------------------------------------------

void Token_stream::putback(Token t){ //����������� ������� putback()
	if (full) error("����� ��������!");
	full = true; //��������� ���������� ���������� ������ � 1
	buffer = t;	//����������� ������� � �����
}

//------------------------------------------------------------------------------------------------

Token Token_stream::get(){ //O���������� ������� get()
	if (full) //���� � ������ ���� �����-�� �������, �� ���������� �
	{
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;
	
	switch(ch){
		case '=': //��� ������ ����������
		case 'x':
		case '(': case ')': case '+': case '-': case '*': case '/': case '{': case '}': case '!':
			return Token{ch};
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				cin.putback(ch); //���������� ������ ������� � ����� cin
				double val;
				cin >> val; //��������� ����� � ��������� ������
				return Token{'8', val}; //����������� ������� "�����" (�� ���� ��������������� �������� ������ Token = '8') 
			}
		default:
			error("�������� �������");
	}
}

//------------------------------------------------------------------------------------------------

Token_stream ts; //���������� ���� Token_stream ����������� ��� ������������� ������� get() & putback() ������ Taken_stream

//------------------------------------------------------------------------------------------------

double expression(); //���������� ��� ������������� � ������� Primary()

//------------------------------------------------------------------------------------------------

double term(); //���������� ��� ������������� � ������� Primary()

//------------------------------------------------------------------------------------------------

int factorial(int x){ //������� ��� ���������� ����������
	if (x == 0) x = 1;
	int i = x;
	--i;
	while(i > 0){
		x *= i;
		--i;
	}
	return x;
}

//------------------------------------------------------------------------------------------------

double primary(){ //������� ��� ����������� ���������� ��������� (������ � ������� � ��������)
	Token t = ts.get();
	switch(t.kind){
		case '(':
		{
			double d = expression();
			t = ts.get();
			if (t.kind != ')') 
			{
				error("��������� ')'");
			}
			return d;
		}
		case '{':
		{
			double c = term();
			t = ts.get();
			if (t.kind != '}') 
			{
				error("��������� '}'");
			}
			return c;
		}
		case '8': //���������� '8' ��� ������������� �����
			return t.value; //������� �������� �����
		default:
			error("��������� ��������� ���������!");
	}
}

//------------------------------------------------------------------------------------------------

double term(){ //������� ��� ����������� ����� (������ � * � /)
	double left = primary(); 
	Token t = ts.get(); 
	while(true)
	{
		switch(t.kind)
		{
			case '!': //��������� (����. 7! = 7*6*5*4*3*2*1)
			{
				int x = left; //������������ ���������� � �������� ���������� left, ��� ��� ���������� � � ���� int
				left = factorial(x);
				t = ts.get();
				break;
			}
			case '*':
			{
				left *= primary();
				t = ts.get();
				break;
			}
			case '/':
			{
				double d = primary();
				if(d==0)
				{
					error("������� �� ����");
				}
				left /= d;
				t = ts.get();
				break;
			}
			default:
			{
				ts.putback(t); //������� t � ����� ������;
				return left;
			}
		}
	}	
}

//------------------------------------------------------------------------------------------------

double expression(){
	double left = term(); //���������� � ���������� �����
	Token t = ts.get(); //��������� �������
	while(true)
	{
		switch(t.kind)
		{
		case '+':
			left += term(); //���������� � ������������ �����
			t = ts.get();
			break;
		case '-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t); //������� t � ����� ������
			return left; //����� �������. ���� ��� + ��� -, ���������� �����
		}
	}
}

//------------------------------------------------------------------------------------------------

int main()
try
{
	cout << "����� ���������� � ���������-����� �����������!\n"
		 << "������� ��������� � ������� � ��������� ������.\n"
		 << "��� ��������� ���������� ������ ������ '='.\n"
		 << "��� ������ ������� ������ 'x'(���).\n";
	double val = 0;
	while(cin){
		Token t = ts.get();
		if(t.kind == 'x') break;
		if(t.kind == '=') cout << "=" << val << '\n';
		else ts.putback(t);
		val = expression();
	}
	keep_window_open();
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n'; 
	keep_window_open();
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
	keep_window_open();
    return 2;
}

//------------------------------------------------------------------------------------------------