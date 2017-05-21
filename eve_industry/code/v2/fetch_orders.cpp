/// \file
/// File Name:                      fetch_order_list.cpp \n
/// Date created:                   Sat Jan 7 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or fetch_order_list.h for documentation.

#include <iostream>
#include <locale>
#include <string>
#include <vector>

#include "fetch_orders.h"
#include "ftps_get.h"
#include "json.h"
#include "utility.h"

std::vector<order_t> fetch_orders
(
    const std::string& eve_api_root,
    const std::string& region,
    unsigned station_id
){
    
    std::vector<order_t> ret;
    
    // JSON Page 1 - The eve root
    std::string raw_json_data = ftps_get(eve_api_root);
    json_root_t structured_json_data;
    if (!structured_json_data.parse(raw_json_data))
    {
        std::cerr
            << "Error.  Could not parse CREST root \""
            << eve_api_root
            << "\" while fetching market orders\n";
        return ret;
    }
    
    // Page 2 - Page with all the market regions
    const json_struct_t* regions = (json_struct_t*)walk_struct(structured_json_data.start_node, "regions", json_type_t::STRUCT);
    if (regions == NULL)
    {
        std::cerr << "Error.  Failed to walk from CREST root to \"regions\" while fetching market orders\n";
        return ret;
    }
    const json_string_t* regions_href = (json_string_t*)walk_struct(regions, "href", json_type_t::STRING);
    if (regions_href == NULL)
    {
        std::cerr << "Error.  Failed to walk from \"regions\" to \"href\" fetching market orders\n";
        return ret;
    }
    raw_json_data = ftps_get(regions_href->value);
    if (!structured_json_data.parse(raw_json_data))
    {
        std::cerr
            << "Error.  Could not parse regions page \""
            << regions_href->value
            << "\" while fetching market orders\n";
        return ret;
    }
    
    // Page 3 - The page for the specific region you want
    const json_array_t* regions_items = (json_array_t*)walk_struct(structured_json_data.start_node, "items", json_type_t::ARRAY);
    if (regions_items == NULL)
    {
        std::cerr << "Error.  Failed to walk from \"regions\" to \"items\" fetching market orders\n";
        return ret;
    }
    std::string desired_region = lowercase(region);
    std::string cur_region;
    const json_string_t* located_href;
    bool found_region = false;
    for (unsigned ix = 0, bound = regions_items->values.size(); ix < bound; ix++)
    {
        
        // Walk the array of all the region names until we find the one we want
        
        if (regions_items->values[ix]->get_type() != json_type_t::STRUCT)
            continue;
        const json_struct_t* cur_item = (json_struct_t*)(regions_items->values[ix]);
        
        const json_string_t* cur_region_name = (json_string_t*)walk_struct(cur_item, "name", json_type_t::STRING);
        if (cur_region_name == NULL)
            continue;
        
        cur_region = lowercase(cur_region_name->value);
        if (cur_region == desired_region)
        {
            
            const json_string_t* cur_region_href = (json_string_t*)walk_struct(cur_item, "href", json_type_t::STRING);
            if (cur_region_href == NULL)
                continue;
            
            found_region = true;
            located_href = cur_region_href;
            break;
            
        }
        
    }
    if (!found_region)
    {
        std::cerr
            << "Error.  Could not find a region named \""
            << region
            << "\" while fetching market orders\n";
        return ret;
    }
    raw_json_data = ftps_get(located_href->value);
    if (!structured_json_data.parse(raw_json_data))
    {
        std::cerr
            << "Error.  Could not parse \""
            << region
            << "\" at \""
            << located_href->value
            << "\" while fetching market orders\n";
        return ret;
    }
    
    // Page 4 - marketOrdersAll
    const json_struct_t* market_orders_all= (json_struct_t*)walk_struct
    (
        structured_json_data.start_node,
        "marketOrdersAll",
        json_type_t::STRUCT
    );
    if (market_orders_all == NULL)
    {
        std::cerr
            << "Error.  Failed to walk from \""
            << region
            << "\" to \"marketOrdersAll\" while fetching market orders\n";
        return ret;
    }
    const json_string_t* market_orders_all_href = (json_string_t*)walk_struct
    (
        market_orders_all,
        "href",
        json_type_t::STRING
    );
    if (market_orders_all_href == NULL)
    {
        std::cerr << "Error.  Failed to walk from \"marketOrdersAll\" to \"href\" while fetching market orders\n";
        return ret;
    }
    
    unsigned page = 1;
    const json_string_t* cur_href = market_orders_all_href;
    while (true)
    {
        
        std::cout
            << "Fetching page "
            << page
            << " of market data from \""
            << cur_href->value
            << "\"\n";
        
        // Fetch and pre-process a page of market data
        raw_json_data = ftps_get(cur_href->value);
        if(!structured_json_data.parse(raw_json_data))
        {
            std::cerr
                << "Error.  Failed to parse market data page "
                << page
                << " from \""
                << cur_href->value
                << "\".\n";
            return ret;
        }
        
        // Walk through the market data and extract price information
        const json_array_t* items = (json_array_t*)walk_struct
        (
            structured_json_data.start_node,
            "items",
            json_type_t::ARRAY
        );
        if (items == NULL)
        {
            std::cerr
                << "Error.  Could not find \"items\" array in market data page "
                << page
                << " from \""
                << cur_href->value
                << "\".\n";
            return ret;
        }
        unsigned num_items = items->values.size();
        for (unsigned ix = 0; ix < num_items; ix++)
        {
            
            // Select a single order from the current page of market data
            if (items->values[ix]->get_type() != json_type_t::STRUCT)
                continue;
            const json_struct_t* cur_order = (json_struct_t*)(items->values[ix]);
            
            order_t accumulator;
            
            // Extract station ID of the order
            json_int_t* cur_station_id = (json_int_t*)walk_struct(cur_order, "stationID", json_type_t::INT);
            if (cur_station_id == NULL)
                continue;
            unsigned extracted_station_id = cur_station_id->value;
            if (station_id != 0 && extracted_station_id != station_id)
                continue;
            accumulator.station_id = extracted_station_id;
            
            // Extract if the order is for buying/selling
            json_bool_t* buy = (json_bool_t*)walk_struct(cur_order, "buy", json_type_t::BOOLEAN);
            if (buy == NULL)
                continue;
            accumulator.is_sell = !buy->value;
            
            // Extract the price of the current order
            json_float_t* price_float = (json_float_t*)walk_struct(cur_order, "price", json_type_t::FLOAT);
            if (price_float == NULL)
            {
                json_int_t* price_int = (json_int_t*)walk_struct(cur_order, "price", json_type_t::INT);
                if (price_int == NULL)
                    continue;
                else
                    accumulator.price = float(price_int->value);
            } else {
                accumulator.price = price_float->value;
            }
            
            // Extract the quantity of the current order
            json_int_t* quantity = (json_int_t*)walk_struct(cur_order, "volume", json_type_t::INT);
            if (quantity == NULL)
                continue;
            accumulator.quantity = quantity->value;
            
            // Extract item ID of the current order
            json_int_t* item_id = (json_int_t*)walk_struct(cur_order, "type", json_type_t::INT);
            if (item_id == NULL)
                continue;
            accumulator.item_id = item_id->value;
            
            // All fields extracted successfully.  Commit order to returned list.
            ret.push_back(accumulator);
            
        }
        
        // See if there's another page of market data then navigate to it
        const json_struct_t* next = (json_struct_t*)walk_struct
        (
            structured_json_data.start_node,
            "next",
            json_type_t::STRUCT
        );
        
        // Successfully return when all pages are depleted
        if (next == NULL)
            return ret;
        
        cur_href = (json_string_t*)walk_struct(next, "href", json_type_t::STRING);
        if (cur_href == NULL)
        {
            std::cerr << "Error.  Failed to walk from \"next\" to \"href\" while fetching market orders\n";
            return ret;
        }
        
        page++;
        
    }
    
}

