/// \file
/// File Name:                      json.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or json.h for documentation.

#include <climits>
#include <iostream>
#include <string>
#include <vector>

#include "json.h"
#include "token.h"

const char* json_type_to_string(json_type_t json_type)
{
    
    static const unsigned num_enums = 6;
    static const char* strings[num_enums] = 
    {
        "BOOLEAN",
        "INT",
        "FLOAT",
        "STRING",
        "ARRAY",
        "STRUCT"
    };
    
    unsigned ix = unsigned(json_type);
    
    if (ix < num_enums)
        return strings[ix];
    else
        return "INVALID";
    
}

unsigned json_root_t::parse_array
(
    const std::vector<json_token_t>& tokens,
    unsigned start_ix,
    json_array_t* output
){
    
    // Empty array
    if (tokens[start_ix] == token_type_t::CLOSE_SQUARE)
        return start_ix;
    
    unsigned num_tokens = tokens.size();
    std::vector<json_omni_t*>* values = &(output->values);
    
    // Extract members of the struct until we find a nested struct, nested array
    // or closing curly.
    unsigned ix = start_ix;
    while (true)
    {
        
        // We are expecting the pattern
        // {BOOL_TRUE, BOOL_FALSE, INT, FLOAT, STRING, OPEN_CURLY, OPEN_SQUARE} -> {COMMA, CLOSE_SQUARE}
        // Therefore we need at least 2 additional tokens
        const json_token_t* cur_token = &(tokens[ix]);
        if (ix + 2 >= num_tokens)
        {
            std::cerr
                << "Error parsing json array line "
                << cur_token->line
                << ".  Not enough tokens to form array member\n";
            return 0;
        }
        
        // Value of an element for an array
        cur_token = &(tokens[ix]);
        switch (cur_token->token_type)
        {
            case token_type_t::BOOL_FALSE:
                values->emplace_back(new json_bool_t(false));
                break;
            case token_type_t::BOOL_TRUE:
                values->emplace_back(new json_bool_t(true));
                break;
            case token_type_t::CLOSE_CURLY:
                std::cerr
                    << "Error parsing json array line "
                    << cur_token->line
                    << ".  Unexpected CLOSE_CURLY\n";
                return 0;
            case token_type_t::CLOSE_SQUARE:
                std::cerr
                    << "Error parsing json array line "
                    << cur_token->line
                    << ".  Unexpected CLOSE_SQUARE\n";
                return 0;
            case token_type_t::COLON:
                std::cerr
                    << "Error parsing json array line "
                    << cur_token->line
                    << ".  Unexpected COLON\n";
                return 0;
            case token_type_t::COMMA:
                std::cerr
                    << "Error parsing json array line "
                    << cur_token->line
                    << ".  Unexpected COMMA\n";
                return 0;
            case token_type_t::FLOAT:
                values->emplace_back(new json_float_t(stof(cur_token->value)));
                break;
            case token_type_t::INT:
                try
                {
                    values->emplace_back(new json_int_t(stoi(cur_token->value)));
                } catch (std::out_of_range) {
                    values->emplace_back(new json_int_t(UINT_MAX));
                }
                break;
            case token_type_t::OPEN_CURLY:
            {
                // Allocate and recurse into a nested struct
                json_struct_t* nested_struct = new json_struct_t;
                values->emplace_back(nested_struct);
                unsigned last_parsed_ix = this->parse_struct(tokens, ix + 1, nested_struct);
                if (last_parsed_ix == 0)
                    return 0;
                else
                    ix = last_parsed_ix;
                break;
            }
            case token_type_t::OPEN_SQUARE:
            {
                // Allocate and recurse into another nested array
                json_array_t* nested_array = new json_array_t;
                values->emplace_back(nested_array);
                unsigned last_parsed_ix = this->parse_array(tokens, ix + 1, nested_array);
                if (last_parsed_ix == 0)
                    return 0;
                else
                    ix = last_parsed_ix;
                break;
            }
            case token_type_t::STRING:
                values->emplace_back(new json_string_t(cur_token->value));
                break;
            default:
                // Also should never happen
                std::cerr
                    << "Error.  Unrecognized token type "
                    << token_type_to_string(*cur_token)
                    << '\n';
                return 0;
        }

        // COMMA
        ix++;
        cur_token = &(tokens[ix]);
        switch (cur_token->token_type)
        {
            case token_type_t::COMMA:
                // Prepare to add another element to the same struct
                ix++;
                continue;
            case token_type_t::CLOSE_SQUARE:
                return ix;
            default:
                std::cerr
                    << "Error parsing json array line "
                    << cur_token->line
                    << ".  Expected COMMA or CLOSE_SQUARE but got "
                    << token_type_to_string(*cur_token)
                    << '\n';
                return 0;
        }
        
    }
    
}

