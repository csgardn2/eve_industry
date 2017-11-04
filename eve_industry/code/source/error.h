/// @file error.h
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Tuesday September 12 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#ifndef HEADER_GUARD_ERROR
#define HEADER_GUARD_ERROR

#include <string>
#include <string_view>
#include <vector>

/// @brief Error codes are defined to facilitate programmatic detection and
/// correction.
enum class error_code_t
{
    
    FILE_SIZE_FAILED,
    FILE_READ_FAILED,
    FILE_WRITE_FAILED,
    JSON_SCHEMA_VIOLATION,
    
    MODE_MISSING,
    MODE_INVALID,
    ITEM_ATTRIBUTES_OUT_MISSING,
    ITEM_ATTRIBUTES_IN_MISSING,
    STATION_ATTRIBUTES_IN_MISSING,
    PRICES_OUT_MISSING,
    
    UNKNOWN_ORDER_TYPE,
    
    EVE_SUCKS,
    
    /// This element must be last
    NUM_ENUMS
    
};

/// @brief This is an object that gets thrown.  It contains an error code
/// to allow the program to reliably check the type of error that occurred
/// as well as a human readable message suitable for printing.
class error_message_t
{
    
    public:
        
        /// @brief Default constructor
        inline error_message_t() = default;
        
        /// @brief Initialization constructor
        inline error_message_t(error_code_t error_code)
          : error_code_(error_code)
        {
            // All work done in initializer list
        }
        
        /// @brief Initialization constructor
        inline error_message_t(error_code_t error_code, std::string_view error_message)
          : error_code_(error_code),
            message_(error_message)
        {
            // All work done in initializer list
        }
        
        /// @brief Since the @ref message_ "message string" shall never be
        /// used for comparison, you can compare errors based only on an error
        /// code.
        ///
        /// @return true if error codes match and false otherwise.
        inline bool operator==(error_code_t error_code) const
        {
            return this->error_code_ == error_code;
        }
        
        /// @brief Since the @ref message_ "message string" shall never be
        /// used for comparison, you can compare errors based only on an error
        /// code.
        ///
        /// @return true if error codes match and false otherwise.
        inline bool operator==(const error_message_t& other) const
        {
            return this->error_code_ == other.error_code_;
        }
        
        /// @brief 1:1 Inverse of equality operator
        inline bool operator!=(error_code_t error_code) const
        {
            return !(*this == error_code);
        }
        
        /// @brief 1:1 Inverse of equality operator
        inline bool operator!=(const error_message_t& other) const
        {
            return !(*this == other);
        }
        
        /// @brief Read access to @ref error_code_ member.
        inline error_code_t error_code() const
        {
            return this->error_code_;
        }
        
        /// @brief Write access to @ref error_code_ member.
        inline void error_code(error_code_t new_error_code)
        {
            this->error_code_ = new_error_code;
        }
        
        /// @brief Read access to @ref message_ member.
        inline const std::string& message() const
        {
            return this->message_;
        }
        
        /// @brief Write access to @ref message_ member.
        inline void message(const std::string_view new_message)
        {
            this->message_ = new_message;
        }
        
        /// @brief Get the literal name of an enum symbol.
        inline static std::string_view enum_to_string(error_code_t error_code)
        {
            return error_names_[unsigned(error_code)];
        }
        
    protected:
        
        /// @brief Printable names for enums.
        static const std::vector<std::string_view> error_names_;
        
        /// @brief Integral tag for this error.
        error_code_t error_code_;
        
        /// @brief Arbitrary human readable message suitable for printing.
        /// There shall never be any guarentees on how this member is formatted.
        /// Use @ref error_code_ to check what type of error occurred.
        std::string message_;
        
};


/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, error_code_t error_code)
{
    stream << error_message_t::enum_to_string(error_code);
    return stream;
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const error_message_t& error_message)
{
    stream << error_message.message();
    return stream;
}

#endif // header guard

