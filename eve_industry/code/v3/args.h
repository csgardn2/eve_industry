/// \file
/// File Name:                      args.h \n
/// Date created:                   Sun April 2 2017 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.

#ifndef HEADER_GUARD_ARGS
#define HEADER_GUARD_ARGS

enum class arg_error_t
{
    SUCCESS,
    NUM_ENUMS
};

/// \brief Parses argc and argv from main and does some basic validation
/// of the arguments.
class args_t
{
    
    public:
        
        /// \brief Default constructor
        args_t() = default;
        
        /// \brief Calls the \ref parse function during construction
        args_t(int argc, char const* const* argv);
        
        arg_error_t parse()
        
    protected:
        
};

#endif // header guard
