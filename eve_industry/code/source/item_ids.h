/// @file item_ids.h
/// @brief Declaration of @ref item_ids_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Sunday October 8 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ITEM_IDS
#define HEADER_GUARD_ITEM_IDS

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"

/// @brief List of integral uniquifiers used by inventory items in the EvE
/// universe.  These are only the IDs, not the attributes for any items.
class item_ids_t : public std::vector<uint64_t>
{
        
    protected:
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
    public:
        
        /// @brief Default constructor
        inline item_ids_t() = default;
        
        // Add operator[] here if desired.
        // inline type operator[](unsigned ix) const;
        // inline type& operator[](unsigned ix);
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        /// @brief Pull as many item IDs from the EvE API as possible.  All
        /// previous content is cleared.
        void fetch();
        
        /// @brief Open a file conforming to schema.json and use it to initialize
        /// this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_file(std::istream& file);
        
        /// @brief Decode serialized content conforming to schema.json and use it
        /// to initialize this object, clearing previous content.
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
        /// conforms to the schema schema.json.
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
        /// conforms to the schema schema.json.
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
inline std::ostream& operator<<(std::ostream& stream, const item_ids_t& source)
{
    source.write_to_file(stream);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::string& operator<<(std::string& buffer, const item_ids_t& source)
{
    source.write_to_buffer(buffer);
    return buffer;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, item_ids_t& destination);

/// @brief Extraction operator for decoding.
inline std::string_view operator>>(std::string_view& buffer, item_ids_t& destination)
{
    destination.read_from_buffer(buffer);
    return std::string_view();
}

/// @brief Extraction operator for decoding.
inline Json::Value operator>>(const Json::Value& json_root, item_ids_t& destination)
{
    destination.read_from_json(json_root);
    return Json::Value();
}

#endif // Header Guard

