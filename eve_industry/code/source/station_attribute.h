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
        
        /// @brief String representations of enumeration symbols.
        static const std::vector<std::string_view> error_names_;
        
        /// @brief Default error messages suitable for printing.  Note that
        /// some of these are inherently vague without additional information.
        static const std::vector<std::string_view> default_error_messages_;
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
        /// @brief The integral ID of this station, as used by the EvE swagger
        /// interface.
        unsigned id_;
        
        /// @brief The full name of a station such as
        /// "Jita IV - Moon 4 - Caldari Navy Assembly Plant".
        std::string name_;
        
        /// @brief Integral ID of the solar system that this station is in.
        unsigned system_;
        
        /// @brief Integral ID of the region that this station is in.
        unsigned region_;
        
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
        inline station_attribute_t() = default;
        
        /// @brief Copy constructor
        inline station_attribute_t(const station_attribute_t& source) = default;
        
        /// @brief Move constructor
        inline station_attribute_t(station_attribute_t&& source) = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        inline station_attribute_t(unsigned id, std::string_view name, unsigned system, unsigned region)
          : id_(id),
            name_(name),
            system_(system),
            region_(region)
        {
            // All work done in initializer list
        }
        
        /// See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline station_attribute_t(std::istream& file)
        {
            this->decode(file);
        }
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline station_attribute_t(std::string_view buffer)
        {
            this->decode(buffer);
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline station_attribute_t(const Json::Value& json_root)
        {
            this->decode(json_root);
        }
        
        /// @brief Destructor
        inline ~station_attribute_t() = default;
        
        /// @brief Assignment operator with deep copy.
        inline station_attribute_t& operator=(const station_attribute_t& source) = default;
        
        /// @brief Assignment operator with shallow copy.
        inline station_attribute_t& operator=(station_attribute_t&& source) = default;
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline station_attribute_t& operator=(std::istream& file)
        {
            this->decode(file);
            return *this;
        }
        
        /// @brief See @ref decode(std::string_view buffer)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline station_attribute_t& operator=(std::string_view buffer)
        {
            this->decode(buffer);
            return *this;
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline station_attribute_t& operator=(const Json::Value& json_root)
        {
            this->decode(json_root);
            return *this;
        }
        
        // Add operators here if desired.
        // inline type operator[](unsigned ix) const;
        // inline type& operator[](unsigned ix);
        
        /// @brief Returns true if both item's @ref id_ members are equal.
        /// Doesn't bother checking other stats for extra speed.
        inline bool operator==(const station_attribute_t& source) const
        {
            return this->id_ == source.id_;
        }
        
        /// @brief 1:1 inverse of @ref operator==(const station_attribute_t& source) const "operator==".
        inline bool operator!=(const station_attribute_t& source) const
        {
            return !(*this == source);
        }
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        inline unsigned id() const
        {
            return this->id_;
        }
        
        inline void id(unsigned new_id)
        {
            this->id_ = new_id;
        }
        
        inline const std::string& name() const
        {
            return this->name_;
        }
        
        inline void name(std::string_view new_name)
        {
            this->name_ = new_name;
        }
        
        inline unsigned system() const
        {
            return this->system_;
        }
        
        inline void system(unsigned new_system)
        {
            this->system_ = new_system;
        }
        
        inline unsigned region() const
        {
            return this->region_;
        }
        
        inline void region(unsigned new_region)
        {
            this->region_ = new_region;
        }
        
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
inline std::ostream& operator<<(std::ostream& stream, station_attribute_t::error_code_t error_code)
{
    stream << station_attribute_t::error_message_t::enum_to_string(error_code);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const station_attribute_t::error_message_t& error_message)
{
    stream << error_message.message();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const station_attribute_t& source)
{
    stream << source.encode();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::string& operator<<(std::string& buffer, const station_attribute_t& source)
{
    buffer += source.encode();
    return buffer;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, station_attribute_t& destination);

/// @brief Extraction operator for decoding.
inline std::string_view operator>>(std::string_view& buffer, station_attribute_t& destination)
{
    destination.decode(buffer);
    return std::string_view();
}

/// @brief Extraction operator for decoding.
inline Json::Value operator>>(const Json::Value& json_root, station_attribute_t& destination)
{
    destination.decode(json_root);
    return Json::Value();
}

#endif // Header Guard

