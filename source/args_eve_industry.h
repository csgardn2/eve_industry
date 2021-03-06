/// @file args_eve_industry.h
/// @brief Declaration of @ref args::eve_industry_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ARGS_EVE_INDUSTRY
#define HEADER_GUARD_ARGS_EVE_INDUSTRY

#include <fstream>
#include <string_view>
#include <vector>

#include "blueprint_profit.h"
#include "debug_mode.h"
#include "error.h"
#include "json.h"

namespace args
{

    /// @brief Command line argument parsing and validation for eve_industry
    /// program.
    class eve_industry_t
    {
        
        public:
            
            /// @brief Specifies the high level operation to be performed by
            /// this tool.
            enum class mode_t
            {
                /// Pull the integral IDs of all the items in EvE.  This normally
                /// only has to be done once.
                /// * Inputs: EvE API
                /// * Outputs: --item-attributes-out
                FETCH_ITEM_ATTRIBUTES,
                /// Pull market prices for all requested items for all requested
                /// stations
                /// * Inputs: EvE API, --item-attributes-in, --station-attributes-in
                /// * Outputs: --prices-out
                FETCH_PRICES,
                /// Read market data from a previous FETCH_PRICES operation and
                /// calculate the ratios on (output material value) / (input material value)
                /// for each blueprint.
                /// * Inputs: --prices-in, --blueprints-in
                /// * Outputs: --profits-out
                CALCULATE_BLUEPRINT_PROFITS,
                /// This must be last.
                NUM_ENUMS
            };
            
            /// @brief Make modes human readable.
            inline static std::string_view enum_to_string(mode_t mode)
            {
                return eve_industry_t::mode_names_[unsigned(mode)];
            }
            
            /// @brief Default constructor
            inline eve_industry_t() = default;
            
            inline mode_t mode() const
            {
                return this->mode_;
            }
            
            inline const debug_mode_t& debug_mode() const
            {
                return this->debug_mode_;
            }
            
            inline const std::string& item_attributes_out() const
            {
                return this->item_attributes_out_;
            }
            
            inline const std::string& item_attributes_in() const
            {
                return this->item_attributes_in_;
            }
            
            inline const std::string& station_attributes_in() const
            {
                return this->station_attributes_in_;
            }
            
            inline const std::string& prices_out() const
            {
                return this->prices_out_;
            }
            
            inline const std::string& prices_in() const
            {
                return this->prices_in_;
            }
            
            inline const std::string& blueprints_in() const
            {
                return this->blueprints_in_;
            }
            
            inline const std::string& profits_out() const
            {
                return this->profits_out_;
            }
            
            inline bool cull_stations() const
            {
                return this->cull_stations_;
            }
            
            inline bool cull_orders() const
            {
                return this->cull_orders_;
            }
            
            inline blueprint_profit_t::sort_strategy_t output_order() const
            {
                return this->output_order_;
            }
            
            inline blueprint_profit_t::sort_strategy_t decryptor_optimization_strategy()
            {
                return this->decryptor_optimization_strategy_;
            }
            
            // Add more operators here if desired.
            
            /// @brief Extract arguments from the command line and validate them.
            /// Any previous content is destroyed.
            ///
            /// @exception eve_industry_t::error_message_t.
            void parse
            (
                /// The number of strings in argv.  You can pass this from main.
                unsigned argc,
                /// Array of strings generated where each string is a whitespace
                /// delemited item from the command line.  You can pass this from
                /// main.
                char const* const* argv
            );
            
            /// @brief Reset all arguments to their zero/empty values.
            void clear();
            
            /// @brief Open a file conforming to data/json/schema.json and use it to
            /// initialize this object, clearing previous content.
            ///
            /// @exception error_message_t
            /// @exception Json::Exception
            void read_from_json_file(std::istream& file);
            
            /// @brief Decode serialized content conforming to data/json/schema.json
            /// and use it to initialize this object, clearing previous content.
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
            
            /// @brief Return a printable string of all values in mode_values.
            std::string valid_mode_values() const;
            
            /// @brief String representations of enumeration symbols
            static const std::vector<std::string_view> mode_names_;
            
            /// @brief These are valid values that can be passed to --mode on the
            /// command line.
            static const std::vector<std::string_view> mode_values_;
            
            /// @brief The high-level operation to be performed by this tool.
            mode_t mode_;
            
            /// @brief see @ref debug_mode_t.
            debug_mode_t debug_mode_;
            
            /// @brief Filename of a JSON containing the integral IDs used by the
            /// EvE API for items.
            std::string item_attributes_out_;
            
            /// @brief See item_attributes_out_.
            std::string item_attributes_in_;
            
            /// @brief Filename of a JSON containing static information about
            /// stations with markets such as the station's ID, name, solar system,
            /// and region.
            std::string station_attributes_in_;
            
            /// @brief Filename of a JSON containing live market prices for each
            /// item specified by @ref item_attributes_in at all stations specified
            /// by @ref station_attributes_in_.
            std::string prices_out_;
            
            /// @brief See @ref prices_out_.
            std::string prices_in_;
            
            /// @brief Filename of a JSON file containing info about blueprints such
            /// as input materials and invention probabilities.
            std::string blueprints_in_;
            
            /// @brief Filename of a JSON file to report how profitable each blueprint is.
            std::string profits_out_;
            
            /// @brief If true, strips out market orders from stations that weren't
            /// requested by @ref station_attributes_in.
            bool cull_stations_;
            
            /// @brief If true, strips out all market orders that weren't the lowest
            /// sell order.
            bool cull_orders_;
            
            /// @brief See @ref blueprint_profit_t::sort_strategy_t
            blueprint_profit_t::sort_strategy_t output_order_;
            
            /// @brief See @ref blueprint_profit_t::sort_strategy_t
            blueprint_profit_t::sort_strategy_t decryptor_optimization_strategy_;
            
    };
    
} // Namespace args

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const args::eve_industry_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, args::eve_industry_t::mode_t mode)
{
    stream << args::eve_industry_t::enum_to_string(mode);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, args::eve_industry_t& destination);

#endif // Header Guard
