#include <iostream>
#include <string>

#include "error.h"
#include "table.h"
#include "token.h"
#include "functions.h"

using namespace std;

TokenBuffer token_buffer = TokenBuffer();

int main()
{
    const char* prompt = ">";
    const char* result = "=";

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
