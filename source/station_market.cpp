/// @file station_market.cpp
/// @brief Implementation of @ref station_market_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday November 7 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <string>
#include <string_view>

#include "item_market.h"
#include "station_market.h"
#include "error.h"
#include "json.h"

void station_market_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding station_market_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding station_market_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void station_market_t::read_from_json_buffer(std::string_view buffer)
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

void station_market_t::read_from_json_json(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of station_market is not of type \"object\".\n");
    
    // Decode station_id
    const Json::Value& json_station_id = json_root["station_id"];
    if (!json_station_id.isUInt64())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <station_market>/station_id was not found or not of type \"unsigned integer\".\n");
    this->station_id_ = json_station_id.asUInt64();
    
    // Decode items array
    const Json::Value& json_items = json_root["items"];
    if (!json_items.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <station_market>/items was not found or not of type \"array\".\n");
    
    // Decode each item market in the items array
    this->items_.clear();
    this->items_.reserve(json_items.size());
    for (const Json::Value& json_cur_item : json_items)
    {
        item_market_t cur_item_market;
        cur_item_market.read_from_json_json(json_cur_item);
        this->items_.emplace(cur_item_market.item_id(), cur_item_market);
    }
    
}

void station_market_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_json_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding station_market_t object.");
}

void station_market_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_2(indent_start + 2 * spaces_per_tab, ' ');
    std::string_view indent_1(indent_2.data(), indent_start + spaces_per_tab);
    std::string_view indent_0(indent_2.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    // Encode station_id
    buffer += indent_1;
    buffer += "\"station_id\": ";
    buffer += std::to_string(this->station_id_);
    buffer += ",\n";
    
    // Encode each item_market in the items member.
    buffer += indent_1;
    unsigned num_items = this->items_.size();
    if (num_items == 0)
    {
        
        buffer += "\"items\": []\n";
        
    } else {
        
        buffer += "\"items\": [\n";
        buffer += indent_2;
        unsigned num_items_processed = 0;
        unsigned last_item_to_process = num_items - 1;
        for (const std::pair<uint64_t, item_market_t>& cur_pair : this->items_)
        {
            
            const item_market_t& cur_item_market = cur_pair.second;
            
            cur_item_market.write_to_json_buffer(buffer, indent_start + 2 * spaces_per_tab, spaces_per_tab);
            
            if (num_items_processed == last_item_to_process)
                buffer += '\n';
            else
                buffer += ", ";
            num_items_processed++;
            
        }
        
        buffer += indent_1;
        buffer += "]\n";
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += '}';
    
}

std::istream& operator>>(std::istream& stream, station_market_t& destination)
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

