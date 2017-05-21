/// \file
/// File Name:                      print_blueprint_profitability.cpp \n
/// Date created:                   Mon Jan 9 2017 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html or print_blueprint_profitability.h for documentation.

#include <string>
#include <vector>

#include "calculate_blueprint_profitability.h"

std::string print_blueprint_profitability(const std::vector<profitability_t>& profitability)
{
    
    unsigned num_elements = profitability.size();
    
    std::vector<std::string> rendered_profit_per_run;
    rendered_profit_per_run.reserve(num_elements);
    std::vector<std::string> rendered_isk_per_second;
    rendered_isk_per_second.reserve(num_elements);
    std::vector<std::string> rendered_runs_per_cycle;
    rendered_runs_per_cycle.reserve(num_elements);
    
    unsigned max_name_length = 4; // length("Name")
    unsigned max_profit_per_run_length = 14; // length("Profit Per Run")
    unsigned max_isk_per_second_length = 14; // length("ISK Per Second")
    unsigned max_runs_per_cycle_length = 14; // length("Runs Per Cycle")
    
    // Render statistics to strings
    for (unsigned ix = 0; ix < num_elements; ix++)
    {
        
        const profitability_t& cur_profitability = profitability[ix];
        
        // Name
        unsigned cur_length = cur_profitability.name.length();
        if (max_name_length < cur_length)
            max_name_length = cur_length;
        
        // Profit per run
        rendered_profit_per_run.emplace_back(std::to_string(cur_profitability.profit_per_run));
        cur_length = rendered_profit_per_run.back().length();
        if (max_profit_per_run_length < cur_length)
            max_profit_per_run_length = cur_length;
        
        // ISK per second
        rendered_isk_per_second.emplace_back(std::to_string(cur_profitability.isk_per_second));
        cur_length = rendered_isk_per_second.back().length();
        if (max_isk_per_second_length < cur_length)
            max_isk_per_second_length = cur_length;
        
        // Runs per cycle
        rendered_runs_per_cycle.emplace_back(std::to_string(cur_profitability.runs_per_cycle));
        cur_length = rendered_runs_per_cycle.back().length();
        if (max_runs_per_cycle_length < cur_length)
            max_runs_per_cycle_length = cur_length;
        
        
    }
    
    // Print statistic strings with padding to output
    std::string padding;
    std::string rendered_break_even_price;
    
    std::string ret = "Name, ";
    padding.assign(max_name_length - 4, ' ');
    ret += padding;
    ret += "Profit Per Run, ";
    padding.assign(max_profit_per_run_length - 14, ' ');
    ret += padding;
    ret += "ISK Per Second, ";
    padding.assign(max_isk_per_second_length - 14, ' ');
    ret += padding;
    ret += "Runs Per Cycle, ";
    padding.assign(max_runs_per_cycle_length - 14, ' ');
    ret += padding;
    ret += "Break Even Price\n";
    
    for (unsigned ix = 0; ix < num_elements; ix++)
    {
        
        // Name
        ret += '\"';
        ret += profitability[ix].name;
        ret += "\", ";
        padding.assign(max_name_length - profitability[ix].name.length(), ' ');
        ret += padding;
        
        // Profit per run
        ret += rendered_profit_per_run[ix];
        ret += ", ";
        padding.assign(max_profit_per_run_length - rendered_profit_per_run[ix].length(), ' ');
        ret += padding;
        
        // ISK per second
        ret += rendered_isk_per_second[ix];
        ret += ", ";
        padding.assign(max_isk_per_second_length - rendered_isk_per_second[ix].length(), ' ');
        ret += padding;
        
        // Runs per cycle
        ret += rendered_runs_per_cycle[ix];
        ret += ", ";
        padding.assign(max_runs_per_cycle_length - rendered_runs_per_cycle[ix].length(), ' ');
        ret += padding;
        
        // Break even price
        rendered_break_even_price = std::to_string(profitability[ix].break_even_price);
        ret += rendered_break_even_price;
        ret += '\n';
        
    }
    
    return ret;
    
}

