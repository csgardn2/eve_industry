/// \file
/// File Name:                      fetch_order_list.h \n
/// Date created:                   Sat Jan 7 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_FETCH_ORDER_LIST
#define HEADER_GUARD_FETCH_ORDER_LIST

/// Contains info about a single market order in the EvE market.
class order_t
{
    
    public:
        
        /// \brief True if an order is a sell order and false otherwise
        bool is_sell;
        
        /// \brief The amount if ISK offered or requested for one of these items.
        float price;
        
        /// \brief The number of items remaining in the order
        unsigned quantity;
        
        /// \brief Item ID as used by the CREST API.  See \ref fetch_item_ids.
        unsigned item_id;
        
        /// \brief The station ID as used by the CREST API
        unsigned station_id;
        
};

/// \brief Pull all the market data for a particular region.
std::vector<order_t> fetch_orders
(
    
    /// [in] CREST root web address.  Usually https://crest-tq.eveonline.com.
    const std::string& eve_api_root,
    
    /// [in] The name of a region such as "The Forge" or "Domain".  Case is ignored.
    const std::string& region,
    
    /// [in] Setting this to zero means no stations are filtered and the
    /// entire regional market is saved
    unsigned station_id = 0
    
);

/// \brief Look for a file containing item orders.  If it exists, then load the
/// orders from that file.  If it doesn't, load the orders from CREST and write
/// them to a file.
std::vector<order_t> fetch_orders_through_cache
(
    const std::string& order_cache_file_name,
    const std::string& eve_api_root,
    const std::string& region,
    unsigned station_id
);

#endif // header guard