unsigned json_root_t::parse_struct
(
    const std::vector<json_token_t>& tokens,
    unsigned start_ix,
    json_struct_t* output
){
    
    // Empty struct
    if (tokens[start_ix] == token_type_t::CLOSE_CURLY)
        return start_ix;
    
    unsigned num_tokens = tokens.size();
    std::vector<std::string>* names = &(output->names);
    std::vector<json_omni_t*>* values = &(output->values);
    
    // Extract members of the struct until we find a nested struct, nested array
    // or closing curly.
    unsigned ix = start_ix;
    while (true)
    {
        
        // We are expecting the pattern
        // STRING -> COLON -> {BOOL_TRUE, BOOL_FALSE, INT, FLOAT, STRING, OPEN_CURLY, OPEN_SQUARE} -> {COMMA, CLOSE_SQUARE}
        // Therefore we need at least 3 additional tokens
        const json_token_t* cur_token = &(tokens[ix]);
        if (ix + 3 >= num_tokens)
        {
            std::cerr
                << "Error parsing json struct line "
                << cur_token->line
                << ".  Not enough tokens to form struct member\n";
            return 0;
        }
        
        if (*cur_token != token_type_t::STRING)
        {
            std::cerr
                << "Error parsing json struct line "
                << cur_token->line
                << ".  Expecting a STRING but got "
                << token_type_to_string(*cur_token)
                << '\n';
            return 0;
        }

        // Name of an element for a struct (STRING)
        names->emplace_back(cur_token->value);

        // COLON
        ix++;
        cur_token = &(tokens[ix]);
        if (*cur_token != token_type_t::COLON)
        {
            std::cerr
                << "Error parsing json struct line "
                << cur_token->line
                << ".  Expecting a COLON but got "
                << token_type_to_string(*cur_token)
                << '\n';
            return 0;
        }

        // Value of an element for a struct
        ix++;
        cur_token = &(tokens[ix]);
        switch (cur_token->token_type)
        {
            case token_type_t::BOOL_FALSE:
                values->emplace_back(new json_bool_t(false));
                break;
            case token_type_t::BOOL_TRUE:
                values->emplace_back(new json_bool_t(true));
                break;
            case token_type_t::CLOSE_CURLY:
                std::cerr
                    << "Error parsing json struct line "
                    << cur_token->line
                    << ".  Unexpected CLOSE_CURLY\n";
                return 0;
            case token_type_t::CLOSE_SQUARE:
                std::cerr
                    << "Error parsing json struct line "
                    << cur_token->line
                    << ".  Unexpected CLOSE_SQUARE\n";
                return 0;
            case token_type_t::COLON:
                std::cerr
                    << "Error parsing json struct line "
                    << cur_token->line
                    << ".  Unexpected COLON\n";
                return 0;
            case token_type_t::COMMA:
                std::cerr
                    << "Error parsing json struct line "
                    << cur_token->line
                    << ".  Unexpected COMMA\n";
                return 0;
            case token_type_t::FLOAT:
                values->emplace_back(new json_float_t(stof(cur_token->value)));
                break;
            case token_type_t::INT:
                try
                {
                    values->emplace_back(new json_int_t(stoi(cur_token->value)));
                } catch (std::out_of_range) {
                    values->emplace_back(new json_int_t(UINT_MAX));
                }
                break;
            case token_type_t::OPEN_CURLY:
            {
                // Allocate and recurse into another nested struct
                json_struct_t* nested_struct = new json_struct_t;
                values->emplace_back(nested_struct);
                unsigned last_parsed_ix = this->parse_struct(tokens, ix + 1, nested_struct);
                if (last_parsed_ix == 0)
                    return 0;
                else
                    ix = last_parsed_ix;
                break;
            }
            case token_type_t::OPEN_SQUARE:
            {
                // Allocate and recurse into a nested array
                json_array_t* nested_array = new json_array_t;
                values->emplace_back(nested_array);
                unsigned last_parsed_ix = this->parse_array(tokens, ix + 1, nested_array);
                if (last_parsed_ix == 0)
                    return 0;
                else
                    ix = last_parsed_ix;
                break;
            }
            case token_type_t::STRING:
                values->emplace_back(new json_string_t(cur_token->value));
                break;
            default:
                // Also should never happen
                std::cerr
                    << "Error.  Unrecognized token type "
                    << token_type_to_string(*cur_token)
                    << '\n';
                return 0;
        }

        // COMMA
        ix++;
        cur_token = &(tokens[ix]);
        switch (cur_token->token_type)
        {
            case token_type_t::COMMA:
                // Prepare to add another element to the same struct
                ix++;
                continue;
            case token_type_t::CLOSE_CURLY:
                return ix;
            default:
                std::cerr
                    << "Error parsing json struct line "
                    << cur_token->line
                    << ".  Expected COMMA or CLOSE_CURLY but got "
                    << token_type_to_string(*cur_token)
                    << '\n';
                return 0;
        }
        
    }
    
}

