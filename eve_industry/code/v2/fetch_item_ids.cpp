/// \file
/// File Name:                      fetch_item_ids.cpp \n
/// Date created:                   Thurs Jan 5 2017 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 -fopenmp \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or fetch_item_ids.h for documentation.

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "fetch_item_ids.h"
#include "ftps_get.h"
#include "json.h"
#include "utility.h"

/// \brief Return value for parse_market_group_item.  Elements from a \ref
/// json_struct_t are collected here
class market_group_t
{
    
    public:
        
        /// Default constructor
        inline market_group_t()
            : valid(false)
        {
            // Intentionally left blank
        }
        
        /// Copy constructor
        inline market_group_t(const market_group_t& other) = default;
        
        /// Initialization constructor
        inline market_group_t(const std::string* href_, unsigned parent_id_, unsigned group_id_)
            : href(href_), parent_id(parent_id_), group_id(group_id_), valid(true)
        {
            // Intentionally left blank
        }
        
        /// \brief Item -> types -> href.  This points to an item within an
        /// externally allocated \ref json_root_t.
        const std::string *href;
        
        /// \brief Item -> parentGroup -> id
        unsigned parent_id;
        
        /// \brief Item -> id
        unsigned group_id;
        
        /// \brief Set to true only if all 3 of \ref href, 
        bool valid;
        
};

/// \brief Parse the group id, parent group id, and href from a market group
market_group_t parse_market_group_item(const json_struct_t* array_item)
{
    
    // Group ID
    const json_int_t* group_id = (json_int_t*)walk_struct(array_item, "id", json_type_t::INT);
    if (group_id == NULL)
        return market_group_t();
    
    // Parent Group ID
    const json_struct_t* parent_group = (json_struct_t*)walk_struct(array_item, "parentGroup", json_type_t::STRUCT);
    if (parent_group == NULL)
        return market_group_t();
    const json_int_t* parent_id = (json_int_t*)walk_struct(parent_group, "id", json_type_t::INT);
    if (parent_id == NULL)
        return market_group_t();
    
    // Href
    const json_struct_t* types = (json_struct_t*)walk_struct(array_item, "types", json_type_t::STRUCT);
    if (types == NULL)
        return market_group_t();
    const json_string_t* href = (json_string_t*)walk_struct(types, "href", json_type_t::STRING);
    if (href == NULL)
        return market_group_t();
    
    // All items collected successfully
    return market_group_t(&(href->value), parent_id->value, group_id->value);
    
}

/// \brief I farted
/// \return The number of items added to the output vector
unsigned parse_leaf_group
(
    
    /// [in] The json file stored at a "good href" (see \ref fetch_item_ids)
    const json_root_t& root,
    
    /// [out] Items and their IDs are appended to this vector
    std::vector<item_id_pair_t>* output
    
){
    
    const json_struct_t* start_node = root.start_node;
    if (start_node == NULL)
        return 0;
    
    const json_array_t* items = (json_array_t*)walk_struct(start_node, "items", json_type_t::ARRAY);
    if (items == NULL)
        return 0;
    
    unsigned num_items = items->values.size();
    unsigned num_found_items = 0;
    for (unsigned ix = 0; ix < num_items; ix++)
    {
        
        // Select a single struct from the items array
        if (items->values[ix]->get_type() != json_type_t::STRUCT)
            continue;
        const json_struct_t* cur_element = (json_struct_t*)(items->values[ix]);
        
        // Step into the "type" member of the current struct
        const json_struct_t* item_types = (json_struct_t*)walk_struct(cur_element, "type", json_type_t::STRUCT);
        if (item_types == NULL)
            continue;
        
        // Find the item name within the "type" sub-structure
        const json_string_t* name = (json_string_t*)walk_struct(item_types, "name", json_type_t::STRING);
        if (name == NULL)
            continue;
        
        // Find the item id within the "type" sub-structure
        const json_int_t* id = (json_int_t*)walk_struct(item_types, "id", json_type_t::INT);
        if (id == NULL)
            continue;
        
        num_found_items++;
        output->emplace_back(name->value, id->value);
        
    }
    
    return num_found_items;
    
}

