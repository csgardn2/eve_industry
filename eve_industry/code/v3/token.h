/// \file
/// File Name:                      preprocessing.h \n
/// Date created:                   Fri Dec 30 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_PREPROCESSING
#define HEADER_GUARD_PREPROCESSING

#include <string>
#include <vector>

/// \brief Identifies a single token or constant type from a JSON file
enum class token_type_t
{
    BOOL_FALSE,
    BOOL_TRUE,
    CLOSE_CURLY,
    CLOSE_SQUARE,
    COLON,
    COMMA,
    FLOAT,
    INT,
    OPEN_CURLY,
    OPEN_SQUARE,
    STRING
};

/// \brief Produce a human readable string for an enumerated token type.
const char* token_type_to_string(token_type_t token_type);

/// \brief A small, atomic, piece of the JSON stream such as a single delimiter,
/// string constant, or numeric constant.  
class json_token_t
{
    
    public:
        
        /// \brief Default constructor
        json_token_t() = default;
        
        /// \brief Initialization constructor.  \ref json_token_t::value "value"
        /// is initialized to an empty string
        json_token_t
        (
            token_type_t token_type_,
            unsigned line_
        )
            : token_type(token_type_), line(line_), value()
        {
            // Intentionally left blank
        }
        
        /// \brief Initialization constructor.  \ref json_token_t::value "value"
        /// is initialized from the str parameter
        json_token_t
        (
            token_type_t token_type_,
            unsigned line_,
            const char* str
        )
            : token_type(token_type_), line(line_), value(str)
        {
            // Intentionally left blank
        }
        
        /// \brief Initialization constructor.  \ref json_token_t::value "value"
        /// is initialized from a substring of the str parameter
        json_token_t
        (
            token_type_t token_type_,
            unsigned line_,
            const std::string& str,
            unsigned start,
            unsigned length
        )
            : token_type(token_type_), line(line_), value(str, start, length)
        {
            // Intentionally left blank
        }
        
        /// \brief Returns true when all elements (including \ref
        /// json_token_t::line "line") are the same and false otherwise.
        inline bool operator==(const json_token_t& other) const
        {
            return
                this->token_type == other.token_type
             && this->line == other.line
             && this->value == other.value;
        }
        
        /// \brief Returns true when the token type of two tokens are the same.
        /// The function ignores the \ref line and \ref value members.  Returns
        /// false otherwise.
        inline bool operator==(token_type_t other) const
        {
            return this->token_type == other;
        }
        
        /// \brief Inverse of \ref
        /// json_token_t::operator==(const token_type_t) const "operator=="
        inline bool operator!=(token_type_t other) const
        {
            return this->token_type != other;
        }
        
        /// \brief Inverse of \ref
        /// json_token_t::operator==(const json_token_t&) const "operator=="
        inline bool operator!=(const json_token_t& other) const
        {
            return !(other == *this);
        }
        
        /// \brief Generate a string representing the contents of the structure
        /// for debugging and diagnostic purposes.
        std::string print() const;
        
        /// \brief See \ref token_type_t
        token_type_t token_type;
        
        /// \brief The line number of a token.  This is mostly used to help
        /// print error messages.
        unsigned line;
        
        /// \brief If an integer, float, or other constant is encountered, it
        /// is copied here.  Integers, floats, or bools are not
        /// converted to their native type (they are just copied as strings into
        /// \ref json_token_t::value "value").  Delimiting quotation marks are
        /// not included included in string constants and escaped characters are
        /// passed without being processed (so the string "\t" will be stored as a
        /// \ followed by a t and not the tab character).
        std::string value;
        
};

/// \brief Produce a human readable string for an enumerated token type.
inline const char* token_type_to_string(const json_token_t& token)
{
    return token_type_to_string(token.token_type);
}

/// \brief Remove all characters that are javascript (JSON) comments.
/// Comments in javascript are exactly the same as C++ comments
/// Technically, JSON files aren't required to support comments but I'm going
/// to support them anyway.
/// \image html strip_comments_state_machine.png
std::string strip_comments(const std::string& input_string);

/// \brief Remove all whitespace that isn't inside a quoted string.  You should
/// \brief run \ref strip_comments first, as this function will not handle
/// comments
/// \image html strip_whitespace_state_machine.png
std::string strip_whitespace(const std::string& input_string);

/// \brief Convert a pre-processed string into a sequence of tokens which will
/// be easy to process.  You should preprocess the input string with \ref
/// strip_comments and \ref strip_whitespace first.
std::vector<json_token_t> tokenize(const std::string input_string);

#endif // header guard

