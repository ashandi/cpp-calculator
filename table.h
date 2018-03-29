#include <string>

struct user_var {
    std::string name;
    user_var* next;
    double value;
};

extern user_var* look(std::string name, bool insert = false);
inline user_var* insert(std::string name) { return look(name, true); }
