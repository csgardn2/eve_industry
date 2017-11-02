/// @file order.h
/// @brief Declaration of @ref order_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday October 31 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ORDER
#define HEADER_GUARD_ORDER

#include <fstream>
#include <string_view>
#include <vector>

#include "error.h"
#include "json.h"

/// @brief Indicates if a market bid is a buy or sell order.
enum class order_type_t
{
    /// @brief The person who placed the bid wants to buy this item.
    BUY,
    /// @brief The person who placed this bid wants to sell this item.
    SELL,
    /// @brief This must be last.
    NUM_ENUMS
};

/// @brief A market bid 
class order_t
{
        
    protected:
        
        /// @brief String representations of enumeration symbols.
        static const std::vector<std::string_view> error_names_;
        
        /// @brief Default error messages suitable for printing.  Note that
        /// some of these are inherently vague without additional information.
        static const std::vector<std::string_view> default_error_messages_;
        
        // Try to make all your members protected, even if they don't
        // really need to be.
        
        /// @brief The integral uniquifier of the inventory item associated
        /// with these bid.
        unsigned item_id_;
        
        /// @brief The asking price for this bid.
        float price_;
        
        /// @brief The integral uniquifier of the NPC station of this bid.
        unsigned station_id_;
        
        /// @brief Indicates if this order is a buy or sell order.
        order_type_t order_type_;
        
    public:
        
        /// @brief Error codes are defined to facilitate programmatic detection and
        /// correction.
        enum class error_code_t
        {
            FILE_SIZE_FAILED,
            FILE_READ_FAILED,
            FILE_WRITE_FAILED,
            JSON_SCHEMA_VIOLATION,
            UNKNOWN_ORDER_TYPE,
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
        inline order_t() = default;
        
        /// @brief Copy constructor
        inline order_t(const order_t& source) = default;
        
        /// @brief Move constructor
        inline order_t(order_t&& source) = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        inline order_t(unsigned item_id, float price, unsigned station_id, order_type_t order_type)
          : item_id_(item_id),
            price_(price),
            station_id_(station_id),
            order_type_(order_type)
        {
            // All work done in initializer list
        }
        
        /// See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline order_t(std::istream& file)
        {
            this->decode(file);
        }
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline order_t(std::string_view buffer)
        {
            this->decode(buffer);
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline order_t(const Json::Value& json_root)
        {
            this->decode(json_root);
        }
        
        /// @brief Destructor
        inline ~order_t() = default;
        
        /// @brief Assignment operator with deep copy.
        inline order_t& operator=(const order_t& source) = default;
        
        /// @brief Assignment operator with shallow copy.
        inline order_t& operator=(order_t&& source) = default;
        
        /// @brief See @ref decode(std::istream& file)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline order_t& operator=(std::istream& file)
        {
            this->decode(file);
            return *this;
        }
        
        /// @brief See @ref decode(std::string_view buffer)
        ///
        /// @exception error_message_t
        /// @exception Json::Exception
        inline order_t& operator=(std::string_view buffer)
        {
            this->decode(buffer);
            return *this;
        }
        
        /// @brief See @ref decode(const Json::Value& json_root)
        ///
        /// @exception error_message_t
        inline order_t& operator=(const Json::Value& json_root)
        {
            this->decode(json_root);
            return *this;
        }
        
        // Add operators here if desired.
        // inline type operator[](unsigned ix) const;
        // inline type& operator[](unsigned ix);
        
        /// @brief Returns true if all members are equal
        inline bool operator==(const order_t& source) const
        {
            return
                this->item_id_ == source.item_id_
             && this->price_ == source.price_
             && this->station_id_ == source.station_id_
             && this->order_type_ == source.order_type_;
        }
        
        /// @brief 1:1 inverse of @ref operator==(const order_t& source) const "operator==".
        inline bool operator!=(const order_t& source) const
        {
            return !(*this == source);
        }
        
        // Add member read and write functions
        // inline type member() const;
        // inline void member(const type& t);
        
        inline unsigned item_id() const
        {
            return this->item_id_;
        }
        
        inline void item_id(unsigned new_item_id)
        {
            this->item_id_ = new_item_id;
        }
        
        inline float price() const
        {
            return this->price_;
        }
        
        inline void price(float new_price)
        {
            this->price_ = new_price;
        }
        
        inline unsigned station_id() const
        {
            return this->station_id_;
        }
        
        inline void station_id(unsigned new_station_id)
        {
            this->station_id_ = new_station_id;
        }
        
        inline order_type_t order_type() const
        {
            return this->order_type_;
        }
        
        inline void order_type(order_type_t new_order_type)
        {
            this->order_type_ = new_order_type;
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
inline std::ostream& operator<<(std::ostream& stream, order_t::error_code_t error_code)
{
    stream << order_t::error_message_t::enum_to_string(error_code);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const order_t::error_message_t& error_message)
{
    stream << error_message.message();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const order_t& source)
{
    stream << source.encode();
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::string& operator<<(std::string& buffer, const order_t& source)
{
    buffer += source.encode();
    return buffer;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, order_t& destination);

/// @brief Extraction operator for decoding.
inline std::string_view operator>>(std::string_view& buffer, order_t& destination)
{
    destination.decode(buffer);
    return std::string_view();
}

/// @brief Extraction operator for decoding.
inline Json::Value operator>>(const Json::Value& json_root, order_t& destination)
{
    destination.decode(json_root);
    return Json::Value();
}

#endif // Header Guard

