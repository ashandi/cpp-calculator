#include <iostream>
#include <string>
#include "token.h"
#include "error.h"

Token TokenBuffer::get()
{
    if(buffer_not_empty) {
        buffer_not_empty = false;
        return buffer;
    }

    char sign;
    do {
        if (!std::cin.get(sign))
            return Token(END);
    } while (sign != '\n' && isspace(sign));

    switch (sign) {
    case ';': case '\n':
        return Token(PRINT);
    case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9': case '.': {
        std::cin.putback(sign);
        double val;
        std::cin >> val;
        return Token(NUMBER, val);
    }
    case '+': case '-': case '/': case '*':
    case '(': case ')': case '=':
        return Token(token_value(sign));
    default:
        if (isalpha(sign))
        {
            std::string str;
            str += sign;
            while(std::cin.get(sign) && isalnum(sign))
            {
                str += sign;
            }
            std::cin.putback(sign);
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
