/// \file
/// File Name:                      calculate_blueprint_profitability.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or calculate_blueprint_profitability.h for documentation.

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "blueprint.h"
#include "calculate_blueprint_profitability.h"
#include "fetch_item_ids.h"
#include "fetch_orders.h"
#include "utility.h"

/// Function used to sort the output list
bool ordering_function(const profitability_t& first, const profitability_t& second)
{
    return first.isk_per_second > second.isk_per_second;
}

std::vector<profitability_t> calculate_blueprint_profitability
(
    const std::vector<item_id_pair_t>& item_ids,
    const std::vector<order_t>& orders,
    const std::vector<blueprint_t>& blueprints,
    float cycle_time
){
    
    // Create a map of item IDs to prices
    unsigned num_orders = orders.size();
    std::unordered_map<unsigned, float> id_price_map;
    for (unsigned ix = 0; ix < num_orders; ix++)
    {
        
        const order_t& cur_order = orders[ix];
        
        if (!cur_order.is_sell)
            continue;
        
        const std::unordered_map<unsigned, float>::const_iterator not_found = id_price_map.end();
        std::unordered_map<unsigned, float>::iterator match = id_price_map.find(cur_order.item_id);
        
        if (match == not_found)
        {
            // No price set for this item yet.  Add it
            id_price_map.emplace(cur_order.item_id, cur_order.price);
            continue;
        } else if (match->second > cur_order.price && match->second > 0.01) {
            // The current order price is lower than any previous order.  Replace
            match->second = cur_order.price;
            continue;
        }
        
    }
    
    // Create a map of lowercase item names to IDs
    unsigned num_items = item_ids.size();
    std::unordered_map<unsigned, std::string> id_name_map;
    for (unsigned ix = 0; ix < num_items; ix++)
    {
        const item_id_pair_t& cur_item = item_ids[ix];
        id_name_map.emplace(cur_item.id, lowercase(cur_item.name));
    }
    
    // Create a map of lowercase item names to prices
    std::unordered_map<std::string, float> name_price_map;
    for (std::pair<unsigned, std::string> match_1 : id_name_map)
    {
        
        unsigned cur_id = match_1.first;
        
        std::unordered_map<unsigned, float>::const_iterator not_found = id_price_map.end();
        std::unordered_map<unsigned, float>::const_iterator match_2 = id_price_map.find(cur_id);
        
        if (match_2 == not_found)
            // Price was not found for this particular item.
            continue;
        
        name_price_map.emplace(match_1.second, match_2->second);
        
    }
    
    // Calculate the profitability of each blueprint
    unsigned num_blueprints = blueprints.size();
    std::vector<profitability_t> ret;
    for (unsigned ix = 0; ix < num_blueprints; ix++)
    {
        
        const blueprint_t& cur_blueprint = blueprints[ix];
        
        // Only use blueprints that are available
        if (cur_blueprint.availability != availability_t::AVAILABLE)
            continue;
        
        std::unordered_map<std::string, float>::const_iterator match;
        std::unordered_map<std::string, float>::const_iterator not_found = name_price_map.end();
        
        // Calculate the value of the output product
        match = name_price_map.find(lowercase(cur_blueprint.name));
        if (match == not_found)
        {
            std::cerr
                << "Warning.  Could not find price of output item \""
                << cur_blueprint.name
                << "\".  Skipping blueprint.\n";
            continue;
        }
        float output_value = match->second;
        
        // Calculate to total value of all input materials
        unsigned num_input_materials = cur_blueprint.input_materials.size();
        bool error = false;
        float total_input_value = 0.0f;
        
        for (unsigned iy = 0; iy < num_input_materials; iy++)
        {
            match = name_price_map.find(lowercase(cur_blueprint.input_materials[iy]));
            if (match == not_found)
            {
                std::cerr
                    << "Warning.  Could not find price of input item \""
                    << cur_blueprint.input_materials[iy]
                    << "\".  Skipping blueprint.\n";
                error = true;
                break;
            }
            
            total_input_value += match->second * cur_blueprint.input_quantities[iy];
        }
        if (error)
            continue;
        
        if (total_input_value == 0)
        {
            std::cerr
                << "Warning.  Input material value of \""
                << cur_blueprint.input_quantities[ix]
                << "\" for \""
                << cur_blueprint.name
                << "\" blueprint was zero.  Skipping\n";
            continue;
        }
        
        if (cur_blueprint.time < 0.01f)
        {
            std::cerr
                << "Warning.  Blueprint \""
                << cur_blueprint.name
                << "\" had manufacturing time of 0.  Skipping\n";
            continue;
        }
        
        float profit_per_run = output_value / total_input_value;
        
        ret.emplace_back
        (
            cur_blueprint.name,
            profit_per_run,
            (output_value - total_input_value) / cur_blueprint.time,
            cycle_time / cur_blueprint.time,
            total_input_value
        );
        
    }
    
    // Sort the profitability list by ISK / second
    unsigned num_outputs = ret.size();
    std::sort(ret.data(), ret.data() + num_outputs - 1, ordering_function);
    
    return ret;
    
}

