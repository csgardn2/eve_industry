/// @file regional_market_orders.cpp
/// @brief Implementation of @ref regional_market_orders_t class
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
#include "regional_market_orders.h"

void regional_market_orders_t::fetch(uint64_t region_id)
{
    
    this->clear();
    
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
        for (unsigned ix = 0, bound = progress_message.length(); ix < bound; ix++)
            std::cout << '\b';
        progress_message = "Page ";
        progress_message += std::to_string(page);
        std::cout << progress_message << std::flush;
        
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
        unsigned old_size = this->size();
        unsigned new_size = old_size + json_regional_market_page.size();
        this->resize(new_size);
        for
        (
            unsigned read_ix = 0, write_ix = old_size;
            write_ix < new_size;
            read_ix++, write_ix++
        ){
            
            order_t new_order;
            
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
            
            // Conor, you need to change all EvE related ints to long ints.
            
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
            
            (*this)[write_ix] = new_order;
            
        }
        /*
        std::cout
            << "Page = "
            << std::to_string(page)
            << " Query = "
            << query
            << " "
            << std::to_string(json_regional_market_page.size())
            << "\n"
            << progress_message
            << this->back() << '\n';
        */
        if (json_regional_market_page.empty())
        {
            std::cout << '\n';
            return;
        }
        
    }

}

void regional_market_orders_t::read_from_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED);
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED);
    this->read_from_buffer(std::string_view(buffer));
    
}

void regional_market_orders_t::read_from_buffer(std::string_view buffer)
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

void regional_market_orders_t::read_from_json(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of regional_market_orders is not of type \"array\".\n");
    
    // Re-allocate storage
    this->clear();
    this->reserve(json_root.size());
    
    // Parse each order in the input array
    for (const Json::Value& json_cur_order : json_root)
    {
        order_t new_order;
        new_order.read_from_json(json_cur_order);
        this->emplace_back(std::move(new_order));
    }
    
}

void regional_market_orders_t::write_to_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED);
}

void regional_market_orders_t::write_to_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    // Pretty formatting for empty array
    if (this->empty())
    {
        buffer += "[]";
        return;
    }
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    buffer += indent_1;
    
    // Encode each object in this array.
    for (unsigned ix = 0, last_ix = this->size() - 1; ix <= last_ix; ix++)
    {
        
        // Encode an order
        (*this)[ix].write_to_buffer(buffer, indent_start + spaces_per_tab, spaces_per_tab);
        
        // Prepare for next order
        if (ix == last_ix)
            buffer += '\n';
        else
            buffer += ", ";
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += ']';
    
}

std::istream& operator>>(std::istream& stream, regional_market_orders_t& destination)
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

