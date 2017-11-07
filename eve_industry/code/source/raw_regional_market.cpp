/// @file raw_regional_market.cpp
/// @brief Implementation of @ref raw_regional_market_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 3 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "error.h"
#include "https_get.h"
#include "json.h"
#include "raw_order.h"
#include "raw_regional_market.h"

void raw_regional_market_t::fetch(uint64_t region_id)
{
    
    this->clear();
    this->region_id_ = region_id;
    
    // Automatically de-allocate memory when this function returns or throws
    // an error.
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    
    std::string progress_message;
    
    std::string query_prefix("https://esi.tech.ccp.is/latest/markets/");
    query_prefix += std::to_string(region_id);
    query_prefix += "/orders/?datasource=tranquility&order_type=all&page=";
    
    // There are multiple pages of item IDs.  The last page is signified with
    // and empty array.
    unsigned page = 1;
    while (true)
    {
        
        // Print progress message, erasing previous one if it existed
        if (this->debug_mode_.verbose())
        {
            for (unsigned ix = 0, bound = progress_message.length(); ix < bound; ix++)
                std::cout << '\b';
            progress_message = "Page ";
            progress_message += std::to_string(page);
            std::cout << progress_message << std::flush;
        }
        
        // Prepare a request URL
        std::string query(query_prefix);
        query += std::to_string(page);
        page++;
        
        // Attempt to pull data from the EVE API
        std::string payload = https_get(query);
        
        // Attempt to decode received data
        Json::Value json_regional_market_page;
        std::string error_message;
        if (!reader->parse(payload.data(), payload.data() + payload.size(), &json_regional_market_page, &error_message))
        {
            std::string message("Error.  Tried to decode JSON data from\"");
            message += query;
            message += "\" but it failed with message \"";
            message += error_message;
            message += "\".\n";
            throw error_message_t(error_code_t::EVE_SUCKS, message);
        }
        
        // Append fetched data to internal storage
        unsigned old_size = this->orders_.size();
        unsigned new_size = old_size + json_regional_market_page.size();
        this->orders_.resize(new_size);
        for
        (
            unsigned read_ix = 0, write_ix = old_size;
            write_ix < new_size;
            read_ix++, write_ix++
        ){
            
            // Accumulate fields for each order in this structure
            raw_order_t new_order;
            
            const Json::Value& json_cur_element = json_regional_market_page[read_ix];
            if (!json_cur_element.isObject())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" was not of type \"object\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            
            const Json::Value& json_cur_price = json_cur_element["price"];
            if (!json_cur_price.isNumeric())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" did not contain a valid float named \"price\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            new_order.price(json_cur_price.asFloat());
            
            const Json::Value& json_cur_item_id = json_cur_element["type_id"];
            if (!json_cur_item_id.isUInt64())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" did not contain a valid unsigned named \"type_id\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            new_order.item_id(json_cur_item_id.asUInt64());
            
            const Json::Value& json_cur_station_id = json_cur_element["location_id"];
            if (!json_cur_station_id.isUInt64())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" did not contain a valid unsigned named \"location_id\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            new_order.station_id(json_cur_station_id.asUInt64());
            
            const Json::Value& json_cur_order_type = json_cur_element["is_buy_order"];
            if (!json_cur_order_type.isBool())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" did not contain a valid bool named \"is_buy_order\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            if (json_cur_order_type.asBool())
                new_order.order_type(order_type_t::BUY);
            else
                new_order.order_type(order_type_t::SELL);
            
            this->orders_[write_ix] = new_order;
            
        }
        
        if (json_regional_market_page.empty())
        {
            if (this->debug_mode_.verbose())
                std::cout << '\n';
            return;
        }
        
    }

}

void raw_regional_market_t::read_from_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding raw_regional_market_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding raw_regional_market object.\n");
    this->read_from_buffer(std::string_view(buffer));
    
}

void raw_regional_market_t::read_from_buffer(std::string_view buffer)
{
    
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    
    Json::Value json_root;
    std::string error_message;
    bool success = reader->parse(buffer.begin(), buffer.end(), &json_root, &error_message);
    delete reader;
    if (!success)
        Json::throwRuntimeError(error_message);
    
    // Now that the JSON syntax is parsed, extract the stat_list specific
    // data.
    this->read_from_json(json_root);
    
}

void raw_regional_market_t::read_from_json(const Json::Value& json_root)
{
    
    this->orders_.clear();
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of raw_regional_market is not of type \"object\".\n");
    
    // Parse region_id
    const Json::Value& json_region_id = json_root["region_id"];
    if (!json_region_id.isUInt64())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <raw_regional_market>/region_id was not found or not of type \"unsigned integer\".\n");
    this->region_id_ = json_region_id.asUInt64();
    
    // Parse orders
    const Json::Value& json_orders = json_root["orders"];
    if (!json_orders.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <raw_regional_market>/orders was not found or not of type \"object\".\n");
    
    // Re-allocate storage
    this->orders_.reserve(json_orders.size());
    
    // Parse each order in the input array
    for (const Json::Value& json_cur_order : json_orders)
    {
        raw_order_t new_order;
        new_order.read_from_json(json_cur_order);
        this->orders_.emplace_back(std::move(new_order));
    }
    
}

void raw_regional_market_t::write_to_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding raw_regional_market object.");
}

void raw_regional_market_t::write_to_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_2(indent_start + 2 * spaces_per_tab, ' ');
    std::string_view indent_1(indent_2.data(), indent_start + spaces_per_tab);
    std::string_view indent_0(indent_2.data(), indent_start);
    
    buffer += "{\n";
    
    // Encode Region ID
    buffer += indent_1;
    buffer += "\"region_id\": ";
    buffer += std::to_string(this->region_id_);
    buffer += ",\n";
    
    // Call encoder for each order.
    buffer += indent_1;
    unsigned num_orders = this->orders_.size();
    if (num_orders == 0)
    {
        buffer += "\"orders\": []\n";
    } else {
        
        buffer += "\"orders\": [\n";
        
        buffer += indent_2;
        for (unsigned ix = 0, last_ix = num_orders - 1; ix <= last_ix; ix++)
        {
            
            this->orders_[ix].write_to_buffer(buffer, indent_start + 2 * spaces_per_tab, spaces_per_tab);
            
            if (ix == last_ix)
                buffer += '\n';
            else
                buffer += ", ";
            
        }
        
        buffer += indent_1;
        buffer += "]\n";
        
    }
    
    buffer += indent_0;
    buffer += '}';
    
}

std::istream& operator>>(std::istream& stream, raw_regional_market_t& destination)
{
    try
    {
        destination.read_from_file(stream);
    } catch (error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

