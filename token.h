#include <string>

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
    std::string s_value;
    Token(token_value t):
        kind(t) {}
    Token(token_value t, double value):
        kind(t), d_value(value) {}
    Token(token_value t, std::string value):
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

extern TokenBuffer token_buffer;
