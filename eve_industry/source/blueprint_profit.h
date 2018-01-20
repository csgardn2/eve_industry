/// @file blueprint_profit.h
/// @brief Declaration of @ref blueprint_profit_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday November 16 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_BLUEPRINT_PROFIT
#define HEADER_GUARD_BLUEPRINT_PROFIT

#include <fstream>
#include <string_view>
#include <unordered_map>

#include "decryptor.h"
#include "error.h"
#include "json.h"
#include "manufacturability.h"

class blueprint_t;
class blueprints_t;
class station_market_t;

/// @brief This is a final profitability report for a specific blueprint.
class blueprint_profit_t
{
        
    public:
        
        /// @brief Used to specify an optimization method or column to sort
        /// by when encoding a list of blueprint profits
        enum class sort_strategy_t
        {
            PROFIT_AMOUNT,
            PROFIT_PERCENT,
            PROFIT_PER_SECOND,
            /// This element must be last
            NUM_ENUMS
        };
        
        /// @brief Default constructor
        inline blueprint_profit_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.
        
        // Add member read and write functions here
        
        const manufacturability_t& manufacturability() const
        {
            return this->manufacturability_;
        }
        
        const decryptor_t& decryptor() const
        {
            return this->decryptor_;
        }
        
        uint64_t blueprint_id() const
        {
            return this->blueprint_id_;
        }
        
        unsigned time() const
        {
            return this->time_;
        }
        
        float total_cost() const
        {
            return this->total_cost_;
        }
        
        float output_value() const
        {
            return this->output_value_;
        }
        
        // Add special-purpose functions here
        
        /// @brief Choose the optimal decryptor 
        void initialize_from_market
        (
            /// [in] The blueprint whose profit you want to calculate
            const blueprint_t& blueprint_of_interest,
            /// [in] Archive of all blueprints.  This is mostly used for
            /// invention, which requires a T1 blueprint and a T2 blueprint.
            const blueprints_t& all_blueprints,
            /// [in] Item price data
            const station_market_t& station_market,
            /// [in] Selects a decryptor to modify time, input materials, and
            /// output if this is a T2 blueprint.
            const decryptor_t& decryptor
        );
        
        inline float profit_amount() const
        {
            return this->output_value_ - this->total_cost_;
        }
        
        inline float profit_percent() const
        {
            return this->output_value_ / this->total_cost_;
        }
        
        inline float profit_per_second() const
        {
            return this->profit_amount() / this->time_;
        }
        
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
            /// [in] Allows human readable names to be displayed with the
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
            /// [in] Allows human readable names to be displayed with the
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
        
        /// @brief See @ref manufacturability_t.
        manufacturability_t manufacturability_;
        
        /// The decryptor used when calculating input cost and output value.
        decryptor_t decryptor_;
        
        /// @brief This profitability report was generated for the blueprint
        /// with this ID.
        uint64_t blueprint_id_;
        
        /// @brief Total time to produce 1 final output item including copy and
        /// invention time.
        ///
        /// Assumes that all input materials are purchased and not manufactured.
        /// Invention time is weighted using the invention probability.
        ///
        /// Only valid if @ref manufacturability_t::is_ok() == true
        unsigned time_;
        
        /// @brief Sum of all costs including input materials, datacores,
        /// decryptors.
        ///
        /// Invention costs are weighted by the probability of
        /// success.
        ///
        /// Only valid if @ref manufacturability_t::is_ok() == true
        float total_cost_;
        
        /// @brief Value of the output items produced by one manufacturing run.
        /// 
        /// For example, a mjolnir fury light missil run produces 5000 missiles.
        /// If these missiles sell for 87 ISK each then this value would be 435000.
        ///
        /// Only valid if @ref manufacturability_t::is_ok() == true
        float output_value_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const blueprint_profit_t& source)
{
    source.write_to_json_file(stream, std::unordered_map<uint64_t, std::string_view>());
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, blueprint_profit_t& destination);

#endif // Header Guard
