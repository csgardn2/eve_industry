/// @file item_ids.cpp
/// @brief Implementation of @ref item_ids_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Sunday October 8 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "https_get.h"
#include "item_ids.h"
#include "error.h"
#include "json.h"

/// @brief String representations of enumeration symbols.
const std::vector<std::string_view> item_ids_t::error_names_ =
{
    "FILE_SIZE_FAILED",
    "FILE_READ_FAILED",
    "FILE_WRITE_FAILED",
    "JSON_SCHEMA_VIOLATION",
    "EVE_SUCKS"
};

/// @brief Default error messages suitable for printing.  Note that
/// some of these are inherently vague without additional information.
const std::vector<std::string_view> item_ids_t::default_error_messages_ =
{
    "Error.  Failed to determine size of input file.\n",
    "Error.  Failed to read content from file.\n",
    "Error.  Failed to write content to file.\n",
    "Error.  Json input does not contain the correct fields.\n",
    "Error.  CCP changed something that used to work.\n"
};

void item_ids_t::fetch()
{
    
    this->clear();
    
    // Automatically de-allocate memory when this function returns or throws
    // an error.
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    
    std::string progress_message;
    
    // There are multiple pages of item IDs.  The last page is signified with
    // and empty array.
    std::string_view query_prefix("https://esi.tech.ccp.is/latest/universe/types/?datasource=tranquility&page=");
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
        Json::Value json_item_ids;
        std::string error_message;
        if (!reader->parse(payload.data(), payload.data() + payload.size(), &json_item_ids, &error_message))
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
        unsigned new_size = old_size + json_item_ids.size();
        this->resize(new_size);
        for
        (
            unsigned read_ix = 0, write_ix = old_size;
            write_ix < new_size;
            read_ix++, write_ix++
        ){
            
            const Json::Value& json_cur_element = json_item_ids[read_ix];
            
            if (!json_cur_element.isUInt())
            {
                std::string message("Error.  Element ");
                message += std::to_string(read_ix);
                message += " fetched from \"";
                message += query;
                message += "\" was not of type \"unsigned int\"\n";
                throw error_message_t(error_code_t::EVE_SUCKS, message);
            }
            
            (*this)[write_ix] = json_cur_element.asUInt();
            
        }
        
        if (json_item_ids.empty())
        {
            std::cout << '\n';
            return;
        }
        
    }
    
}

void item_ids_t::decode(std::istream& file)
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
    this->decode(std::string_view(buffer));
    
}

void item_ids_t::decode(std::string_view buffer)
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

void item_ids_t::decode(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of <item_ids> is not of type \"array\".\n");
    
    // Allocate memory
    unsigned size = json_root.size();
    this->clear();
    this->resize(size);
    
    // Copy each ID
    for (unsigned ix = 0; ix < size; ix++)
    {
        
        const Json::Value& json_cur_element = json_root[ix];
        if (!json_cur_element.isUInt())
        {
            std::string message("Error.  <item_ids>[");
            message += std::to_string(ix);
            message += "] was not of type \"unsigned int\".\n";
            throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
        }
        
        (*this)[ix] = json_cur_element.asUInt();
        
    }
    
}

void item_ids_t::encode(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->encode(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED);
}

void item_ids_t::encode(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    
    // Encode member variables.
    for (signed ix = 0, last_ix = this->size() - 1; ix <= last_ix; ix++)
    {
        
        buffer += indent_1;
        buffer += std::to_string((*this)[ix]);
        
        if (ix != last_ix)
            buffer += ',';
        buffer += '\n'; 
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += "]";
    
}

std::istream& operator>>(std::istream& stream, item_ids_t& decode)
{
    try
    {
        decode.decode(stream);
    } catch (item_ids_t::error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

