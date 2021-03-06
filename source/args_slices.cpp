/// @file args_slices.cpp
/// @brief Implementation of @ref args::slices_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday December 7 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "args_slices.h"
#include "error.h"
#include "json.h"

void args::slices_t::initialize_from_command_line(unsigned argc, char const* const* argv)
{
    
    // Argument currently being parsed
    unsigned cur_arg_ix = 0;
    
    // Skip to the first argument starting with '-'.
    while (true)
    {
        if (cur_arg_ix >= argc)
            return;
        if (argv[cur_arg_ix][0] == '-')
            break;
        cur_arg_ix++;
    }
    
    this->storage_.clear();
    
    // Extract each argument segment
    while (true)
    {
        
        /// All out of arguments
        if (cur_arg_ix >= argc)
            return;
        
        // The previous iteration will leave us off at the next argument
        std::string new_key = argv[cur_arg_ix];
        cur_arg_ix++;
        
        // Accumulate all parameters after this parameter
        std::vector<std::string> new_value;
        while (true)
        {
            if (cur_arg_ix >= argc || argv[cur_arg_ix][0] == '-')
                break;
            new_value.emplace_back(argv[cur_arg_ix]);
            cur_arg_ix++;
        }
        
        this->storage_.emplace(std::move(new_key), std::move(new_value));
        
    }
    
}

void args::slices_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when encoding slices_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding slices_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void args::slices_t::read_from_json_buffer(std::string_view buffer)
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

void args::slices_t::read_from_json_structure(const Json::Value& json_root)
{
    
    this->clear();
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of slices is not of type \"array\".\n");
    
    unsigned num_slices = json_root.size();
    this->storage_.reserve(num_slices);
    
    // Iterate through each element in the root array
    for (unsigned ix = 0; ix < num_slices; ix++)
    {
        
        // Parse current slice in the array
        const Json::Value& json_slice = json_root[ix];
        if (!json_slice.isObject())
        {
            std::string message("Error.  slices[");
            message += std::to_string(ix);
            message += "] was not of type \"object\".";
            throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
        }
        
        // Parse slice[ix]/argument
        // Delay storage
        const Json::Value& json_argument = json_slice["argument"];
        if (!json_argument.isString())
        {
            std::string message("Error.  slices[");
            message += std::to_string(ix);
            message += "]/argument was not found or not of type \"string\".";
            throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
        }
        
        // Parse slice[ix]/parameters
        const Json::Value& json_parameters = json_slice["parameters"];
        if (!json_parameters.isArray())
        {
            std::string message("Error.  slices[");
            message += std::to_string(ix);
            message += "]/parameters was not found or not of type \"array\".";
            throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
        }
        
        // Start extracting each parameter
        unsigned num_parameters = json_parameters.size();
        std::vector<std::string> new_parameters(num_parameters);
        for (unsigned iy = 0; iy < num_parameters; iy++)
        {
            const Json::Value& json_cur_parameter = json_parameters[iy];
            if (!json_cur_parameter.isString())
            {
                std::string message("Error.  slices[");
                message += std::to_string(ix);
                message += "]/parameters[";
                message += std::to_string(iy);
                message += "] was not of type \"string\".";
                throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
            }
            new_parameters[iy] = json_cur_parameter.asString();
        }
        
        // Commit slice to storage
        this->storage_.emplace(json_argument.asString(), std::move(new_parameters));
        
    }
    
}

void args::slices_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    std::string buffer;
    this->write_to_json_buffer(buffer, indent_start, spaces_per_tab);
    file << buffer;
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding slices object.\n");
}

void args::slices_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    // Pretty formatting for empty argument set
    if (this->storage_.empty())
    {
        buffer += "[]";
        return;
    }
    
    // Create as many indent levels as needed using string_view.
    std::string indent_3(indent_start + 3 * spaces_per_tab, ' ');
    std::string_view indent_2(indent_3.data(), 2 * spaces_per_tab + indent_start);
    std::string_view indent_1(indent_3.data(), spaces_per_tab + indent_start);
    std::string_view indent_0(indent_3.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    
    // Encode each slice
    buffer += indent_1;
    bool first = true;
    for (const std::pair<std::string, std::vector<std::string> >& cur_slice : this->storage_)
    {
        
        if (first)
            first = false;
        else
            buffer += ", ";
        
        buffer += "{\n";
        
        // Encode argument for current slice
        buffer += indent_2;
        buffer += "\"argument\": \"";
        buffer += cur_slice.first;
        buffer += "\",\n";
        
        // Encode parameters for current slice
        buffer += indent_2;
        buffer += "\"parameters\": ";
        const std::vector<std::string>& parameters = cur_slice.second;
        if (parameters.empty())
        {
            
            buffer += "[]\n";
            
        } else {
            
            buffer += "[\n";
            
            for (unsigned ix = 0, last_ix = parameters.size() - 1; ix <= last_ix; ix++)
            {
                
                // Encode current parameter
                buffer += indent_3;
                buffer += '\"';
                buffer += parameters[ix];
                buffer += '\"';
                
                if (ix == last_ix)
                    buffer += '\n';
                else
                    buffer += ",\n";
                
            }
            
            buffer += indent_2;
            buffer += "]\n";
            
        }
        
        buffer += indent_1;
        buffer += '}';
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += '\n';
    buffer += indent_0;
    buffer += ']';
    
}

std::istream& operator>>(std::istream& stream, args::slices_t& destination)
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

