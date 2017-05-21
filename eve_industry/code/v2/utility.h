/// \file
/// File Name:                      utility.h \n
/// Date created:                   Wed Dec 21 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 14.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_UTILITY
#define HEADER_GUARD_UTILITY

#include <fstream>
#include <string>
#include "json.h"

/// \brief Returns the largest of two numbers
inline unsigned max(unsigned x, unsigned y)
{
    if (x > y)
        return x;
    else
        return y;
}

/// \brief Returns the smallest of two numbers
inline unsigned min(unsigned x, unsigned y)
{
    if (x < y)
        return x;
    else
        return y;
}

/// \brief Remove all spaces and nonalphanumeric characters then convert the
/// remaining characters to uppercase.
std::string strip_and_upper(const char* input_string);

/// \brief Walk through a specific JSON struct non-recursivly, looking for an
/// element with a specific name and type
/// \return A pointer to that item if it is found or NULL if it is not found
const json_omni_t* walk_struct
(
    
    /// [in] The JSON structure to search
    const json_struct_t* node,
    
    /// [in] The name of the element that you want to find
    const std::string& search_name,
    
    /// [in] The type of the element that you want to find
    json_type_t search_type
    
);

/// \brief Convert a string to lowercase
std::string lowercase(const std::string& input);

/// \brief Read the entire contents of a file into RAM
std::string read_file(std::ifstream& file);

#endif // header guard

