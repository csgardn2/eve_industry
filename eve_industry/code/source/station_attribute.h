/// @file station_attribute.h
/// @brief Declaration of @ref station_attribute_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday October 31 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_STATION_ATTRIBUTE
#define HEADER_GUARD_STATION_ATTRIBUTE

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"

/// @brief Static information about an NPC station in EvE that doesn't change
/// often such as its location.
class station_attribute_t
{
        
    protected:
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
        /// @brief The full name of a station such as
        /// "Jita IV - Moon 4 - Caldari Navy Assembly Plant".
        std::string name_;
        
        /// @brief The integral ID of this station, as used by the EvE swagger
        /// interface.
        uint64_t station_id_;
        
        /// @brief Integral ID of the solar system that this station is in.
        uint64_t system_id_;
        
        /// @brief Integral ID of the region that this station is in.
        uint64_t region_id_;
        
    public:
        
        inline station_attribute_t() = default;
        
        // Add member initialization constructors here
        
        // Try to use initializer lists when possible.
        inline station_attribute_t(std::string_view name, uint64_t station_id, uint64_t system_id, uint64_t region_id)
          : name_(name),
            station_id_(station_id),
            system_id_(system_id),
            region_id_(region_id)
        {
            // All work done in initializer list
        }
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        inline const std::string& name() const
        {
            return this->name_;
        }
        
        inline void name(std::string_view new_name)
        {
            this->name_ = new_name;
        }
        
        inline uint64_t station_id() const
        {
            return this->station_id_;
        }
        
        inline void station_id(uint64_t new_station_id)
        {
            this->station_id_ = new_station_id;
        }
        
        inline uint64_t system_id() const
        {
            return this->system_id_;
        }
        
        inline void system_id(uint64_t new_system_id)
        {
            this->system_id_ = new_system_id;
        }
        
        inline uint64_t region_id() const
        {
            return this->region_id_;
        }
        
        inline void region(uint64_t new_region_id)
        {
            this->region_id_ = new_region_id;
        }
        
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
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const station_attribute_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, station_attribute_t& destination);

#endif // Header Guard