std::vector<item_id_pair_t> fetch_item_ids(const std::string& eve_api_root)
{
    
    std::vector<item_id_pair_t> ret;
    
    // Fetch the EvE root JSON file
    std::string raw_json_data = ftps_get(eve_api_root);
    json_root_t structured_json_data;
    if (!structured_json_data.parse(raw_json_data))
        return ret;
    
    // Fetch marketGroups
    const json_struct_t* raw_market_groups = (json_struct_t*)walk_struct(structured_json_data.start_node, "marketGroups", json_type_t::STRUCT);
    if (raw_market_groups == NULL)
    {
        std::cerr << "Error.  Failed to find \"marketGroups\" inside EvE ROOT\n";
        return ret;
    }
    const json_string_t* market_groups_href = (json_string_t*)walk_struct(raw_market_groups, "href", json_type_t::STRING);
    if (market_groups_href == NULL)
    {
        std::cerr << "Error.  Failed to find \"href\" inside EvE Root -> marketGroups\n";
        return ret;
    }
    raw_json_data = ftps_get(market_groups_href->value);
    if (!structured_json_data.parse(raw_json_data))
        return ret;
    
    // Walk to the JSON array containing all the market groups
    const json_array_t* items = (json_array_t*)walk_struct(structured_json_data.start_node, "items", json_type_t::ARRAY);
    if (items == NULL)
    {
        std::cerr << "Error.  Failed to find \"items\" array in \"marketGroups\" page\n";
        return ret;
    }
    
    // Collect the useful data from all these structs
    unsigned num_raw_items = items->values.size();
    std::vector<market_group_t> parsed_market_groups;
    parsed_market_groups.reserve(num_raw_items);
    for (unsigned ix = 0; ix < num_raw_items; ix++)
    {
        if (items->values[ix]->get_type() != json_type_t::STRUCT)
            continue;
        market_group_t market_group = parse_market_group_item((json_struct_t*)(items->values[ix]));
        if (market_group.valid)
            parsed_market_groups.emplace_back(market_group);
    }
    
    // Determine which market groups are leaf nodes
    // id_map maps a group ID back to its index inside the parsed_market_groups vector
    std::unordered_map<unsigned, unsigned> id_map;
    unsigned num_parsed_items = parsed_market_groups.size();
    for (unsigned ix = 0; ix < num_parsed_items; ix++)
        id_map.insert(std::pair<unsigned, unsigned>(parsed_market_groups[ix].group_id, ix));
    // Flad which market groups are leaf nodes.  Following these href links will
    // lead to actual items and their IDs
    std::vector<bool> are_leaves(num_parsed_items, true);
    
    const std::unordered_map<unsigned, unsigned>::const_iterator not_found = id_map.end();
    for (unsigned ix = 0; ix < num_parsed_items; ix++)
    {
        unsigned parent_id = parsed_market_groups[ix].parent_id;
        const std::unordered_map<unsigned, unsigned>::const_iterator match = id_map.find(parent_id);
        if (match != not_found)
            are_leaves[match->second] = false;
    }
    
    // Now that we know which hrefs are good, collect them all into a vector
    std::vector<std::string> good_hrefs;
    for (unsigned ix = 0; ix < num_parsed_items; ix++)
    {
        if (are_leaves[ix])
            good_hrefs.emplace_back(*(parsed_market_groups[ix].href));
    }
    
    // Perform a massive data pull from the EvE servers by download each JSON
    // file at each href we found
    unsigned num_good_hrefs = good_hrefs.size();
    unsigned processed_hrefs = 0;
    unsigned cursor_pos = 0;
    
    std::string total_hrefs_str = std::to_string(num_good_hrefs);
    std::string processed_hrefs_str;
    
    #pragma omp parallel for
    for (unsigned ix = 0; ix < num_good_hrefs; ix++)
    {
        
        // Fetch and decode a market group
        const std::string cur_raw = ftps_get(good_hrefs[ix]);
        json_root_t cur_structured;
        if (!cur_structured.parse(cur_raw))
            continue;
        
        #pragma omp critical
        {
            
            parse_leaf_group(cur_structured, &ret);
            processed_hrefs++;
            
            // Print status to screen
            for (unsigned iy = 0; iy < cursor_pos; iy++)
                std::cout << '\b';
            processed_hrefs_str = std::to_string(processed_hrefs);
            std::cout << processed_hrefs_str << '/' << total_hrefs_str << std::flush;
            cursor_pos = processed_hrefs_str.length() + total_hrefs_str.length() + 1;
            
        }
        
    }
    
    std::cout << '\n';
    
    return ret;
    
}

