/// @file station_attributes.cpp
/// @brief Implementation of @ref station_attributes_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday October 31 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "station_attribute.h"
#include "station_attributes.h"
#include "error.h"
#include "json.h"

void station_attributes_t::read_from_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding station_attributes_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding station_attributes_t object.\n");
    this->read_from_buffer(std::string_view(buffer));
    
}

void station_attributes_t::read_from_buffer(std::string_view buffer)
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

void station_attributes_t::read_from_json(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of station_attributes is not of type \"array\".\n");
    
    // Pre-allocate storage
    this->stations_.clear();
    this->stations_.reserve(json_root.size());
    
    // Iterate through each element in the array, decode it, and store it.
    for (const Json::Value& cur_element : json_root)
    {
        station_attribute_t new_station_attribute;
        new_station_attribute.read_from_json(cur_element);
        this->stations_.emplace_back(std::move(new_station_attribute));
    }
    
}

void station_attributes_t::write_to_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding station_attributes_t object.");
}

void station_attributes_t::write_to_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // Prettify empty arrays.
    if (this->stations_.empty())
    {
        buffer += "[]";
        return;
    }
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    
    // Encode each member
    for (unsigned ix = 0, last_ix = this->stations_.size() - 1; ix <= last_ix; ix++)
    {
        
        buffer += indent_1;
        (*this)[ix].write_to_buffer(buffer, indent_start + spaces_per_tab, spaces_per_tab);
        
        if (ix == last_ix)
            buffer += '\n';
        else
            buffer += ", ";
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += "]";
    
}

std::istream& operator>>(std::istream& stream, station_attributes_t& destination)
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

