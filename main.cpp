#include <iostream>
#include <string.h>

using namespace std;

int number_of_errors = 0;
double error(const char* message);

enum token_value {
    NAME, NUMBER, END,
    PLUS='+', MINUS='-', MULTIPLICATION='*', DIVISION='/',
    PRINT=';', ASSIGN='=', LEFT_BRACKET='(', RIGHT_BRACKET=')'
};

class Token
{
public:
    token_value kind;
    double d_value;
    string s_value;
    Token(token_value t):
        kind(t) {}
    Token(token_value t, double value):
        kind(t), d_value(value) {}
    Token(token_value t, string value):
        kind(t), s_value(value) {}
};

class TokenBuffer
{
public:
    TokenBuffer():
        buffer_not_empty(false), buffer(PRINT) {}
    Token get();
    void putback(Token t);
private:
    bool buffer_not_empty;
    Token buffer;
};

Token TokenBuffer::get()
{
    if(buffer_not_empty) {
        buffer_not_empty = false;
        return buffer;
    }

    char sign;
    do {
        if (!cin.get(sign))
            return Token(END);
    } while (sign != '\n' && isspace(sign));

    switch (sign) {
    case ';': case '\n':
        return Token(PRINT);
    case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '.': {
        cin.putback(sign);
        double val;
        cin >> val;
        return Token(NUMBER, val);
    }
    case '+': case '-': case '/': case '*':
    case '(': case ')': case '=':
        return Token(token_value(sign));
    default:
        if (isalpha(sign))
        {
            string str;
            str += sign;
            while(cin.get(sign) && isalnum(sign))
            {
                str += sign;
            }
            cin.putback(sign);
            return Token(NAME, str);
        }
        error("Incorrect sign!");
        return Token(PRINT);
    }
}

void TokenBuffer::putback(Token t)
{
    buffer = t;
    buffer_not_empty = true;
}

double expression();
double term();
double prim();

struct name {
    char* string;
    name* next;
    double value;
};

const int table_size = 32;
name* table[table_size];

name* insert(const char* name);
name* look(const char* name, bool insert = false);

const char* prompt = ">";
const char* result = "=";

TokenBuffer token_buffer = TokenBuffer();

int main()
{
    insert("pi")->value = 3.1415926535897932385;
    insert("e")->value = 2.7182818284590452354;

    while(true)
    {
        cout << prompt;

        Token t = token_buffer.get();

        if (t.kind == PRINT)
            continue;
        if (t.kind == END)
            break;

        token_buffer.putback(t);

        cout << result << expression() << endl;
    }

    return 0;
}

double expression()
{
    double left = term();
    Token t = token_buffer.get();

    while(true)
    {
        switch (t.kind) {
        case PLUS:
            left += term();
            t = token_buffer.get();
            break;
        case MINUS:
            left -= term();
            t = token_buffer.get();
            break;
        default:
            token_buffer.putback(t);
            return left;
        }
    }
}

double term()
{
    double left = prim();
    Token t = token_buffer.get();

    while(true)
    {
        switch(t.kind) {
        case MULTIPLICATION:
            left *= prim();
            t = token_buffer.get();
            break;
        case DIVISION:{
            double d = prim();
            if (d == 0)
                return error("Division by zero!");
            left /= d;
            t = token_buffer.get();
            break;
        }
        default:
            token_buffer.putback(t);
            return left;
       }
    }
}

double prim()
{
    Token t = token_buffer.get();
    switch(t.kind) {
    case NUMBER:
        return t.d_value;
    case NAME: {
        string var = t.s_value;
        t = token_buffer.get();
        if (t.kind == ASSIGN)
        {
            name *n = insert(var.c_str());
            n->value = expression();
            return n->value;
        }
        token_buffer.putback(t);
        return look(var.c_str())->value;
    }
    case MINUS:
        return -prim();
    case LEFT_BRACKET: {
        double e = expression();
        t = token_buffer.get();
        if (t.kind != RIGHT_BRACKET)
            return error("Expected \')\'");
        return e;
    }
    case END:
        return 1;
    default:
        return error("Undefined sign!");
    }
}

double error(const char* message)
{
    cerr << "error: " << message << endl;
    number_of_errors++;
    return 1;
}

name* insert(const char* name)
{
    return look(name, true);
}

name* look(const char* var, bool ins)
{
    int hash = 0;
    const char* p = var;
    while(*p)
    {
        hash = hash << 1 ^ *p++;
    }
    if (hash < 0) hash = -hash;
    hash %= table_size;
    for (name* n = table[hash]; n; n = n->next)
    {
        if (strcmp(var, n->string) == 0) return n;
    }
    if (!ins) error("Name not found");

    name* new_name = new name;
    new_name->string = new char[strlen(var) + 1];
    strcpy(new_name->string, var);
    new_name->value = 1;
    new_name->next = table[hash];
    table[hash] = new_name;

    return new_name;
}
