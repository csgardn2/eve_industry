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
#include <thread>
#include <vector>

#include "debug_mode.h"
#include "error.h"
#include "https_get.h"
#include "json.h"
#include "page_fetcher.h"
#include "raw_order.h"
#include "raw_regional_market.h"
/*
/// @brief Starts a loop which fetches pages of market data.
void fetch_worker
(
    /// [in] Integral uniquifier of the region to fetch data from
    uint64_t region_id,
    /// [out] All orders for all threads are stored here
    std::vector<raw_order_t>* outputs_,
    /// [in, out] Starts at 1.  Indicates the page which has been
    /// most recently fetched.
    std::atomic<unsigned>* previous_page,
    /// [in, out] This is initialized to false and is set
    /// to true when the last page has been fetched and processed.
    /// All threads will terminate when this is set to true.
    std::atomic<bool>* kill_switch,
    /// [in, out] Guards writes and re-allocations to the outputs vector.
    std::mutex* output_lock,
    /// [out] Used to track which market pages were fetched successfully.
    std::set<unsigned>* successfully_fetched_pages,
    /// [in] Used to control verbosity
    const debug_mode_t* debug_mode
){
    
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
    while (*kill_switch == false)
    {
        
        unsigned page = (*previous_page)++;
        
        // Print progress message, erasing previous one if it existed
        if (debug_mode->verbose())
            std::cout << "Page " << page << '\n';
        
        // Prepare a request URL
        std::string query(query_prefix);
        query += std::to_string(page);
        
        // Attempt to pull data from the EVE API
        std::string payload;
        unsigned num_attempts = 0;
        while (true)
        {
            
            static const unsigned max_attempts = 3;
            
            try
            {
                payload = https_get(query);
            } catch (error_message_t&) {
                
                if (debug_mode->verbose())
                    std::cout
                        << "Warning.  Failed to fetch market page "
                        << page
                        << " attempt "
                        << num_attempts
                        << " / "
                        << max_attempts
                        << '\n';
                
                // Either there was an error or we fetched past
                // the last market page.  In either case, it's time
                // for all worker threads to retire.
                num_attempts++;
                if (num_attempts > max_attempts)
                {
                    *kill_switch = true;
                    return;
                }
                
                // Failed HTTPS GET attempt, try again
                continue;
                
            }
            
            // Fetched page successfully
            break;
            
        }
        
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
        
        // EvE will send an empty market page to signify the end of data.
        if (json_regional_market_page.empty())
        {
            *kill_switch = true;
            return;
        }
        
        // Append fetched data to internal storage
        output_lock->lock();
        unsigned old_size = outputs_->size();
        unsigned new_size = old_size + json_regional_market_page.size();
        outputs_->resize(new_size);
        
        for
        (
            unsigned read_ix = 0, write_ix = old_size;
            write_ix < new_size;
            read_ix++, write_ix++
        ){
            
            // Accumulate fields for each order in this structure
            raw_order_t new_order;
            
            const Json::Value& json_cur_order = json_regional_market_page[read_ix];
            if (!json_cur_order.isObject())
            {
                std::string message("Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" was not of type \"object\".\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            
            const Json::Value& json_cur_price = json_cur_order["price"];
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
            
            const Json::Value& json_cur_item_id = json_cur_order["type_id"];
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
            
            const Json::Value& json_cur_station_id = json_cur_order["location_id"];
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
            
            const Json::Value& json_cur_order_type = json_cur_order["is_buy_order"];
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
            
            (*outputs_)[write_ix] = new_order;
            
        }
        
        successfully_fetched_pages->emplace(page);
        output_lock->unlock();
        page++;
        
    }
    
}
*/

void raw_regional_market_t::fetch(uint64_t region_id)
{
    
    this->region_id_ = region_id;
    
    page_fetcher_t page_fetcher;
    page_fetcher.debug_mode(this->debug_mode());
    
    std::string query_prefix("https://esi.tech.ccp.is/latest/markets/");
    query_prefix += std::to_string(region_id);
    query_prefix += "/orders/?datasource=tranquility&order_type=all";
    page_fetcher.base_url(query_prefix);
    
    std::vector<Json::Value> payloads = page_fetcher.parallel_fetch(std::thread::hardware_concurrency());
    
    this->decode_from_payloads(payloads);
    
}

void raw_regional_market_t::decode_from_payloads(const std::vector<Json::Value>& payloads)
{
    
    this->clear();
    
    // Calcuate the total number of market orders across all fetched payoads
    unsigned num_orders;
    for (const Json::Value& json_cur_payload : payloads)
    {
        if (!json_cur_payload.isArray())
            throw error_message_t(error_code_t::EVE_SUCKS, "Error.  Could not decode non-array market payload.\n");
        num_orders += json_cur_payload.size();
    }
    this->orders_.reserve(num_orders);
    
    // Step through each payload and select it for decoding
    for (const Json::Value& json_cur_payload : payloads)
    {
        
        // Step through each order and decode it
        for (const Json::Value& json_cur_order : json_cur_payload)
        {
            
            // Accumulate fields for each order in this structure
            raw_order_t new_order;
            
            if (!json_cur_order.isObject())
                throw error_message_t(error_code_t::EVE_SUCKS, "Element fetched from eve API was not of type \"object\".\n");
            
            const Json::Value& json_cur_price = json_cur_order["price"];
            if (!json_cur_price.isNumeric())
                throw error_message_t(error_code_t::EVE_SUCKS, "price fetched from eve API was not of type \"float\".\n");
            new_order.price(json_cur_price.asFloat());
            
            const Json::Value& json_cur_item_id = json_cur_order["type_id"];
            if (!json_cur_item_id.isUInt64())
                throw error_message_t(error_code_t::EVE_SUCKS, "type_id fetched from eve API was not of type \"unsigned integer\".\n");
            new_order.item_id(json_cur_item_id.asUInt64());
            
            const Json::Value& json_cur_station_id = json_cur_order["location_id"];
            if (!json_cur_station_id.isUInt64())
                throw error_message_t(error_code_t::EVE_SUCKS, "location_id fetched from eve API was not of type \"unsigned integer\".\n");
            new_order.station_id(json_cur_station_id.asUInt64());
            
            const Json::Value& json_cur_order_type = json_cur_order["is_buy_order"];
            if (!json_cur_order_type.isBool())
                throw error_message_t(error_code_t::EVE_SUCKS, "is_buy_order fetched from eve API was not of type \"bool\".\n");
            if (json_cur_order_type.asBool())
                new_order.order_type(order_type_t::BUY);
            else
                new_order.order_type(order_type_t::SELL);
            
            this->orders_.emplace_back(std::move(new_order));
            
        }
        
    }
    
}

void raw_regional_market_t::read_from_json_file(std::istream& file)
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
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void raw_regional_market_t::read_from_json_buffer(std::string_view buffer)
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
    this->read_from_json_json(json_root);
    
}

void raw_regional_market_t::read_from_json_json(const Json::Value& json_root)
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
        new_order.read_from_json_json(json_cur_order);
        this->orders_.emplace_back(std::move(new_order));
    }
    
}

void raw_regional_market_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_json_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding raw_regional_market object.");
}

void raw_regional_market_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
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
            
            this->orders_[ix].write_to_json_buffer(buffer, indent_start + 2 * spaces_per_tab, spaces_per_tab);
            
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
        destination.read_from_json_file(stream);
    } catch (error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

