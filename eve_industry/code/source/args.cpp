/// @brief Implementation of @ref args_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "args.h"
#include "error.h"
#include "json.h"

const std::vector<std::string_view> args_t::mode_names_ =
{
    "FETCH_ITEM_ATTRIBUTES",
    "FETCH_PRICES",
    "CALCULATE_BLUEPRINT_PROFITS"
};

const std::vector<std::string_view> args_t::mode_values_ =
{
    "fetch-item-attributes",
    "fetch-prices",
    "calculate-blueprint-profits"
};

std::string args_t::valid_mode_values() const
{
    
    std::string ret("{");
    
    for (unsigned ix = 0, last_ix = args_t::mode_values_.size() - 1; ix <= last_ix; ix++)
    {
        ret += args_t::mode_values_[ix];
        if (ix == last_ix)
            ret += '}';
        else
            ret += ", ";
    }
    
    return ret;
    
}

/// @brief Search for a particular argument within argv and extract that
/// argument's parameter.
///
/// @return
///     *   If a string matching search_for was found in argv, the string
///         immediatly after the match is returned.
///     *   If a match wasn't found, an empty string view is returned.
std::string_view find_argument
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

/// @brief Search for a particular argument within argv and return true if it
/// was found.
bool find_existence
(
    /// [in] This is the string you are looking for within argv
    const std::string_view search_for,
    /// [in] Passed from main.  The number of strings in argv
    unsigned argc,
    /// [in] Passed from main.  Tokenized arguments passed to this program on
    /// the command line.
    char const* const* argv
){
    for (unsigned ix = 0; ix < argc; ix++)
    {
        if (search_for == argv[ix])
            return true;
    }
    return false;
}

