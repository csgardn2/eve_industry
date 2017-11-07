/// @file item_attributes.cpp
/// @brief Implementation of @ref item_attributes_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday October 20 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <iostream>
#include <memory>
#include <omp.h>
#include <string>
#include <string_view>
#include <vector>

#include "item_attributes.h"
#include "item_ids.h"
#include "error.h"
#include "json.h"

void item_attributes_t::fetch(const item_ids_t& item_ids)
{
    
    this->items_.clear();
    
    unsigned num_item_ids = item_ids.ids().size();
    
    // Pre-allocate all storage so re-allocation won't happen while multiple omp
    // threads are writing to it.
    this->items_.resize(num_item_ids);
    
    // This JSON library is kindof weird.  Allocate a json parser outside the
    // loop to avoid constructing a new one each iteration.
    // The smart pointer allows proper de-allocation if an exception is thrown.
    Json::CharReaderBuilder builder;
    
    unsigned num_threads = omp_get_max_threads();
    if (this->debug_mode_.verbose())
        std::cout << "Fetching item attributes using " << num_threads << " threads.\n";
    
    std::vector< std::unique_ptr<Json::CharReader> > readers;
    for (unsigned ix = 0; ix < num_threads; ix++)
        readers.emplace_back(builder.newCharReader());
    
    std::string progress_message;
    
    // Fetch each requested item ID
    unsigned percent = num_item_ids / 100;
    unsigned progress_threshold = percent;
    unsigned num_items_processed = 0;
    #pragma omp parallel for
    for (unsigned ix = 0; ix < num_item_ids; ix++)
    {
        
        unsigned thread_ix = omp_get_thread_num();
        
        // Fetch this item's attributes from EvE API (connect to network).
        this->items_[ix].fetch(item_ids.ids()[ix], readers[thread_ix].get());
        
        #pragma omp atomic
        num_items_processed++;
        
        // Print progress bar
        if (this->debug_mode_.verbose() && num_items_processed == progress_threshold)
        {
            
            // Clear previous message
            for (unsigned iy = 0, bound = progress_message.length(); iy < bound; iy++)
                std::cout << '\b';
            
            // Generate and print progress message
            progress_message = "Fetched ";
            progress_message += std::to_string(num_items_processed);
            progress_message += " / ";
            progress_message += std::to_string(num_item_ids);
            progress_message += ' ';
            progress_message += std::to_string(100 * num_items_processed / num_item_ids);
            progress_message += '%';
            
            #pragma omp critical
            std::cout << progress_message << std::flush;
            
            // Set next milestone for printing.
            progress_threshold += percent;
            
        }
        
    }
    
    if (this->debug_mode_.verbose())
        std::cout << '\n';
    
}

void item_attributes_t::read_from_file(std::istream& file)
{
    
    // Get the number of characters in the input file.
    if (!file.good())
        throw error_message_t(error_code_t::FILE_SIZE_FAILED, "Error.  Failed to determine file size when decoding item_attributes_t object.\n");
    file.seekg(0, std::ios_base::end);
    unsigned file_size = file.tellg();
    file.seekg(0, std::ios_base::beg);
    
    // Read the entire file into RAM at once
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_READ_FAILED, "Error.  Failed to read file when decoding item_attributes_t object.\n");
    this->read_from_buffer(std::string_view(buffer));
    
}

void item_attributes_t::read_from_buffer(std::string_view buffer)
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
    this->read_from_json(json_root);
    
}

void item_attributes_t::read_from_json(const Json::Value& json_root)
{
    
    // Parse root
    if (!json_root.isArray())
        throw error_message_t(error_code_t::JSON_SCHEMA_VIOLATION, "Error.  Root of item_attributes is not of type \"array\".\n");
    
    // Clear previous content
    this->items_.clear();
    this->items_.reserve(json_root.size());
    
    // Decode and store each element in the array.
    for (const Json::Value& cur_element : json_root)
    {
        item_attribute_t new_item_attribute;
        new_item_attribute.read_from_json(cur_element);
        this->items_.emplace_back(new_item_attribute);
    }
    
}

void item_attributes_t::write_to_file(std::ostream& file, unsigned indent_start, unsigned spaces_per_tab) const
{
    file << this->write_to_buffer(indent_start, spaces_per_tab);
    if (!file.good())
        throw error_message_t(error_code_t::FILE_WRITE_FAILED, "Error.  Failed to write file when encoding item_attributes_t object.");
}

void item_attributes_t::write_to_buffer(std::string& buffer, unsigned indent_start, unsigned spaces_per_tab) const
{
    
    std::string indent_1(indent_start + 1 * spaces_per_tab, ' ');
    std::string_view indent_0(indent_1.data(), indent_start);
    
    // It is recommended not to start a new line before the opening brace, to
    // enable chaining.
    buffer += "[\n";
    buffer += indent_1;
    
    // Encode member variables.
    for (signed ix = 0, last_ix = this->items_.size() - 1; ix <= last_ix; ix++)
    {
        
        buffer += this->items_[ix].write_to_buffer(indent_start + spaces_per_tab, spaces_per_tab);
        
        if (ix != last_ix)
            buffer += ", ";
        
    }
    
    // It is recommended to not put a newline on the last brace to allow
    // comma chaining when this object is an element of an array.
    buffer += '\n';
    buffer += indent_0;
    buffer += "]";
    
}

std::istream& operator>>(std::istream& stream, item_attributes_t& destination)
{
    try
    {
        destination.read_from_file(stream);
    } catch (error_message_t error) {
        stream.setstate(std::ios::failbit);
        throw error;
    } catch (Json::Exception error) {
        stream.setstate(std::ios::failbit);
        throw error;
    }
    return stream;
}

