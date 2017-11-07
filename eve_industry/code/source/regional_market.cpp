/// @file regional_market.cpp
/// @brief Implementation of @ref regional_market_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday November 7 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "station_market.h"
#include "raw_regional_market.h"
#include "regional_market.h"
#include "error.h"
#include "json.h"

void regional_market_t::initialize_from_raw_regional_market
(
    const raw_regional_market_t& raw_regional_market
){
    
    this->stations_.clear();
    
    this->region_id_ = raw_regional_market.region_id();
    
    // Iterate through each order in the raw market data
    for (const raw_order_t& cur_raw_order : raw_regional_market.orders())
    {
        
        // Select order's station and item, making new spaces if they didn't exist before
        station_market_t& cur_station = this->find_or_allocate(cur_raw_order.station_id());
        item_market_t& cur_item = cur_station.find_or_allocate(cur_raw_order.item_id());
        
        // Copy this order from raw data to sorted market structure
        cur_station.station_id(cur_raw_order.station_id());
        cur_item.item_id(cur_raw_order.item_id());
        
        switch (cur_raw_order.order_type())
        {
            case order_type_t::BUY:
                cur_item.new_buy_order(cur_raw_order.price());
                break;
            case order_type_t::SELL:
                cur_item.new_sell_order(cur_raw_order.price());
                break;
            default:
                throw error_message_t(error_code_t::UNKNOWN_ORDER_TYPE, "Error.  Could not encode order with unknown order_type.\n");
                break;
        }
        
        
    }
    
}

void regional_market_t::read_from_file(std::istream& file)
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

void regional_market_t::read_from_buffer(std::string_view buffer)
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

void regional_market_t::read_from_json(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of regional_market is not of type \"object\".\n");
    
    // Decode region_id
    const Json::Value& json_region_id = json_root["region_id"];
    if (!json_region_id.isUInt64())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <regional_market>/region_id was not found or not of type \"unsigned integer\".\n");
    this->region_id_ = json_region_id.asUInt64();
    
    // Decode stations array
    const Json::Value& json_stations = json_root["stations"];
    if (!json_stations.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <regional_market>/stations was not found or not of type \"array\".\n");
    
    // Decode each station market in the stations array
    this->stations_.clear();
    this->stations_.reserve(json_stations.size());
    for (const Json::Value& json_cur_station : json_stations)
    {
        station_market_t cur_station_market;
        cur_station_market.read_from_json(json_cur_station);
        this->stations_.emplace(cur_station_market.station_id(), cur_station_market);
    }
    
}

void regional_market_t::write_to_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED);
}

void regional_market_t::write_to_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_2(indent_start + 2 * spaces_per_tab, ' ');
    std::string_view indent_1(indent_2.data(), indent_start + spaces_per_tab);
    std::string_view indent_0(indent_2.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    // Encode regional_id
    buffer += indent_1;
    buffer += "\"regional_id\": ";
    buffer += std::to_string(this->region_id_);
    buffer += ",\n";
    
    // Encode each station_market in the stations member.
    buffer += indent_1;
    unsigned num_stations = this->stations_.size();
    if (num_stations == 0)
    {
        
        buffer += "\"stations\": []\n";
        
    } else {
        
        buffer += "\"stations\": [\n";
        buffer += indent_2;
        unsigned num_stations_processed = 0;
        unsigned last_station_to_process = num_stations - 1;
        for (const std::pair<uint64_t, station_market_t>& cur_pair : this->stations_)
        {
            
            const station_market_t& cur_station_market = cur_pair.second;
            
            cur_station_market.write_to_buffer(buffer, indent_start + 2 * spaces_per_tab, spaces_per_tab);
            
            if (num_stations_processed == last_station_to_process)
                buffer += '\n';
            else
                buffer += ", ";
            num_stations_processed++;
            
        }
        
        buffer += indent_1;
        buffer += "]\n";
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += '}';
    
}

std::istream& operator>>(std::istream& stream, regional_market_t& destination)
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

