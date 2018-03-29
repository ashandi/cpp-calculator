#include <iostream>
#include <string>
#include "error.h"

void error(std::string message)
{
    std::cout << "error: " << message << std::endl;
}
