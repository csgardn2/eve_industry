/// @file item_ids.h
/// @brief Declaration of @ref item_ids.h class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Saturday September 23 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_ITEM_IDS
#define HEADER_GUARD_ITEM_IDS

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "item_id.h"
#include "json.h"

class item_ids_t : public std::vector<item_id_t>
{
        
    protected:
        
        /// @brief String representations of enumeration symbols.
        static const std::vector<std::string_view> error_names_;
        
        /// @brief Default error messages suitable for printing.  Note that
        /// some of these are inherently vague without additional information.
        static const std::vector<std::string_view> default_error_messages_;
        
    public:
        
        /// @brief Error codes are defined to facilitate programmatic detection and
        /// correction.
        enum class error_code_t
        {
            FILE_SIZE_FAILED,
            FILE_READ_FAILED,
            FILE_WRITE_FAILED,
            JSON_SCHEMA_VIOLATION,
            /// This element must be last
            NUM_ENUMS
        };
        
        typedef error_message_template_t
        <
            error_code_t,
            error_names_,
            default_error_messages_
        > error_message_t;
        
        /// @brief Default constructor
        inline item_ids_t() = default;
        
        /// @brief Copy constructor
        inline item_ids_t(const item_ids_t& source) = default;
        
        /// @brief Move constructor
        inline item_ids_t(item_ids_t&& source) = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        /// See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline item_ids_t(std::istream& file)
        {
            this->decode(file);
        }
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline item_ids_t(std::string_view buffer)
        {
            this->decode(buffer);
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline item_ids_t(const Json::Value& json_root)
        {
            this->decode(json_root);
        }
        
        /// @brief Destructor
        inline ~item_ids_t() = default;
        
        /// @brief Assignment operator with deep copy.
        inline item_ids_t& operator=(const item_ids_t& source) = default;
        
        /// @brief Assignment operator with shallow copy.
        inline item_ids_t& operator=(item_ids_t&& source) = default;
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline item_ids_t& operator=(std::istream& file)
        {
            this->decode(file);
            return *this;
        }
        
        /// @brief See @ref decode(std::string_view buffer)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline item_ids_t& operator=(std::string_view buffer)
        {
            this->decode(buffer);
            return *this;
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline item_ids_t& operator=(const Json::Value& json_root)
        {
            this->decode(json_root);
            return *this;
        }
        
        // Add operator[] here if desired.
        // inline type operator[](unsigned ix) const;
        // inline type& operator[](unsigned ix);
        
        /// @brief Returns true of all elements in this vector are equal and in
        /// the same order.
        inline bool operator==(const item_ids_t& source) const
        {
            
            // Check dimensions
            unsigned num_item_ids = this->size();
            if (num_item_ids != source.size())
                return false;
            
            // Check each element
            for (unsigned ix = 0; ix < num_item_ids; ix++)
            {
                if ((*this)[ix] != source[ix])
                    return false;
            }
            
            // All tests passed
            return true;
            
        }
        
        /// @brief 1:1 inverse of @ref operator==(const item_ids_t& source) const "operator==".
        inline bool operator!=(const item_ids_t& source) const
        {
            return !(*this == source);
        }
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        /// @brief Pull all item IDS from the EvE API and then pull the names
        /// corresponding to each item ID.
        void fetch();
        
        /// @brief Open a file conforming to data/json/schema.json and use it to initialize
        /// this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void decode(std::istream& file);
        
        /// @brief Decode serialized content conforming to data/json/schema.json and use it
        /// to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void decode(std::string_view buffer);
        
        /// @brief Extract required data fields from a pre-parsed JSON tree
        /// and use them to initialize this object, clearing previous content.
        ///
        /// @exception error_message_t
        void decode(const Json::Value& json_root);
        
        /// @brief Serialize the content of this file into a file that
        /// conforms to the schema data/json/schema.json.
        void encode
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
        void encode
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
        inline std::string encode
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
            this->encode(buffer, indent_start, spaces_per_tab);
            return buffer;
        }
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, item_ids_t::error_code_t error_code)
{
    stream << item_ids_t::error_message_t::enum_to_string(error_code);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const item_ids_t::error_message_t& error_message)
{
    stream << error_message.message();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const item_ids_t& source)
{
    stream << source.encode();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::string& operator<<(std::string& buffer, const item_ids_t& source)
{
    buffer += source.encode();
    return buffer;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, item_ids_t& destination);

/// @brief Extraction operator for decoding.
inline std::string_view operator>>(std::string_view& buffer, item_ids_t& destination)
{
    destination.decode(buffer);
    return std::string_view();
}

/// @brief Extraction operator for decoding.
inline Json::Value operator>>(const Json::Value& json_root, item_ids_t& destination)
{
    destination.decode(json_root);
    return Json::Value();
}

#endif // Header Guard

