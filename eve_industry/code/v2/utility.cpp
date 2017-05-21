/// \file
/// File Name:                      utility.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engieers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or utility.h for documentation.

#include <locale>
#include <string>
#include <vector>
#include "utility.h"

std::string strip_and_upper(const char* input_string)
{
    
    if (input_string == NULL)
        return "";
    
    std::string output_string;
    for (const char* s = input_string; *s != '\0'; s++)
    {
        char c = *s;
        if (isalnum(c) != 0)
            output_string += c;
    }
    
    // Convert input string to uppercase
    for (unsigned ix = 0, bound = output_string.length(); ix < bound; ix++)
        output_string[ix] = toupper(output_string[ix]);
    
    return output_string;
    
}

const json_omni_t* walk_struct
(
    const json_struct_t* node,
    const std::string& search_name,
    json_type_t search_type
){
    
    if (node == NULL)
        return NULL;
    
    unsigned num_members = node->names.size();
    for (unsigned ix = 0; ix < num_members; ix++)
    {
        if (node->values[ix]->get_type() == search_type && node->names[ix] == search_name)
            return node->values[ix];
    }
    
    // Desired member was not found
    return NULL;
    
}

std::string lowercase(const std::string& input)
{
    
    unsigned length = input.length();
    
    std::string ret;
    ret.reserve(length);
    
    for (unsigned ix = 0; ix < length; ix++)
        ret += tolower(input[ix]);
    
    return ret;
    
}

std::string read_file(std::ifstream& file)
{
    
    std::string ret;
    static const unsigned buffer_size = 1024;
    char buffer[buffer_size];
    
    while (true)
    {
        file.read(buffer, buffer_size);
        unsigned chars_read = file.gcount();
        if (chars_read == 0)
            return ret;
        ret.append(buffer, chars_read);
    }
    
    return ret;
    
}

