#include <iostream>
#include "validator.h"

/// @brief Calls other validator functions.
int main()
{
    
    if (validate_args())
        std::cout << "Successfully validated args_t class.\n";
    
    return 0;
    
}