bool json_root_t::parse(const std::string raw_json_data)
{
    
    // Destroy any previously existing data if any
    
    this->clear();
    
    const std::vector<json_token_t> tokens = tokenize(strip_comments(raw_json_data));
    unsigned num_tokens = tokens.size();
    if (num_tokens == 0)
    {
        std::cerr << "Error.  Could not extract any tokens from json file\n";
        return false;
    }
    
    if (tokens[0] != token_type_t::OPEN_CURLY)
    {
        std::cerr << "Error.  First token in json file was not a '{'\n";
        return false;
    }
    
    this->start_node = new json_struct_t;
    return this->parse_struct(tokens, 1, this->start_node) != 0;
    
}

void json_root_t::copy(const json_root_t& other)
{
    
    // Remember that we assume that start_node == NULL
    
    const json_struct_t* other_start_node = other.start_node;
    
    if (other_start_node == NULL)
        return;
    
    this->start_node = new json_struct_t(*other_start_node);
    
}

void json_root_t::clear()
{
    
    // Polymorphic destructors will handle everything!
    if (this->start_node != NULL)
        delete this->start_node;
    this->start_node = NULL;
    
}

void json_root_t::print_struct(const json_struct_t* cur_node, std::string* output, unsigned indent)
{
    
    const std::vector<std::string>& names = cur_node->names;
    const std::vector<json_omni_t*>& values = cur_node->values;
    unsigned num_elements = values.size();
    
    if (num_elements == 0)
    {
        *output += "{}\n";
        return;
    } else {
        *output += '\n';
        this->print_indent(output, indent);
        *output += "{\n";
    }
    
    indent += 4;
    
    for (unsigned ix = 0; ix < num_elements; ix++)
    {
        
        this->print_indent(output, indent);
        *output += '\"';
        *output += names[ix];
        *output += "\": ";
        
        const json_omni_t* cur_value = values[ix];
        switch(cur_value->get_type())
        {
            case json_type_t::BOOLEAN:
                if (((json_bool_t*)(cur_value))->value)
                {
                    *output += "true\n";
                    continue;
                } else {
                    *output += "false\n";
                    continue;
                }
            case json_type_t::INT:
                *output += std::to_string(((json_int_t*)(cur_value))->value);
                *output += '\n';
                continue;
            case json_type_t::FLOAT:
                *output += std::to_string(((json_float_t*)(cur_value))->value);
                *output += '\n';
                continue;
            case json_type_t::STRING:
                *output += '\"';
                *output += ((json_string_t*)(cur_value))->value;
                *output += "\"\n";
                continue;
            case json_type_t::ARRAY:
                this->print_array((json_array_t*)(cur_value), output, indent);
                continue;
            case json_type_t::STRUCT:
                this->print_struct((json_struct_t*)(cur_value), output, indent);
                continue;
        }
        
    }
    
    this->print_indent(output, indent - 4);
    *output += "}\n";
    
}

void json_root_t::print_array(const json_array_t* cur_node, std::string* output, unsigned indent)
{
    
    const std::vector<json_omni_t*>& values = cur_node->values;
    unsigned num_elements = values.size();
    
    if (num_elements == 0)
    {
        *output += "[]\n";
        return;
    } else {
        *output += '\n';
        this->print_indent(output, indent);
        *output += "[\n";
    }
    
    indent += 4;
    
    for (unsigned ix = 0; ix < num_elements; ix++)
    {
        
        this->print_indent(output, indent);
        
        const json_omni_t* cur_value = values[ix];
        switch(cur_value->get_type())
        {
            case json_type_t::BOOLEAN:
                if (((json_bool_t*)(cur_value))->value)
                {
                    *output += "true\n";
                    continue;
                } else {
                    *output += "false\n";
                    continue;
                }
            case json_type_t::INT:
                *output += std::to_string(((json_int_t*)(cur_value))->value);
                *output += '\n';
                continue;
            case json_type_t::FLOAT:
                *output += std::to_string(((json_float_t*)(cur_value))->value);
                *output += '\n';
                continue;
            case json_type_t::STRING:
                *output += '\"';
                *output += ((json_string_t*)(cur_value))->value;
                *output += "\"\n";
                continue;
            case json_type_t::ARRAY:
                this->print_array((json_array_t*)(cur_value), output, indent);
                continue;
            case json_type_t::STRUCT:
                this->print_struct((json_struct_t*)(cur_value), output, indent);
                continue;
        }
        
    }
    
    this->print_indent(output, indent - 4);
    *output += "]\n";
    
}

