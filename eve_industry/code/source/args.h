/// @file args.h
/// @brief Declaration of @ref args_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ARGS
#define HEADER_GUARD_ARGS

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"

/// @brief Command line arguments are pre-processed, validated, and stored
/// in this structure for easy access.
class args_t
{
    
    /// Public section 1 of 2.
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
            /// This must be last.
            NUM_ENUMS
        };
        
    protected:
        
        /// @brief String representations of enumeration symbols
        static const std::vector<std::string_view> mode_names_;
        
        /// @brief These are valid values that can be passed to --mode on the
        /// command line.
        static const std::vector<std::string_view> mode_values_;
        
        /// @brief The high-level operation to be performed by this tool.
        mode_t mode_;
        
        /// @brief Filename of a JSON containing the integral IDs used by the
        /// EvE API for items.
        std::string item_attributes_out_;
        
        /// @brief Filename of a JSON containing the integral IDs used by the
        /// EvE API for items.
        std::string item_attributes_in_;
        
        /// @brief Filename of a JSON containing static information about
        /// stations with markets such as the station's ID, name, solar system,
        /// and region.
        std::string station_attributes_in_;
        
        /// @brief Filename of a JSON containing live market prices for each
        /// item specified by @ref item_attributes_in at all stations specified
        /// by @ref station_attributes_in_.
        std::string prices_out_;
        
    // Public section 2 of 2.
    public:
        
        /// @brief Make modes human readable.
        inline static std::string_view enum_to_string(mode_t mode)
        {
            return args_t::mode_names_[unsigned(mode)];
        }
        
        /// @brief Default constructor
        inline args_t() = default;
        
        /// @brief Read access to @ref mode_ member.
        inline mode_t mode() const
        {
            return this->mode_;
        }
        
        /// @brief Read access to @ref item_attributes_out_ member.
        inline const std::string& item_attributes_out() const
        {
            return this->item_attributes_out_;
        }
        
        /// @brief Read access to @ref item_attributes_in_ member.
        inline const std::string& item_attributes_in() const
        {
            return this->item_attributes_in_;
        }
        
        /// @brief Read access to @ref station_attributes_in_ member.
        inline const std::string& station_attributes_in() const
        {
            return this->station_attributes_in_;
        }
        
        /// @brief Read access to @ref prices_out_ member.
        inline const std::string& prices_out() const
        {
            return this->prices_out_;
        }
        
        // Add more operators here if desired.
        
        /// @brief Extract arguments from the command line and validate them.
        /// Any previous content is destroyed.
        ///
        /// @exception args_t::error_message_t.
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
        void read_from_file(std::istream& file);
        
        /// @brief Decode serialized content conforming to data/json/schema.json
        /// and use it to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_buffer(std::string_view buffer);
        
        /// @brief Extract required data fields from a pre-parsed JSON tree
        /// and use them to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        void read_from_json(const Json::Value& json_root);
        
        /// @brief Serialize the content of this file into a file that
        /// conforms to the schema data/json/schema.json.
        void write_to_file
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
        void write_to_buffer
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
        inline std::string write_to_buffer
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
            this->write_to_buffer(buffer, indent_start, spaces_per_tab);
            return buffer;
        }
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const args_t& source)
{
    source.write_to_file(stream);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, args_t::mode_t mode)
{
    stream << args_t::enum_to_string(mode);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, args_t& destination);

#endif // Header Guard
