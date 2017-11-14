/// @file raw_regional_market.h
/// @brief Declaration of @ref raw_regional_market_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 3 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_RAW_REGIONAL_MARKET
#define HEADER_GUARD_RAW_REGIONAL_MARKET

#include <atomic>
#include <fstream>
#include <mutex>
#include <string_view>
#include <vector>

#include "debug_mode.h"
#include "error.h"
#include "json.h"
#include "raw_order.h"

/// @brief These are raw orders fetched from the EvE swagger API with minimal
/// pre-processing.  These are typically sorted into a @ref galactic_market_t
/// object later.
class raw_regional_market_t
{
        
    public:
        
        /// @brief Default constructor
        inline raw_regional_market_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.
        
        // Add member access here
        
        uint64_t region_id() const
        {
            return this->region_id_;
        }
        
        void region_id(uint64_t new_region_id)
        {
            this->region_id_ = new_region_id;
        }
        
        const std::vector<raw_order_t>& orders() const
        {
            return this->orders_;
        }
        
        inline const debug_mode_t& debug_mode() const
        {
            return this->debug_mode_;
        }
         
        inline void debug_mode(const debug_mode_t& new_debug_mode)
        {
            this->debug_mode_ = new_debug_mode;
        }
        
        // Add special-purpose member functions here
        
        /// @brief Reset to default-constructed state with zero orders.
        void clear()
        {
            this->orders_.clear();
        }
        
        /// @brief Pull all of the market orders from all NPC stations in a
        /// region in EvE.
        void fetch(uint64_t region_id);
        
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
        
        /// @brief Integral uniquifier used by the EvE Swagger interface for
        /// this region.  For example, 10000002 for "The Forge" or
        /// 10000043 for "Domain".
        uint64_t region_id_;
        
        /// @brief All of the orders pulled from the EvE API for this region.
        std::vector<raw_order_t> orders_;
        
        /// @brief Controls verbose printing and progress messages
        debug_mode_t debug_mode_;
        
        /// @brief Parse data fetched from the EvE API and use it to
        /// re-initialize this object's market orders.
        void decode_from_payloads(const std::vector<Json::Value>& payloads);
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const raw_regional_market_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, raw_regional_market_t& destination);

#endif // Header Guard

