/// @file station_attributes.h
/// @brief Declaration of @ref station_attributes_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday October 31 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_STATION_ATTRIBUTES
#define HEADER_GUARD_STATION_ATTRIBUTES

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"
#include "station_attribute.h"

class station_attributes_t : public std::vector<station_attribute_t>
{
        
    public:
        
        /// @brief Default constructor
        inline station_attributes_t() = default;
        
        // Add operators here if desired.
        // inline type operator[](unsigned ix) const;
        // inline type& operator[](unsigned ix);
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        inline const std::vector<station_attribute_t>& stations() const
        {
            return this->stations_;
        }
        
        /// @brief Open a file conforming to data/json/schema.json and use it to initialize
        /// this object, clearing previous content.
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        void read_from_file(std::istream& file);
        
        /// @brief Decode serialized content conforming to data/json/schema.json and use it
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
        
    protected:
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
        std::vector<station_attribute_t> stations_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const station_attributes_t& source)
{
    source.write_to_file(stream);
    return stream;
}


/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, station_attributes_t& destination);

#endif // Header Guard

