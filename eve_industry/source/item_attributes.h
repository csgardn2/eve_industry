/// @file item_attributes.h
/// @brief Declaration of @ref item_attributes_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday October 20 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ITEM_ATTRIBUTES
#define HEADER_GUARD_ITEM_ATTRIBUTES

#include <fstream>
#include <string_view>
#include <vector>

#include "debug_mode.h"
#include "error.h"
#include "item_attribute.h"
#include "json.h"

class item_ids_t;

/// @brief A group of multiple item attributes.  Useful for bulk fetching,
/// encoding, and decoding possibly using multiple threads.
class item_attributes_t
{
        
    public:
        
        /// @brief Default constructor
        inline item_attributes_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.
        
        // Add member read and write functions
        
        inline const std::vector<item_attribute_t>& items() const
        {
            return this->items_;
        }
        
        inline const debug_mode_t& debug_mode() const
        {
            return this->debug_mode_;
        }
         
        inline void debug_mode(const debug_mode_t& new_debug_mode)
        {
            this->debug_mode_ = new_debug_mode;
        }
        
        /// @brief Fetch the attributes for each item id in the given input
        /// list, replacing previous content.
        ///
        /// @exception item_attribute_t::error_message_t
        /// @exception Json::Exception
        void fetch(const item_ids_t& item_ids);
        
        /// @brief Open a file conforming to schema.json and use it to initialize
        /// this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_json_file(std::istream& file);
        
        /// @brief Decode serialized content conforming to schema.json and use it
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
        /// conforms to the schema schema.json.
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
        /// conforms to the schema schema.json.
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
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
        std::vector<item_attribute_t> items_;
        
        /// @brief Enables printing of diagnostic messages.
        debug_mode_t debug_mode_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const item_attributes_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, item_attributes_t& destination);

#endif // Header Guard
