/// @file item_attribute.cpp
/// @brief Implementation of @ref item_attribute_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday October 10 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "item_attribute.h"
#include "error.h"
#include "https_get.h"
#include "json.h"
#include "util.h"

/// @brief String representations of enumeration symbols.
const std::vector<std::string_view> item_attribute_t::error_names_ =
{
    "FILE_SIZE_FAILED",
    "FILE_READ_FAILED",
    "FILE_WRITE_FAILED",
    "JSON_SCHEMA_VIOLATION",
    "EVE_SUCKS"
};

/// @brief Default error messages suitable for printing.  Note that
/// some of these are inherently vague without additional information.
const std::vector<std::string_view> item_attribute_t::default_error_messages_ =
{
    "Error.  Failed to determine size of input file.\n",
    "Error.  Failed to read content from file.\n",
    "Error.  Failed to write content to file.\n",
    "Error.  Json input does not contain the correct fields.\n",
    "Error.  CCP changed something that used to work.\n"
};

void item_attribute_t::fetch(unsigned id)
{
    
    // Automatically de-allocate memory when this function returns or throws
    // an error.
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    
    this->fetch(id, reader.get());
    
}

void item_attribute_t::fetch(unsigned id, Json::CharReader* reader)
{
    
    // Prepare a query
    std::string query("https://esi.tech.ccp.is/latest/universe/types/");
    query += std::to_string(id);
    query += "/?datasource=tranquility&language=en-us";
    
    // Pull data from the network
    std::string payload = https_get(query);
    
    // Attempt to decode received data
    Json::Value json_root;
    std::string error_message;
    if (!reader->parse(payload.data(), payload.data() + payload.size(), &json_root, &error_message))
    {
        std::string message("Error.  Tried to decode JSON data from\"");
        message += query;
        message += "\" but it failed with message \"";
        message += error_message;
        message += "\".\n";
        throw error_message_t(error_code_t::EVE_SUCKS, message);
    }
    
    // Assign name member.
    const Json::Value& json_name = json_root["name"];
    if (!json_name.isString())
        throw error_message_t(error_code_t::EVE_SUCKS, "Error.  <item_attribute>/name member was not found or not of type \"string\".\n");
    this->name_ = json_name.asString();
    
    // Assign ID member.
    this->id_ = id;
    
}

void item_attribute_t::decode(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED);
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED);
    this->decode(std::string_view(buffer));
    
}

void item_attribute_t::decode(std::string_view buffer)
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
    this->decode(json_root);
    
}

void item_attribute_t::decode(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of item_attribute is not of type \"object\".\n");
    
    // Extract id_ member.
    const Json::Value& json_id = json_root["id"];
    if (!json_id.isUInt())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <item_attribute>/id was not found or not of type \"unsigned integer\".\n");
    this->id_ = json_id.asUInt();
    
    // Extract name_ member.
    const Json::Value& json_name = json_root["name"];
    if (!json_name.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <item_attribute>/name was not found or not of type \"string\".\n");
    this->name_ = json_name.asString();
    
}

void item_attribute_t::encode(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->encode(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED);
}

void item_attribute_t::encode(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    // Encode id_
    buffer += indent_1;
    buffer += "\"id\": ";
    buffer += std::to_string(this->id_);
    buffer += ",\n";
    
    // Encode name_ and escape quotes.
    buffer += indent_1;
    buffer += "\"name\": \"";
    buffer += escape_quotes(this->name_);
    buffer += "\"\n";
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += "}";
    
}

std::istream& operator>>(std::istream& stream, item_attribute_t& decode)
{
    try
    {
        decode.decode(stream);
    } catch (item_attribute_t::error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

