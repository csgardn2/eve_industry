#include <iostream>

#include "args.h"
#include "error.h"
#include "yaml-cpp/yaml.h"

int main(int argc, char** argv)
{
    
    try
    {
        
        // Parse command line arguments
        args_t args;
        args.parse(argc, argv);
        
        // Parse input file
        YAML::Node lard = YAML::LoadFile(args.ccp_yaml_in());
        
        
        
    } catch (const error_message_t& error) {
        
        std::cout << error.message() << '\n';
        
    }
    
    return 0;
    
}

