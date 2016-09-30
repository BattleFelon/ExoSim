#include <iostream>
#include <string>
#include "Murano.h"

int main(int argc, char** argv)
{

    Murano M;

    std::cout << M.tryActivate("12345", "blarg") << std::endl;
    return 0;
}

