/// \file
/// File Name:                      print_blueprint_profitability.h \n
/// Date created:                   Mon Jan 9 2017 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_PRINT_BLUEPRINT_PROFITABILITY
#define HEADER_GUARD_PRINT_BLUEPRINT_PROFITABILITY

#include <string>
#include <vector>

#include "calculate_blueprint_profitability.h"

/// \brief Render a human readable report of the profitability of each blueprint
std::string print_blueprint_profitability(const std::vector<profitability_t>& profitability);

#endif // header guard

