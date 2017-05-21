/// \file
/// File Name:                      fetch_item_ids.h \n
/// Date created:                   Thurs Jan 5 2017 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_FETCH_ITEM_IDS
#define HEADER_GUARD_FETCH_ITEM_IDS

#include <string>
#include <vector>

/// \brief Associates an item with the ID number used in the EVE CREST API
class item_id_pair_t
{
    public:
        
        item_id_pair_t() = default;
        
        /// \brief Initialization constructor
        inline item_id_pair_t(const char* name_, unsigned id_)
            : name(name_), id(id_)
        {
            // Intentionally empty
        }
        
        /// \brief Initialization constructor
        inline item_id_pair_t(const std::string& name_, unsigned id_)
            : name(name_), id(id_)
        {
            // Intentionally empty
        }
        
        /// \brief Name of an item, exactly as it is in the EVE API
        std::string name;
        
        /// \brief Integer ID of an item, as it is in the EVE API
        unsigned id;
        
};

/// \brief Fetch as many item IDs as possible from the EVE Crest Server
std::vector<item_id_pair_t> fetch_item_ids(const std::string& eve_api_root);

/// \brief Look for a file containing item IDs.  If it exists, then load IDs
/// from that file.  If it doesn't, load the IDs from CREST and write them to
/// a file.
std::vector<item_id_pair_t> fetch_item_ids_through_cache
(
    const std::string& item_id_cache_file_name,
    const std::string& eve_api_root
);

#endif // header guard

