#include <string>
#include "functions.h"
#include "error.h"
#include "token.h"
#include "table.h"

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
            if (d == 0){
                error("Division by zero!");
                return 1;
            }
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
        std::string name = t.s_value;
        t = token_buffer.get();
        if (t.kind == ASSIGN)
        {
            user_var *var = insert(name);
            var->value = expression();
            return var->value;
        }
        token_buffer.putback(t);
        return look(name)->value;
    }
    case MINUS:
        return -prim();
    case LEFT_BRACKET: {
        double e = expression();
        t = token_buffer.get();
        if (t.kind != RIGHT_BRACKET) {
            error("Expected \')\'");
            return 1;
        }
        return e;
    }
    case END:
        return 1;
    default:
        error("Undefined sign!");
        return 1;
    }
}
