/// @file args_eve_industry.cpp
/// @brief Implementation of @ref args::eve_industry_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "args_eve_industry.h"
#include "error.h"
#include "json.h"

const std::vector<std::string_view> args::eve_industry_t::mode_names_ =
{
    "FETCH_ITEM_ATTRIBUTES",
    "FETCH_PRICES",
    "CALCULATE_BLUEPRINT_PROFITS"
};

const std::vector<std::string_view> args::eve_industry_t::mode_values_ =
{
    "fetch-item-attributes",
    "fetch-prices",
    "calculate-blueprint-profits"
};

std::string args::eve_industry_t::valid_mode_values() const
{
    
    std::string ret("{");
    
    for (unsigned ix = 0, last_ix = args::eve_industry_t::mode_values_.size() - 1; ix <= last_ix; ix++)
    {
        ret += args::eve_industry_t::mode_values_[ix];
        if (ix == last_ix)
            ret += '}';
        else
            ret += ", ";
    }
    
    return ret;
    
}

blueprint_profit_t::sort_strategy_t string_to_sort_strategy(std::string_view str)
{
    if (str == "profit_amount")
        return blueprint_profit_t::sort_strategy_t::PROFIT_AMOUNT;
    else if (str == "profit_percent")
        return blueprint_profit_t::sort_strategy_t::PROFIT_PERCENT;
    else if (str == "profit_per_second")
        return blueprint_profit_t::sort_strategy_t::PROFIT_PER_SECOND;
    else {
        std::string message("Error.  Invalid value \"");
        message += str;
        message += "\" for enumerated sort strategy.\n";
        throw error_message_t(error_code_t::SORT_STRATEGY_INVALID, message);
    }
}

std::string_view sort_strategy_to_string(blueprint_profit_t::sort_strategy_t sort_strategy)
{
    switch (sort_strategy)
    {
        case blueprint_profit_t::sort_strategy_t::PROFIT_AMOUNT:
            return "profit_amount";
        case blueprint_profit_t::sort_strategy_t::PROFIT_PERCENT:
            return "profit_percent";
        case blueprint_profit_t::sort_strategy_t::PROFIT_PER_SECOND:
            return "profit_per_second";
        default:
            throw error_message_t(error_code_t::UNKNOWN_SORT_STRATEGY, "Error.  Encountered unknown enum for sort_strategy_t while decoding arguments.\n");
    }
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

void args::eve_industry_t::parse(unsigned argc, char const* const* argv)
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
        if (mode == args::eve_industry_t::mode_values_[ix])
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
    // Note this argument is required for FETCH_PRICES
    // and optional for CALCULATE_BLUEPRINT_PROFITS
    if (this->mode_ == mode_t::FETCH_PRICES || this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        this->item_attributes_in_ = find_argument("--item-attributes-in", argc, argv);
        if (this->item_attributes_in_.empty() && this->mode_ == mode_t::FETCH_PRICES)
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
    
    // Parse output_order_
    if (this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        std::string_view output_order_string = find_argument("--output-order", argc, argv);
        if (!output_order_string.empty())
            this->output_order_ = string_to_sort_strategy(output_order_string);
        // else use default value set by clear()
    }
    
    // Parse decryptor_optimization_strategy
    if (this->mode_ == mode_t::CALCULATE_BLUEPRINT_PROFITS)
    {
        std::string_view decryptor_optimization_strategy_string = find_argument("--decryptor-optimization-strategy", argc, argv);
        if (!decryptor_optimization_strategy_string.empty())
            this->decryptor_optimization_strategy_ = string_to_sort_strategy(decryptor_optimization_strategy_string);
        // else use default value set by clear()
    }
    
}

void args::eve_industry_t::clear()
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
    this->output_order_ = blueprint_profit_t::sort_strategy_t::PROFIT_PER_SECOND;
    this->decryptor_optimization_strategy_ = blueprint_profit_t::sort_strategy_t::PROFIT_PER_SECOND;
}

void args::eve_industry_t::read_from_json_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding eve_industry_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding eve_industry_t object.\n");
    this->read_from_json_buffer(std::string_view(buffer));
    
}

void args::eve_industry_t::read_from_json_buffer(std::string_view buffer)
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

