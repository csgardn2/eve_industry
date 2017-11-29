/// @file item_quantities.h
/// @brief Declaration of @ref item_quantities_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 24 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_ITEM_QUANTITIES
#define HEADER_GUARD_ITEM_QUANTITIES

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "item_quantity.h"
#include "json.h"

/// @brief A list of items and a quantity associated with each item.
///
/// Usually used as a bill of materials or a list of output products.
class item_quantities_t
{
        
    public:
        
        /// @brief Default constructor
        inline item_quantities_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.

        // Add member read and write functions
        
        inline const std::vector<item_quantity_t>& materials_list() const
        {
            return this->materials_list_;
        }
        
        inline std::vector<item_quantity_t>& materials_list()
        {
            return this->materials_list_;
        }
        
        // Add special-purpose functions here
        
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
        
    protected:
        
        // Try to make your members protected, even if they don't have to be.
        
        /// @brief See @ref item_quantities_t "class description".
        std::vector<item_quantity_t> materials_list_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const item_quantities_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, item_quantities_t& destination);

#endif // Header Guard
