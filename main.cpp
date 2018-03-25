#include <iostream>
#include <string.h>

using namespace std;

enum token_value {
    NAME, NUMBER, END,
    PLUS='+', MINUS='-', MULTIPLICATION='*', DIVISION='/',
    PRINT=';', ASSIGN='=', LEFT_BRACKET='(', RIGHT_BRACKET=')'
};

token_value get_token();
double expression();
double term();
double prim();

token_value current_token;
double number_value;
char name_string[256];

int number_of_errors = 0;
double error(const char* message);

struct name {
    char* string;
    name* next;
    double value;
};

const int table_size = 23;
name* table[table_size];

name* insert(const char* name);
name* look(const char* name, bool insert = false);

int main()
{
    insert("pi")->value = 3.1415926535897932385;
    insert("e")->value = 2.7182818284590452354;

    while(true)
    {
        cout << ">";

        get_token();
        if (current_token == END)
            break;
        if (current_token == PRINT)
            continue;

        cout << expression() << endl;
    }

    return 0;
}

double expression()
{
    double left = term();

    while(true)
    {
        switch (current_token) {
        case PLUS:
            get_token();
            left += term();
            break;
        case MINUS:
            get_token();
            left -= term();
            break;
        default:
            return left;
        }
    }
}

double term()
{
    double left = prim();

    while(true)
    {
        switch(current_token) {
        case MULTIPLICATION:
            get_token();
            left *= prim();
            break;
        case DIVISION:{
            get_token();
            double d = prim();
            if (d == 0)
                return error("Division by zero!");
            left /= d;
            break;
        }
        default:
            return left;
       }
    }
}

double prim()
{
    switch(current_token) {
    case NUMBER:
        get_token();
        return number_value;
    case NAME:
        if (get_token() == ASSIGN)
        {
            name *n = insert(name_string);
            get_token();
            n->value = expression();
            return n->value;
        }
        return look(name_string)->value;
    case MINUS:
        get_token();
        return -prim();
    case LEFT_BRACKET: {
        get_token();
        double e = expression();
        if (current_token != RIGHT_BRACKET)
            return error("Expected \')\'");
        get_token();
        return e;
    }
    case END:
        return 1;
    default:
        return error("Undefined sign!");
    }
}

token_value get_token()
{
    char sign;
    do {
        if (!cin.get(sign))
            return current_token = END;
    } while (sign != '\n' && isspace(sign));

    switch (sign) {
    case ';': case '\n':
        return current_token = PRINT;
    case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '.':
        cin.putback(sign);
        cin >> number_value;
        return current_token = NUMBER;
    case '+': case '-': case '/': case '*':
    case '(': case ')': case '=':
        return current_token = token_value(sign);
    default:
        if (isalpha(sign))
        {
            char *p = name_string;
            *p++ = sign;
            while(cin.get(sign) && isalnum(sign))
            {
                *p++ = sign;
            }
            cin.putback(sign);
            *p = 0;
            return current_token = NAME;
        }
        error("Incorrect sign!");
        return current_token = PRINT;
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
