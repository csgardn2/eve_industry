/// @file regional_market.h
/// @brief Declaration of @ref regional_market_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday November 7 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_REGIONAL_MARKET
#define HEADER_GUARD_REGIONAL_MARKET

#include <fstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "error.h"
#include "station_market.h"
#include "json.h"

class raw_regional_market_t;

/// @brief Contains all of the buy and sell orders at a particular region.
class regional_market_t
{
        
    public:
        
        /// @brief Default constructor
        inline regional_market_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.
        
        /// @brief Iterator to the first element in this container.
        inline std::unordered_map<uint64_t, station_market_t>::const_iterator begin() const
        {
            return this->stations_.begin();
        }
        
        /// @brief Iterator to the first element in this container.
        inline std::unordered_map<uint64_t, station_market_t>::iterator begin()
        {
            return this->stations_.begin();
        }
        
        /// @brief Iterator one past the end of this container
        inline std::unordered_map<uint64_t, station_market_t>::const_iterator end() const
        {
            return this->stations_.end();
        }
        
        /// @brief Iterator one past the end of this container
        inline std::unordered_map<uint64_t, station_market_t>::iterator end()
        {
            return this->stations_.end();
        }
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        inline uint64_t region_id() const
        {
            return this->region_id_;
        }
        
        inline void region_id(uint64_t new_region_id)
        {
            this->region_id_ = new_region_id;
        }
        
        const std::unordered_map<uint64_t, station_market_t>& stations() const
        {
            return this->stations_;
        }
        
        // Add special-purpose functions here
        
        inline void clear()
        {
            this->stations_.clear();
        }
        
        inline void add_new_station_market(const station_market_t& new_station_market)
        {
            this->stations_.emplace(new_station_market.station_id(), new_station_market);
        }
        
        /// @brief Search for an station with a given ID.  If that station market
        /// already existed, return a reference to that station.  If the station didn't
        /// exist, allocate a new empty market for that station and return a reference
        /// to the empty market.
        inline station_market_t& find_or_allocate(uint64_t station_id)
        {
            return this->stations_[station_id];
        }
        
        /// @brief Clear previous content and sort raw orders from a region into
        /// the corresponding @ref station_market_t "station" and
        /// @ref item_market_t "item" markets.
        void initialize_from_raw_regional_market(const raw_regional_market_t& raw_regional_market);
        
        /// @brief Remove all market data that is not at one of the stations
        /// we're interested in
        void cull_by_station(const std::unordered_set<uint64_t>& stations_to_keep);
        
        /// @brief Open a file conforming to data/json/schema.json and use it to initialize
        /// this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_json_file(std::istream& file);
        
        /// @brief Decode serialized content conforming to data/json/schema.json and use it
        /// to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_json_buffer(std::string_view buffer);
        
        /// @brief Extract required data fields from a pre-parsed JSON tree
        /// and use them to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        void read_from_json_json(const Json::Value& json_root);
        
        /// @brief Serialize the content of this file into a file that
        /// conforms to the schema data/json/schema.json.
        void write_to_json_file
        (
            /// [out] Stream to append serialized object content to.
            std::ostream& file,
            /// [in] The number of space ' ' characters to prepend to each line
            /// in the serialized output.
            unsigned indent_start = 0,
            /// [in] The number of additional spaces ' ' on top of indent_start
            /// to prepend to each line in the serialized output for each
            /// additional level of nesting.
            ///
            /// Indentation typically increases after each opening bracket '['
            /// or brace '{' and decreases after each closing bracket ']' or
            /// brace '}'.
            unsigned spaces_per_tab = 4
        ) const;
        
        /// @brief Serialize the content of this file into a string that
        /// conforms to the schema data/json/schema.json.
        ///
        /// @exception error_message_t
        void write_to_json_buffer
        (
            /// [out] This string is overwritten with serialzed JSON content.
            std::string& buffer,
            /// [in] The number of space ' ' characters to prepend to each line
            /// in the serialized output.
            unsigned indent_start = 0,
            /// [in] The number of additional spaces ' ' on top of indent_start
            /// to prepend to each line in the serialized output for each
            /// additional level of nesting.
            ///
            /// Indentation typically increases after each opening bracket '['
            /// or brace '{' and decreases after each closing bracket ']' or
            /// brace '}'.
            unsigned spaces_per_tab = 4
        ) const;
        
        /// @brief Convinence method for pretty initialize-on-construction
        /// syntax.
        inline std::string write_to_json_buffer
        (
            /// [in] The number of space ' ' characters to prepend to each line
            /// in the serialized output.
            unsigned indent_start = 0,
            /// [in] The number of additional spaces ' ' on top of indent_start
            /// to prepend to each line in the serialized output for each
            /// additional level of nesting.
            ///
            /// Indentation typically increases after each opening bracket '['
            /// or brace '{' and decreases after each closing bracket ']' or
            /// brace '}'.
            unsigned spaces_per_tab = 4
        ) const {
            std::string buffer;
            this->write_to_json_buffer(buffer, indent_start, spaces_per_tab);
            return buffer;
        }
        
    protected:
        
        // Try to make your members protected, even if they don't have to be.
        
        /// @brief The integral uniquifier for this region, as used by the EvE
        /// Swagger API.
        uint64_t region_id_;
        
        /// @brief All of the stations with a market hub in this region are
        /// listed here.
        ///
        /// Each station ID is replicated as a dictionary key for fast lookup.
        std::unordered_map<uint64_t, station_market_t> stations_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const regional_market_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, regional_market_t& destination);

#endif // Header Guard

