#include <string>
#include "table.h"
#include "error.h"

const int table_size = 32;
user_var* table[table_size];

user_var* look(std::string name, bool ins)
{
    int hash = 0;
    const char* p = name.c_str();
    while(*p) {
        hash = hash << 1 ^ *p++;
    }
    if (hash < 0) hash = -hash;
    hash %= table_size;
    for (user_var* var = table[hash]; var; var = var->next) {
        if (name == var->name) return var;
    }
    if (!ins) error("Name not found");

    user_var* new_var = new user_var;
    new_var->name = name;
    new_var->value = 1;
    new_var->next = table[hash];
    table[hash] = new_var;

    return new_var;
}
