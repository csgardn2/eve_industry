/// @file item_quantity.h
/// @brief Declaration of @ref item_quantity_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 24 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_ITEM_QUANTITY
#define HEADER_GUARD_ITEM_QUANTITY

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"

/// @brief Specifies an item ID and an amount of that item.  Usually used
/// with @ref blueprint_t to specify a bill of materials.
class item_quantity_t
{
        
    public:
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        item_quantity_t() = default;
        
        item_quantity_t(uint64_t item_id, unsigned quantity)
          : item_id_(item_id),
            quantity_(quantity)
        {
            // All work done in initializer list.
        }
        
        // Add operators here if desired.

        // Add member read and write functions
        
        uint64_t item_id() const
        {
            return this->item_id_;
        }
        
        void item_id(uint64_t new_item_id)
        {
            this->item_id_ = new_item_id;
        }
        
        unsigned quantity() const
        {
            return this->quantity_;
        }
        
        void quantity(unsigned new_quantity)
        {
            this->quantity_ = new_quantity;
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
        
        /// @brief Integral ID of an EvE inventory item as used by the EvE
        /// Swagger interface.
        uint64_t item_id_;
        
        /// @brief An amount of this item.
        unsigned quantity_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const item_quantity_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, item_quantity_t& destination);

#endif // Header Guard

