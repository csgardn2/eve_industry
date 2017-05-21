/// \file
/// File Name:                      main.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or documentation.

#include <iostream>
#include <string>
#include <vector>

#include "arguments.h"
#include "blueprint.h"
#include "calculate_blueprint_profitability.h"
#include "fetch_item_ids.h"
#include "fetch_orders.h"
#include "print_blueprint_profitability.h"
#include "read_blueprints_file.h"
#include "utility.h"

/// See arguments.h for command line usage
int main(int argc, char** argv)
{
    
    // Pre-process and validate command line arguments and open required files.
    // Prints to stderr if problems occur.
    arguments_t arguments(argc, argv);
    if (!arguments.valid)
        return -1;
    
    // Gather item IDs
    std::vector<item_id_pair_t> item_ids = fetch_item_ids_through_cache
    (
        arguments.item_id_cache_file_name,
        arguments.crest_root
    );
    std::cout << "Found " << item_ids.size() << " item_ids\n";
    if (item_ids.size() == 0)
        return -1;
    
    // Fetch market data
    std::cout << "Order cache = " << arguments.order_cache_file_name << '\n';
    std::vector<order_t> orders = fetch_orders_through_cache
    (
        arguments.order_cache_file_name,
        arguments.crest_root,
        arguments.region,
        arguments.station_id
    );
    std::cout << "Found " << orders.size() << " orders\n";
    if (orders.size() == 0)
        return -1;
    
    // Parse blueprint file
    std::vector<blueprint_t> blueprints = read_blueprints_file(arguments.blueprints_file);
    std::cout << "Found " << blueprints.size() << " blueprints\n";
    
    // Calculate each blueprint's profitability
    std::vector<profitability_t> profitability = calculate_blueprint_profitability
    (
        item_ids,
        orders,
        blueprints,
        arguments.cycle_time
    );    
    
    // Write the final output to file
    arguments.profitability_file << print_blueprint_profitability(profitability);
    std::cout << "Wrote " << profitability.size() << " outputs to final report\n";
    
    return 0;
    
}