/// \brief Parse through a CSV file whose elements are 
///     "name", id
///     "name", id
/// until the file is depleted or an error occurs.  The item ID pairs are then
/// returned.
std::vector<item_id_pair_t> read_item_id_cache(std::ifstream& cache_reader)
{
    
    // Dump the entire file content into a string
    std::string raw_file_contents = read_file(cache_reader);
    std::vector<item_id_pair_t> ret;
    if (raw_file_contents.empty())
        return ret;
    
    // Content of the file should be:
    //      "quoted string", id
    // separated by newlines
    unsigned last_ix = raw_file_contents.length() - 1;
    unsigned line = 1;
    unsigned ix = 0;
    std::string cur_id;
    while (true)
    {
        
        // Find the opening quote of an item name
        unsigned name_start_quote_ix = ix;
        while (true)
        {
            if (name_start_quote_ix >= last_ix)
                return ret;
            char c = raw_file_contents[name_start_quote_ix];
            if (c == '\n')
                line++;
            else if (c == '\"')
                break;
            name_start_quote_ix++;
        }
        
        // Find the closing quote of an item name
        unsigned name_end_quote_ix = name_start_quote_ix + 1;
        while (true)
        {
            if (name_end_quote_ix >= last_ix)
                return ret;
            char c = raw_file_contents[name_end_quote_ix];
            if (c == '\n')
                line++;
            else if (c == '\"')
                break;
            name_end_quote_ix++;
        }
        
        unsigned name_length = name_end_quote_ix - name_start_quote_ix - 1;
        if (name_length == 0)
        {
            std::cerr
                << "Error reading item id cache.  Encountered empty quote on line "
                << line
                << ".\n";
            return ret;
        }
        
        // Find the first character of the item ID integer
        unsigned id_start_ix = name_end_quote_ix + 1;
        while (true)
        {
            if (id_start_ix >= last_ix)
                return ret;
            char c = raw_file_contents[id_start_ix];
            if (c == '\n')
            {
                line++;
                break;
            } else if (isdigit(c) != 0) {
                break;
            }
            id_start_ix++;
        }
        
        // Find the last character of the item ID integer
        unsigned id_end_ix = id_start_ix + 1;
        while (true)
        {
            if (id_end_ix >= last_ix)
                return ret;
            char c = raw_file_contents[id_end_ix];
            if (c == '\n')
            {
                line++;
                break;
            } else if (isdigit(c) == 0) {
                break;
            }
            id_end_ix++;
        }
        id_end_ix--;
        unsigned id_length = id_end_ix - id_start_ix + 1;
        
        // Emplace the name-ID pair into the end of the returned vector
        cur_id.assign(raw_file_contents, id_start_ix, id_length);
        ret.emplace_back
        (
            std::string(raw_file_contents, name_start_quote_ix + 1, name_length),
            std::stoi(cur_id)
        );
        
        ix = id_end_ix + 1;
        
    }
    
}

std::vector<item_id_pair_t> fetch_item_ids_through_cache
(
    const std::string& item_id_cache_file_name,
    const std::string& eve_api_root
){
    
    std::vector<item_id_pair_t> ret;
    
    // Try to open the cache for reading
    std::ifstream cache_reader(item_id_cache_file_name);
    if (cache_reader.good())
    {
        
        // Cache was opened.  Read it and return it.
        std::vector<item_id_pair_t> ret = read_item_id_cache(cache_reader);
        return ret;
        
    } else {
        
        std::cout
            << "Could not find item ID cache.  Downloading it from \""
            << eve_api_root
            << "\".  This could take a while.\n";
        
        // Cache doesn't exist.  Pull data from CREST
        std::vector<item_id_pair_t> ret = fetch_item_ids(eve_api_root);
        std::cout
            << "Finished download.  Writing to cache \""
            << item_id_cache_file_name
            << "\".\n";
        
        // Write IDs to cache
        if (cache_reader.is_open())
            cache_reader.close();
        std::ofstream cache_writer(item_id_cache_file_name);
        
        unsigned num_ids = ret.size();
        for (unsigned ix = 0; ix < num_ids; ix++)
            cache_writer << '\"' << ret[ix].name << "\", " << ret[ix].id << '\n';
        
        return ret;
        
    }
    
}

