#include <iostream>

#include "args.h"
#include "https_get.h"

/// @brief Code execution starts here
int main(int argc, char** argv)
{
    
    // Parse command line arguments
    args::args_t args;
    try
    {
        args.parse(argc, argv);
    } catch (args::error_t error) {
        std::cerr << args::enum_to_string(error) << '\n';
        return -1;
    }
    
    return 0;
    
}

