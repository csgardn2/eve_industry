/// @file args.cpp
/// @brief Implementation of @ref args_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see html/index.html

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "args.h"
#include "error.h"
#include "json.h"

const std::vector<std::string_view> args_t::error_names_ =
{
    "FILE_SIZE_FAILED",
    "FILE_READ_FAILED",
    "FILE_WRITE_FAILED",
    "JSON_SCHEMA_VIOLATION",
    "MODE_MISSING",
    "MODE_INVALID",
    "ITEM_IDS_OUT_MISSING"
};

const std::vector<std::string_view> args_t::default_error_messages_ =
{
    "Error.  Failed to determine size of input file.\n",
    "Error.  Failed to read content from file.\n",
    "Error.  Failed to write content to file.\n",
    "Error.  Json input does not contain the correct fields.\n",
    "Error.  Missing argument --mode {fetch-item-ids}.\n",
    "Error.  Valid options for mode are {fetch-item-ids}\n.",
    "Error.  Missing argument --item-ids-out FILE.\n"
};

const std::vector<std::string_view> args_t::mode_names_ =
{
    "FETCH_ITEM_IDS"
};

const std::vector<std::string_view> args_t::mode_values_ =
{
    "fetch-item-ids"
};

/// @brief Search for a particular argument within argv and extract that
/// argument's parameter.
///
/// @return
///     *   If a string matching search_for was found in argv, the string
///         immediatly after the match is returned.
///     *   If a match wasn't found, an empty string view is returned.
std::string_view search_argv
(
    /// [in] This is the string you are looking for within argv
    const std::string_view search_for,
    /// [in] Passed from main.  The number of strings in argv
    unsigned argc,
    /// [in] Passed from main.  Tokenized arguments passed to this program on
    /// the command line.
    char const* const* argv
){
    for (unsigned ix = 0, bound = argc - 1; ix < bound; ix++)
    {
        if (search_for == argv[ix])
            return std::string_view(argv[ix + 1]);
    }
    return std::string_view();
}

void args_t::parse(unsigned argc, char const* const* argv)
{
    
    // Close any previously open files
    this->clear();
    
    // Parse --mode
    std::string_view mode = search_argv("--mode", argc, argv);
    if (mode.empty())
        throw error_message_t(error_code_t::MODE_MISSING);
    this->mode_ = mode_t::NUM_ENUMS;
    for (unsigned ix = 0; ix < unsigned(mode_t::NUM_ENUMS); ix++)
    {
        if (mode == args_t::mode_values_[ix])
        {
            this->mode_ = mode_t(ix);
            break;
        }
    }
    if (this->mode_ == mode_t::NUM_ENUMS)
        throw error_message_t(error_code_t::MODE_INVALID);
    
    // Parse --item-ids-out
    if (this->mode_ == mode_t::FETCH_ITEM_IDS)
    {
        this->item_ids_out_ = search_argv("--item-ids-out", argc, argv);
        if (this->item_ids_out_.empty())
        {
            std::string message("Error.  --item-ids-out FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::ITEM_IDS_OUT_MISSING);
        }
    }
    
}

void args_t::clear()
{
    this->mode_ = mode_t::NUM_ENUMS;
    this->item_ids_out_.clear();
}

void args_t::decode(std::istream& file)
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

void args_t::decode(std::string_view buffer)
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

void args_t::decode(const Json::Value& json_root)
{
    
    this->clear();
    
    // Parse root
    if (!json_root.isObject())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of args is not of type \"object\".\n");
    
    // Parse root/mode
    const Json::Value& json_mode = json_root["mode"];
    if (!json_mode.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/mode was not found or not of type \"string\".\n");
    const std::string_view mode = json_mode.asString();
    
    // Verify that mode is within the set of valid enumerated values
    this->mode_ = mode_t::NUM_ENUMS;
    for (unsigned ix = 0; ix < unsigned(mode_t::NUM_ENUMS); ix++)
    {
        if (mode == args_t::mode_names_[ix])
        {
            this->mode_ = mode_t(ix);
            break;
        }
    }
    if (this->mode_ == mode_t::NUM_ENUMS)
    {
        std::string message("Error.  Value for <args>/mode must be one of the values {");
        for (signed ix = 0, last_ix = signed(mode_t::NUM_ENUMS) - 1; ix <= last_ix; ix++)
        {
            message += args_t::mode_names_[ix];
            if (ix != last_ix)
                message += ", ";
        }
        message += "}.\n";
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
    }
    
    // Parse root/item_ids_out
    const Json::Value& json_item_ids_out = json_root["item_ids_out"];
    if (!json_item_ids_out.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/item_ids_out was not found or not of type \"string\".\n");
    this->item_ids_out_ = json_item_ids_out.asString();
    
}

void args_t::encode(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->encode(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED);
}

void args_t::encode(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    // Encode mode
    buffer += indent_1;
    buffer += "\"mode\": \"";
    buffer += args_t::mode_names_[unsigned(this->mode_)];
    buffer += "\"";
    
    buffer += ",\n";
    buffer += indent_1;
    buffer += "\"item_ids_out\": \"";
    buffer += this->item_ids_out_;
    buffer += "\"\n";
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += "}";
    
}

std::istream& operator>>(std::istream& stream, args_t& decode)
{
    try
    {
        decode.decode(stream);
    } catch (args_t::error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

