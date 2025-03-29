#include <iostream>
#include "Logger.h"

void printCppStandard()
{
    switch( __cplusplus )
    {
    case 199711L:
        Logger::Info( "%s: C++98/C++03", __func__ );
        break;
    case 201103L:
        Logger::Info( "%s: C++11", __func__ );
        break;
    case 201402L:
        Logger::Info( "%s: C++14", __func__ );
        break;
    case 201703L:
        Logger::Info( "%s: C++17", __func__ );
        break;
    case 202002L:
        Logger::Info( "%s: C++20", __func__ );
        break;
    default:
        Logger::Info( "%s: Unknown C++ standard: %ld", __func__, __cplusplus );
        break;
    }
}

int main()
{
    printCppStandard();
    return 0;
}
