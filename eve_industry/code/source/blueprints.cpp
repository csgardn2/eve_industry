/// @file blueprints.cpp
/// @brief Implementation of @ref blueprints_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday November 16 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "blueprint.h"
#include "blueprints.h"
#include "error.h"
#include "json.h"

void blueprints_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when encoding blueprints_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding blueprints_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void blueprints_t::read_from_json_buffer(std::string_view buffer)
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

void blueprints_t::read_from_json_structure(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of blueprints_t is not of type \"array\".\n");
    
    // Allocate storage for all blueprints
    unsigned num_blueprints = json_root.size();
    this->storage_.resize(num_blueprints);
    
    // Overwrite previous content
    for (unsigned ix = 0; ix < num_blueprints; ix++)
        this->storage_[ix].read_from_json_structure(json_root[ix]);
    
}

void blueprints_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    std::string buffer;
    this->write_to_json_buffer(buffer, indent_start, spaces_per_tab);
    file << buffer;
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding blueprints_t object.\n");
}

void blueprints_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // Use compact notation for empty vectors
    unsigned num_blueprints = this->storage_.size();
    if (num_blueprints == 0)
    {
        buffer += "[]";
        return;
    }
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    
    // Encode each blueprint as an element of an array.
    buffer += indent_1;
    for (unsigned ix = 0, last_ix = num_blueprints - 1; ix <= last_ix; ix++)
    {
        this->storage_[ix].write_to_json_buffer(buffer, indent_start + spaces_per_tab, spaces_per_tab);
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

std::istream& operator>>(std::istream& stream, blueprints_t& destination)
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

