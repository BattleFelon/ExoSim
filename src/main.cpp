#include <iostream>
#include <string>
#include "Murano.h"

int main(int argc, char** argv)
{

    Murano M;

    std::cout << M.tryActivate("1234", "dubhxzv0r4e1m7vi") << std::endl;
    std::cout << M.postData("1234", "10") << std::endl;
    return 0;
}

