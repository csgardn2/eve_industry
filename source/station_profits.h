/// @file station_profits.h
/// @brief Declaration of @ref station_profits_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday November 16 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_STATION_PROFITS
#define HEADER_GUARD_STATION_PROFITS

#include <fstream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "blueprint_profit.h"
#include "error.h"
#include "json.h"

class blueprints_t;

/// @brief Profitability report on all blueprints based on the lowest sell
/// orders at each station.
class station_profits_t
{
        
    public:
        
        /// @brief Default constructor
        inline station_profits_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.

        // Add member read and write functions
        
        inline uint64_t station_id() const
        {
            return this->station_id_;
        }
        
        inline const std::vector<blueprint_profit_t>& blueprint_profits() const
        {
            return this->blueprint_profits_;
        }
        
        // Add special-purpose functions here
        
        /// @brief Calculate the input cost and output value for each blueprint
        /// based on the lowest sell orders.
        void calculate_blueprint_profits
        (
            const blueprints_t& blueprints_in,
            const station_market_t& station_market,
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
        
        /// @brief This is the station whose market was used to calculate
        /// each blueprint profit in @ref blueprint_profits_.
        uint64_t station_id_;
        
        /// @brief Set of all the station_profits owned by a player.
        std::vector<blueprint_profit_t> blueprint_profits_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const station_profits_t& source)
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
std::istream& operator>>(std::istream& stream, station_profits_t& destination);

#endif // Header Guard

