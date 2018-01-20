/// @file item_quantity.cpp
/// @brief Implementation of @ref item_quantity_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 24 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <string>
#include <string_view>

#include "item_quantity.h"
#include "error.h"
#include "json.h"

void item_quantity_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when encoding item_quantity_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding item_quantity_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void item_quantity_t::read_from_json_buffer(std::string_view buffer)
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
    this->read_from_json_structure(json_root);
    
}

void item_quantity_t::read_from_json_structure(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of item_quantity is not of type \"object\".\n");
    
    // Parse item_id
    const Json::Value& json_item_id = json_root["item_id"];
    if (!json_item_id.isUInt64())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <item_quantity>/item_id was not found or not of type \"unsigned integer\".\n");
    this->item_id_ = json_item_id.asUInt64();
    
    // Parse quantity
    const Json::Value& json_quantity = json_root["quantity"];
    if (!json_quantity.isUInt())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <item_quantity>/quantity was not found or not of type \"unsigned integer\".\n");
    this->quantity_ = json_quantity.asUInt();
    
}

void item_quantity_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    std::string buffer;
    this->write_to_json_buffer(buffer, indent_start, spaces_per_tab);
    file << buffer;
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding item_quantity_t object.\n");
}

void item_quantity_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    buffer += indent_1;
    buffer += "\"item_id\": ";
    buffer += std::to_string(this->item_id_);
    buffer += ",\n";
    
    buffer += indent_1;
    buffer += "\"quantity\": ";
    buffer += std::to_string(this->quantity_);
    buffer += '\n';
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += '}';
    
}

std::istream& operator>>(std::istream& stream, item_quantity_t& destination)
{
    try
    {
        destination.read_from_json_file(stream);
    } catch (const error_message_t& error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (const Json::Exception& error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}
