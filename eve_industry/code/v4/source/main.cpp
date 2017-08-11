#include <iostream>

#include "https_get.h"

/// @brief Code execution starts here
int main()
{
    
    std::cout << https_get("www.example.com") << '\n';
    
    return 0;
    
}

