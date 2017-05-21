/// \file
/// File Name:                      read_blueprints_file.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or parse_blueprint_list.h for documentation.

#include <cmath>
#include <ctgmath>
#include <iostream>
#include <string>
#include <vector>

#include "blueprint.h"
#include "json.h"
#include "utility.h"

/// \brief Similar to \ref walk_struct except this function will extract either
/// \ref json_type_t "INT" or \ref json_type_t "FLOAT" with a given name.
/// If the item is not found, then NAN is returned.
float extract_real(const json_struct_t* node, const std::string& search_name)
{
    
    if (node == NULL)
        return NAN;
    
    unsigned num_members = node->names.size();
    for (unsigned ix = 0; ix < num_members; ix++)
    {
        
        if (node->names[ix] != search_name)
            continue;
        
        const json_omni_t* cur_member = node->values[ix];
        switch (cur_member->get_type())
        {
            case json_type_t::FLOAT:
                return ((json_float_t*)(cur_member))->value;
            case json_type_t::INT:
                return (float)(((json_int_t*)(cur_member))->value);
            default:
                return NAN;
        }
        
    }
    
    return NAN;
    
}

/// \brief Take a single node which may be either \ref json_type_t "INT" or \ref
/// json_type_t "FLOAT" and cast its value to a float.  Returns NAN if
/// the input node is not an INT nor a FLOAT.
float extract_real(const json_omni_t* node)
{
    
    if (node == NULL)
        return NAN;
    
    switch (node->get_type())
    {
        case json_type_t::FLOAT:
            return ((json_float_t*)(node))->value;
        case json_type_t::INT:
            return float(((json_int_t*)(node))->value);
        default:
            return NAN;
    }
    
}

std::vector<blueprint_t> read_blueprints_file(std::ifstream& blueprints_file)
{
    
    std::vector<blueprint_t> ret;
    
    // Dump the contents of the entire file into RAM
    std::string raw_json_data = read_file(blueprints_file);
    if (raw_json_data.size() == 0)
        return ret;
    
    // Parse the file into a JSON tree
    json_root_t structured_json_data;
    if (!structured_json_data.parse(raw_json_data))
        return ret;
    
    // Locate the array which holds all the blueprints
    const json_array_t* blueprints_array = (json_array_t*)walk_struct
    (
        structured_json_data.start_node,
        "items",
        json_type_t::ARRAY
    );
    if (blueprints_array == NULL)
    {
        std::cerr << "Error.  Could not find \"items\" array in blueprints file\n";
        return ret;
    }
    
    unsigned num_blueprints = blueprints_array->values.size();
    for (unsigned ix = 0; ix < num_blueprints; ix++)
    {
        
        // Select one blueprint from the array
        if (blueprints_array->values[ix]->get_type() != json_type_t::STRUCT)
            continue;
        const json_struct_t* cur_blueprint = (json_struct_t*)(blueprints_array->values[ix]);
        
        blueprint_t accumulator;
        
        // Extract output item name
        const json_string_t* name = (json_string_t*)walk_struct(cur_blueprint, "name", json_type_t::STRING);
        if (name == NULL)
        {
            std::cerr << "Warning.  Skipping blueprint without a name in blueprints file\n";
            continue;
        }
        accumulator.name = name->value;
        
        const json_string_t* availability = (json_string_t*)walk_struct(cur_blueprint, "availability", json_type_t::STRING);
        if (availability == NULL)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with non-existant \"availability\" member\n";
            continue;
        }
        if (availability->value == "available")
            accumulator.availability = availability_t::AVAILABLE;
        else if (availability->value == "manufacturing")
            accumulator.availability = availability_t::MANUFACTURING;
        else if (availability->value == "selling")
            accumulator.availability = availability_t::SELLING;
        else if (availability->value == "unavailable")
            accumulator.availability = availability_t::UNAVAILABLE;
        else
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid \"availability\" value \""
                << availability->value
                << "\".  Valid options are \"available\", \"manufacturing\", \"selling\", or \"unavailable\"\n";
            continue;
        }
        
        // Extract manufacturing time
        float time = extract_real(cur_blueprint, "time");
        if (std::isnan(time) != 0)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid or non-existant \"time\" member\n";
            continue;
        }
        accumulator.time = time;
        
        // Extract output volume
        float volume = extract_real(cur_blueprint, "volume");
        if (std::isnan(volume) != 0)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid or non-existant \"volume\" member\n";
            continue;
        }
        accumulator.volume = volume;
        
        // Extract output quantity
        float output_quantity = extract_real(cur_blueprint, "output quantity");
        if (std::isnan(output_quantity) != 0)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid or non-existant \"output quantity\" member\n";
            continue;
        }
        accumulator.output_quantity = output_quantity;
        
        // Extract material efficiency
        float me = extract_real(cur_blueprint, "me");
        if (std::isnan(me) != 0)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid or non-existant \"me\" member\n";
            continue;
        }
        accumulator.me = me;
         
        // Extract time efficiency
        float te = extract_real(cur_blueprint, "te");
        if (std::isnan(te) != 0)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" with invalid or non-existant \"te\" member\n";
            continue;
        }
        accumulator.te = te;
        
        // Locate input materials and input quantities arrays
        const json_array_t* input_materials = (json_array_t*)walk_struct(cur_blueprint, "input materials", json_type_t::ARRAY);
        if (input_materials == NULL)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "without \"input materials\" array\n.";
            continue;
        }
        const json_array_t* input_quantities = (json_array_t*)walk_struct(cur_blueprint, "input quantities", json_type_t::ARRAY);
        if (input_materials == NULL)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "without \"input quantities\" array\n.";
            continue;
        }
        
        unsigned num_input_materials = input_materials->values.size();
        unsigned num_input_quantities = input_quantities->values.size();
        if (num_input_materials != num_input_quantities)
        {
            std::cerr
                << "Warning.  Skipping blueprint \""
                << accumulator.name
                << "\" due to mismatch in \"input materials\" array size ("
                << num_input_materials
                << ") and \"input quantities\" array size ("
                << num_input_quantities
                << ").\n";
            continue;
        }
        
        // Parse the contents of the input materials and input quantities arrays.
        // Both sizes are now guarenteed to be the same.
        bool error = false;
        for (unsigned ix = 0; ix < num_input_materials; ix++)
        {
            
            // Extract a material name
            if (input_materials->values[ix]->get_type() != json_type_t::STRING)
            {
                std::cerr
                    << "Warning.  Blueprint \""
                    << accumulator.name
                    << "\"->\"input names\"["
                    << ix
                    << "] was not of type STRING.\n";
                error = true;
                break;
            }
            const std::string& cur_material_name = ((json_string_t*)(input_materials->values[ix]))->value;
            accumulator.input_materials.push_back(cur_material_name);
            
            // Extract a meterial quantity
            float cur_material_quantity = extract_real(input_quantities->values[ix]);
            if (std::isnan(cur_material_quantity) != 0)
            {
                std::cerr
                    << "Warning.  Blueprint \""
                    << accumulator.name
                    << "\"->\"input quantities\"["
                    << ix
                    << "] could not be parsed.\n";
                error = true;
                break;
            }
            
            accumulator.input_quantities.push_back(cur_material_quantity);
            
        }
        if (error)
            continue;
        
        // At this point, the accumulator has been fully initialized.  Commit it
        // to the blueprint array
        ret.push_back(accumulator);
        
    }
    
    return ret;
    
}