void args::eve_industry_t::read_from_json_json(const Json::Value& json_root)
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
        if (mode == args::eve_industry_t::mode_values_[ix])
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
    
    // Parse root/prices_in
    const Json::Value& json_prices_in = json_root["prices_in"];
    if (!json_prices_in.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/prices_in was not found or not of type \"string\".\n");
    this->prices_in_ = json_prices_in.asString();
    
    // Parse root/blueprints_in
    const Json::Value& json_blueprints_in = json_root["json_blueprints_in"];
    if (!json_blueprints_in.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/blueprints_in was not found or not of type \"string\".\n");
    this->blueprints_in_ = json_blueprints_in.asString();
    
    // Parse root/profits_out
    const Json::Value& json_profits_out = json_root["json_profits_out"];
    if (!json_profits_out.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/profits_out was not found or not of type \"string\".\n");
    this->profits_out_ = json_profits_out.asString();
    
    // Parse root/output_order
    const Json::Value& json_output_order = json_root["json_output_order"];
    if (!json_output_order.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/output_order was not found or not of type \"string\".\n");
    const std::string& output_order_string = json_output_order.asString();
    this->output_order_ = string_to_sort_strategy(std::string_view(output_order_string));
    
    // Parse root/decryptor_optimization_strategy
    const Json::Value& json_decryptor_optimization_strategy = json_root["json_decryptor_optimization_strategy"];
    if (!json_decryptor_optimization_strategy.isString())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  <args>/decryptor_optimization_strategy was not found or not of type \"string\".\n");
    const std::string& decryptor_optimization_strategy_string = json_decryptor_optimization_strategy.asString();
    this->decryptor_optimization_strategy_ = string_to_sort_strategy(std::string_view(decryptor_optimization_strategy_string));
    
}

void args::eve_industry_t::write_to_json_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_json_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding eve_industry_t object.");
}

void args::eve_industry_t::write_to_json_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "{\n";
    
    // Encode mode
    buffer += indent_1;
    buffer += "\"mode\": \"";
    buffer += args::eve_industry_t::mode_values_[unsigned(this->mode_)];
    buffer += "\",\n";
    
    // Encode item_attributes_out
    buffer += indent_1;
    buffer += "\"item_attributes_out\": \"";
    buffer += this->item_attributes_out_;
    buffer += "\",\n";
    
    // Encode item_attributes_in
    buffer += indent_1;
    buffer += "\"item_attributes_in\": \"";
    buffer += this->item_attributes_in_;
    buffer += "\",\n";
    
    // Encode station_attributes_in
    buffer += indent_1;
    buffer += "\"station_attributes_in\": \"";
    buffer += this->station_attributes_in_;
    buffer += "\",\n";
    
    // Encode prices_out
    buffer += indent_1;
    buffer += "\"prices_out\": \"";
    buffer += this->prices_out_;
    buffer += "\",\n";
    
    // Encode prices_in
    buffer += indent_1;
    buffer += "\"prices_in\": \"";
    buffer += this->prices_in_;
    buffer += "\",\n";
    
    // Encode blueprints_in
    buffer += indent_1;
    buffer += "\"blueprints_in\": \"";
    buffer += this->blueprints_in_;
    buffer += "\",\n";
    
    // Encode profits_out
    buffer += indent_1;
    buffer += "\"profits_out\": \"";
    buffer += this->profits_out_;
    buffer += "\",\n";
    
    // Encode cull_stations
    buffer += indent_1;
    buffer += "\"cull_stations\": ";
    buffer += this->cull_stations_ ? "true,\n" : "false,\n";
    
    // Encode cull_orders
    buffer += indent_1;
    buffer += "\"cull_orders\": ";
    buffer += this->cull_orders_ ? "true,\n" : "false,\n";
    
    // Encode output_order
    buffer += indent_1;
    buffer += "\"output_order\": \"";
    buffer += sort_strategy_to_string(this->output_order_);
    buffer += "\",\n";
    
    // Encode 
    buffer += indent_1;
    buffer += "\"decryptor_optimization_strategy\": \"";
    buffer += sort_strategy_to_string(this->decryptor_optimization_strategy_);
    buffer += "\"\n";
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += indent_0;
    buffer += "}";
    
}

std::istream& operator>>(std::istream& stream, args::eve_industry_t& destination)
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
