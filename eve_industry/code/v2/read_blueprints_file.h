/// \file
/// File Name:                      read_blueprints_file.h \n
/// Date created:                   Wed Dec 21 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 14.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_PARSE_BLUEPRINT_LIST
#define HEADER_GUARD_PARSE_BLUEPRINT_LIST

#include <vector>
#include "blueprint.h"

/// \brief Walk through a JSON file containing an array of structures which
/// describe the blueprints owned by a character.
std::vector<blueprint_t> read_blueprints_file(std::ifstream& blueprints_file);

#endif // header guard