/// \brief Parse through a CSV file whose elements are 
///     is_sell, price, quantity, item_id, station_id
///     is_sell, price, quantity, item_id, station_id
/// until the file is depleted or an error occurs.  The orders are then returned.
std::vector<order_t> read_order_cache(std::ifstream& cache_reader)
{
    
    std::vector<order_t> ret;
    while (true)
    {
        
        char comma;
        order_t accumulator;
        
        cache_reader >> accumulator.is_sell;
        cache_reader >> comma;
        cache_reader >> accumulator.price;
        cache_reader >> comma;
        cache_reader >> accumulator.quantity;
        cache_reader >> comma;
        cache_reader >> accumulator.item_id;
        cache_reader >> comma;
        cache_reader >> accumulator.station_id;
        
        if (!cache_reader.good())
            return ret;
        
        ret.push_back(accumulator);
        
    }
    
}

std::vector<order_t> fetch_orders_through_cache
(
    const std::string& order_cache_file_name,
    const std::string& eve_api_root,
    const std::string& region,
    unsigned station_id
){
    
    std::vector<order_t> ret;
    
    // Try to open the cache for reading
    std::ifstream cache_reader(order_cache_file_name);
    if (cache_reader.good())
    {
        
        // Cache was opened.  Read it and return it.
        std::vector<order_t> ret = read_order_cache(cache_reader);
        return ret;
        
    } else {
        
        std::cout
            << "Could not find order cache.  Downloading it from \""
            << eve_api_root
            << "\".  This could take a while.\n";
        
        // Cache doesn't exist.  Pull data from CREST
        std::vector<order_t> ret = fetch_orders(eve_api_root, region, station_id);
        std::cout
            << "Finished download.  Writing to cache \""
            << order_cache_file_name
            << "\".\n";
        
        // Write IDs to cache
        if (cache_reader.is_open())
            cache_reader.close();
        std::ofstream cache_writer(order_cache_file_name);
        
        unsigned num_ids = ret.size();
        for (unsigned ix = 0; ix < num_ids; ix++)
        {
            const order_t& cur_order = ret[ix];
            cache_writer
                << cur_order.is_sell << ", "
                << cur_order.price << ", "
                << cur_order.quantity << ", "
                << cur_order.item_id << ", "
                << cur_order.station_id << '\n';
        }
        
        return ret;
        
    }
    
}


