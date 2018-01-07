/// @file galactic_profits.h
/// @brief Declaration of @ref galactic_profits_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday November 16 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_GALACTIC_PROFITS
#define HEADER_GUARD_GALACTIC_PROFITS

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"
#include "station_profits.h"

class blueprints_t;
class galactic_market_t;

/// @brief Reports on all blueprint profitabilities across all stations.
class galactic_profits_t
{
        
    public:
        
        /// @brief Default constructor
        inline galactic_profits_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.

        // Add member read and write functions
        
        const std::vector<station_profits_t>& station_profits() const
        {
            return this->station_profits_;
        }
        
        // Add special-purpose functions here
        
        /// @brief Initialize this object by calculating profit margins for
        /// all blueprints across all stations.
        void caclulate_station_profits
        (
            const blueprints_t& blueprints,
            const galactic_market_t& galactic_market,
            blueprint_profit_t::sort_strategy_t decryptor_optimization_strategy
        );
        
        // Add encoders/decoders here
        
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
        void read_from_json_structure(const Json::Value& json_root);
        
        /// @brief Serialize the content of this file into a file that
        /// conforms to the schema data/json/schema.json.
        void write_to_json_file
        (
            /// [out] Stream to append serialized object content to.
            std::ostream& file,
            /// [in] Specify how the output should be sorted.  Better values
            /// are written first, near the head of the file.
            blueprint_profit_t::sort_strategy_t output_order,
            /// [in] Allows human readable names to be displayed with each
            /// blueprint's profit report
            const std::unordered_map<uint64_t, std::string_view>& blueprint_names,
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
            /// [in] Specify how the output should be sorted.  Better values
            /// are written first, near buffer[0].
            blueprint_profit_t::sort_strategy_t output_order,
            /// [in] Allows human readable names to be displayed with each
            /// blueprint's profit report
            const std::unordered_map<uint64_t, std::string_view>& blueprint_names,
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
        
    protected:
        
        // Try to make your members protected, even if they don't have to be.
        
        /// @brief Blueprint profitability for each station.
        std::vector<station_profits_t> station_profits_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const galactic_profits_t& source)
{
    source.write_to_json_file
    (
        stream,
        blueprint_profit_t::sort_strategy_t::PROFIT_PER_SECOND,
        std::unordered_map<uint64_t, std::string_view>()
    );
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, galactic_profits_t& destination);

#endif // Header Guard