void args_t::parse(unsigned argc, char const* const* argv)
{
    
    // Close any previously open files
    this->clear();
    
    // Parse --mode
    std::string_view mode = find_argument("--mode", argc, argv);
    if (mode.empty())
    {
        std::string message("Error.  Missing argument --mode ");
        message += valid_mode_values();
        message += ".\n";
        throw error_message_t(error_code_t::MODE_MISSING, message);
    }
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
    {
        std::string message("Error.  Invalid argument \"");
        message += mode;
        message += "\" for mode.  Use one of ";
        message += valid_mode_values();
        message += ".\n";
        throw error_message_t(error_code_t::MODE_INVALID, message);
    }
    
    // Parse --quiet
    this->debug_mode_.verbose(!find_existence("--quiet", argc, argv));
    
    // Parse --item-attributes-out
    if (this->mode_ == mode_t::FETCH_ITEM_ATTRIBUTES)
    {
        this->item_attributes_out_ = find_argument("--item-attributes-out", argc, argv);
        if (this->item_attributes_out_.empty())
        {
            std::string message("Error.  --item-attributes-out FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::ITEM_ATTRIBUTES_OUT_MISSING, message);
        }
    }
    
    // Parse --item-attributes-in
    if (this->mode_ == mode_t::FETCH_PRICES)
    {
        this->item_attributes_in_ = find_argument("--item-attributes-in", argc, argv);
        if (this->item_attributes_in_.empty())
        {
            std::string message("Error.  --item-attributes-in FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::ITEM_ATTRIBUTES_OUT_MISSING, message);
        }
    }
    
    // Parse --station-attributes-in
    if (this->mode_ == mode_t::FETCH_PRICES)
    {
        this->station_attributes_in_ = find_argument("--station-attributes-in", argc, argv);
        if (this->item_attributes_in_.empty())
        {
            std::string message("Error.  --station-attributes-in FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::ITEM_ATTRIBUTES_OUT_MISSING, message);
        }
    }
    
    // Parse --prices-out
    if (this->mode_ == mode_t::FETCH_PRICES)
    {
        this->prices_out_ = find_argument("--prices-out", argc, argv);
        if (this->prices_out_.empty())
        {
            std::string message("Error.  --prices-out FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::PRICES_OUT_MISSING, message);
        }
    }
    
    // Parse --prices-in
    if (this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        this->prices_in_ = find_argument("--prices-in", argc, argv);
        if (this->prices_in_.empty())
        {
            std::string message("Error.  --prices-in FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::PRICES_IN_MISSING, message);
        }
    }
    
    // Parse --blueprints-in
    if (this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        this->blueprints_in_ = find_argument("--blueprints-in", argc, argv);
        if (this->blueprints_in_.empty())
        {
            std::string message("Error.  --blueprints-in FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::BLUEPRINTS_IN_MISSING, message);
        }
    }
    
    // Parse --profits-out
    if (this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        this->profits_out_ = find_argument("--profits-out", argc, argv);
        if (this->profits_out_.empty())
        {
            std::string message("Error.  --profits-out FILE is required for ");
            message += mode_values_[unsigned(this->mode_)];
            message += " mode.\n";
            throw error_message_t(error_code_t::PROFITS_OUT_MISSING, message);
        }
    }
    
    // Parse --dont-cull-stations
    this->cull_stations_ = !find_existence("--dont-cull-stations", argc, argv);
    
    // Parse --dont-cull-orders
    this->cull_orders_ = !find_existence("--dont-cull-orders", argc, argv);
    
}

void args_t::clear()
{
    this->debug_mode_.verbose(true);
    this->mode_ = mode_t::NUM_ENUMS;
    this->item_attributes_out_.clear();
    this->item_attributes_in_.clear();
    this->station_attributes_in_.clear();
    this->prices_out_.clear();
    this->prices_in_.clear();
    this->blueprints_in_.clear();
    this->profits_out_.clear();
    this->cull_stations_ = false;
    this->cull_orders_ = false;
}

void args_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding args_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding args_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void args_t::read_from_json_buffer(std::string_view buffer)
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

void args_t::read_from_json_json(const Json::Value& json_root)
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
        std::string message("Error.  Value for <args>/mode must be one of the values ");
        message += valid_mode_values();
        message += ".\n";
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, message);
    }
    
    // Parse root/item_attributes_out
    const Json::Value& json_item_attributes_out = json_root["item_attributes_out"];
    if (!json_item_attributes_out.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/item_attributes_out was not found or not of type \"string\".\n");
    this->item_attributes_out_ = json_item_attributes_out.asString();
    
    // Parse root/item_attributes_in
    const Json::Value& json_item_attributes_in = json_root["item_attributes_in"];
    if (!json_item_attributes_in.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/item_attributes_in was not found or not of type \"string\".\n");
    this->item_attributes_in_ = json_item_attributes_in.asString();
    
    // Parse root/station_attributes_in
    const Json::Value& json_station_attributes_in = json_root["station_attributes_in"];
    if (!json_station_attributes_in.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/station_attributes_in was not found or not of type \"string\".\n");
    this->station_attributes_in_ = json_station_attributes_in.asString();
    
    // Parse root/prices_out
    const Json::Value& json_prices_out = json_root["prices_out"];
    if (!json_prices_out.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/prices_out was not found or not of type \"string\".\n");
    this->prices_out_ = json_prices_out.asString();
    
}

void args_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_json_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding args_t object.");
}

void args_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
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
    
    // Encode item_attributes_out
    buffer += ",\n";
    buffer += indent_1;
    buffer += "\"item_attributes_out\": \"";
    buffer += this->item_attributes_out_;
    buffer += "\"";
    
    // Encode item_attributes_in
    buffer += ",\n";
    buffer += indent_1;
    buffer += "\"item_attributes_in\": \"";
    buffer += this->item_attributes_in_;
    buffer += "\"";
    
    // Encode station_attributes_in
    buffer += ",\n";
    buffer += indent_1;
    buffer += "\"station_attributes_in\": \"";
    buffer += this->station_attributes_in_;
    buffer += "\"";
    
    // Encode prices_out
    buffer += ",\n";
    buffer += indent_1;
    buffer += "\"prices_out\": \"";
    buffer += this->prices_out_;
    buffer += "\"";
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += '\n';
    buffer += indent_0;
    buffer += "}";
    
}

std::istream& operator>>(std::istream& stream, args_t& destination)
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