void json_array_t::copy(const json_array_t& other)
{
    
    // Recall that we can assume that this array has already been cleared
    
    // Copy the name of the array
    this->name = other.name;
    
    // Resize the current array to the same dimensions as the other one
    unsigned num_values = other.values.size();
    this->values.resize(num_values);
    
    for (unsigned ix = 0; ix < num_values; ix++)
    {
        
        const json_omni_t* cur_other_omni = other.values[ix];
        if (cur_other_omni == NULL)
        {
            this->values[ix] = NULL;
        } else {
            // Determine the type of each source and recursivly construct an
            // object of the correct datatype
            switch (cur_other_omni->get_type())
            {
                case json_type_t::BOOLEAN:
                    this->values[ix] = new json_bool_t(*((json_bool_t*)(cur_other_omni)));
                    break;
                case json_type_t::INT:
                    this->values[ix] = new json_int_t(*((json_int_t*)(cur_other_omni)));
                    break;
                case json_type_t::FLOAT:
                    this->values[ix] = new json_float_t(*((json_float_t*)(cur_other_omni)));
                    break;
                case json_type_t::STRING:
                    this->values[ix] = new json_string_t(*((json_string_t*)(cur_other_omni)));
                    break;
                case json_type_t::ARRAY:
                    this->values[ix] = new json_array_t(*((json_array_t*)(cur_other_omni)));
                    break;
                case json_type_t::STRUCT:
                    this->values[ix] = new json_struct_t(*((json_struct_t*)(cur_other_omni)));
                    break;
            }
        }
        
    }
    
    
}

void json_array_t::clear()
{
    
    // No need to delete the array name
    
    // Recursivly delete child nodes
    unsigned num_values = this->values.size();
    for (unsigned ix = 0; ix < num_values; ix++)
    {
        json_omni_t* cur_value = this->values[ix];
        if (cur_value != NULL)
            delete cur_value;
    }
    
    // Delete the array itself
    this->values.clear();
    
}

void json_struct_t::copy(const json_struct_t& other)
{
    
    // Recakk that we can assume that any old content has already been cleared
    
    unsigned num_values = other.values.size();
    
    for (unsigned ix = 0; ix < num_values; ix++)
    {
        
        // Copy element name
        this->names[ix] = other.names[ix];
        
        // Determine value type and create a new copy
        const json_omni_t* cur_other_omni = other.values[ix];
        if (cur_other_omni == NULL)
        {
            this->values[ix] = NULL;
        } else {
            // Determine the type of each source and recursivly construct an
            // object of the correct datatype
            switch (cur_other_omni->get_type())
            {
                case json_type_t::BOOLEAN:
                    this->values[ix] = new json_bool_t(*((json_bool_t*)(cur_other_omni)));
                    break;
                case json_type_t::INT:
                    this->values[ix] = new json_int_t(*((json_int_t*)(cur_other_omni)));
                    break;
                case json_type_t::FLOAT:
                    this->values[ix] = new json_float_t(*((json_float_t*)(cur_other_omni)));
                    break;
                case json_type_t::STRING:
                    this->values[ix] = new json_string_t(*((json_string_t*)(cur_other_omni)));
                    break;
                case json_type_t::ARRAY:
                    this->values[ix] = new json_array_t(*((json_array_t*)(cur_other_omni)));
                    break;
                case json_type_t::STRUCT:
                    this->values[ix] = new json_struct_t(*((json_struct_t*)(cur_other_omni)));
                    break;
            }
        }
        
    }
    
}

void json_struct_t::clear()
{
    
    // Delete names
    this->names.clear();
    
    // Delete all child nodes
    unsigned num_values = this->values.size();
    for (unsigned ix = 0; ix < num_values; ix++)
    {
        delete this->values[ix];
    }
    
    // Dekete remaining storage
    this->values.clear();
    
}

