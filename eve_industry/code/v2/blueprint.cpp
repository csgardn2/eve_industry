/// \file
/// File Name:                      blueprint.cpp \n
/// Date created:                   Sun Jan 8 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or blueprint.h for documentation.

#include "blueprint.h"

#include <string>

const char* availability_to_string(availability_t availability)
{
    static const unsigned num_enums = 4;
    static const char* names[num_enums] = 
    {
        "AVAILABLE",
        "MANUFACTURING",
        "SELLING",
        "UNAVAILABLE"
    };
    
    
    unsigned ix = unsigned(availability);
    
    if (ix >= num_enums)
        return "invalid";
    else
        return names[ix];
        
}

std::string blueprint_t::print() const
{
    
    std::string ret;
    
    ret += "name = \"";
    ret += this->name;
    ret += "\"\navailability = ";
    ret += availability_to_string(this->availability);
    ret += "\ntime = ";
    ret += std::to_string(this->time);
    ret += "\nvolume = ";
    ret += std::to_string(this->volume);
    ret += "\noutput_quantity = ";
    ret += std::to_string(this->output_quantity);
    ret += "\nme = ";
    ret += std::to_string(this->me);
    ret += "\nte = ";
    ret += std::to_string(this->te);
    ret += "\ninput_materials = {";
    
    unsigned num_input_materials = this->input_materials.size();
    for (unsigned ix = 0, bound = num_input_materials - 1; ix < bound; ix++)
    {
        ret += this->input_materials[ix];
        ret += ", ";
    }
    if (num_input_materials != 0)
        ret += this->input_materials.back();
    ret += "}\ninput_quantities = {";
    
    unsigned num_input_quantities = this->input_quantities.size();
    for (unsigned ix = 0, bound = num_input_quantities - 1; ix < bound; ix++)
    {
        ret += std::to_string(this->input_quantities[ix]);
        ret += ", ";
    }
    if (num_input_quantities != 0)
        ret += std::to_string(this->input_quantities.back());
    ret += "}\n";
    
    return ret;
    
}

