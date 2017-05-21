/// \file
/// File Name:                      preprocessing.cpp \n
/// Date created:                   Fri Dec 30 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html or preprocessing.h for documentation.

#include <iostream>
#include <string>
#include <vector>
#include "token.h"

/// The comment stripper contains a small state machine.  Characters are read
/// one at a time and can change the state.  For example, if double quotes
/// are encountered, then the STRING state is entered and comment delimiters
/// are ignored until the end quote is found
enum class comment_state_t
{
    
    /// \brief The current character is normal text that should be passed onto
    /// the output string
    NORMAL,
    
    /// \brief We are in a quoted string constand
    STRING,
    
    /// \brief A \ character was encountered while in the STRING state and the
    /// subsequent character will be passed onto the output
    ESCAPE_CHAR,
    
    /// \brief A \ character was encountered inside a single comment beginning
    /// with a \\.  If the next character is a newline, then the comment will
    /// continue to the next line even without a delimiter.
    BACKSLASH_COMMENT,
    
    /// \brief Single / was encountererd.  If another / or * is encountered then
    /// we have found a SINGLE_COMMENT or MULTI_COMMENT respectivly.
    BEGIN_COMMENT,
    
    /// \brief We are in a MULTI_COMMENT and have found a *.  If the next
    /// character is a / then the comment ends.
    BEGIN_MULTI_CLOSE,
    
    /// \brief We are inside a single line comment which typically ends when
    /// the next newline is encountered.
    SINGLE_COMMENT,
    
    /// \brief We are inside a multi line comment which ends with a */.
    MULTI_COMMENT
    
};

/// \brief The only whitespace that will not be stripped is 
enum class whitespace_state_t
{
    
    /// \brief Normal characters to be passed or stripped
    NORMAL,
    
    /// \brief A \\ is encountered inside a string
    ESCAPE_CHAR,
    
    /// \brief We are inside a quoted string and want to pass whitespace
    STRING
    
};

const char* token_type_to_string(token_type_t token_type)
{
    
    static const unsigned num_tokens = 11;
    static const char* token_names[num_tokens] = 
    {
        "BOOL_FALSE",
        "BOOL_TRUE",
        "CLOSE_CURLY",
        "CLOSE_SQUARE",
        "COLON",
        "COMMA",
        "FLOAT",
        "INT",
        "OPEN_CURLY",
        "OPEN_SQUARE",
        "STRING"
    };
    
    unsigned cast_type = unsigned(token_type);
    if (cast_type < num_tokens)
        return token_names[cast_type];
    else
        return "INVALID";
    
}

std::string json_token_t::print() const
{
    
    std::string ret = "{type = ";
    ret += token_type_to_string(this->token_type);
    ret += ", line = ";
    ret += std::to_string(this->line);
    ret += ", value = \"";
    ret += this->value;
    ret += "\"}";
    
    return ret;
    
}

std::string strip_comments(const std::string& input_string)
{
    
    unsigned input_len = input_string.length();
    
    std::string ret;
    ret.reserve(input_len);
    
    // See strip_comments_state_machine.png for visualization
    comment_state_t state = comment_state_t::NORMAL;
    for (unsigned ix = 0; ix < input_len; ix++)
    {
        
        char c = input_string[ix];
        
        switch (state)
        {
            case comment_state_t::NORMAL:
                switch (c)
                {
                    case '\"':
                        state = comment_state_t::STRING;
                        ret += '\"';
                        continue;
                    case '/':
                        state = comment_state_t::BEGIN_COMMENT;
                        continue;
                    default:
                        ret += c;
                        continue;
                }
            case comment_state_t::STRING:
                switch (c)
                {
                    case '\"':
                        ret += '\"';
                        continue;
                    case '\\':
                        ret += '\\';
                        state = comment_state_t::ESCAPE_CHAR;
                        continue;
                    default:
                        ret += c;
                        continue;
                }
            case comment_state_t::ESCAPE_CHAR:
                ret += c;
                state = comment_state_t::STRING;
                continue;
            case comment_state_t::BACKSLASH_COMMENT:
                // You either discard a character because it's a newline
                // or because it's inside a comment
                state = comment_state_t::SINGLE_COMMENT;
                continue;
            case comment_state_t::BEGIN_COMMENT:
                switch (c)
                {
                    case '/':
                        state = comment_state_t::SINGLE_COMMENT;
                        continue;
                    case '*':
                        state = comment_state_t::MULTI_COMMENT;
                        continue;
                    default:
                        ret += '/';
                        ret += c;
                        state = comment_state_t::NORMAL;
                        continue;
                }
            case comment_state_t::BEGIN_MULTI_CLOSE:
                if (c == '/')
                {
                    state = comment_state_t::NORMAL;
                    continue;
                } else {
                    ret += '*';
                    ret += c;
                    continue;
                }
            case comment_state_t::SINGLE_COMMENT:
                switch (c)
                {
                    case '\n':
                        state = comment_state_t::NORMAL;
                        continue;
                    case '\\':
                        state = comment_state_t::BACKSLASH_COMMENT;
                        continue;
                    default:
                        continue;
                }
            case comment_state_t::MULTI_COMMENT:
                if (c == '*')
                {
                    state = comment_state_t::BEGIN_MULTI_CLOSE;
                    continue;
                }
                continue;
        }
        
    }
    
    // Clean up some delayed characters at the end of a string
    switch (state)
    {
        case comment_state_t::BEGIN_COMMENT:
            ret += '/';
            return ret;
        case comment_state_t::ESCAPE_CHAR:
            ret += '\\';
            return ret;
        default:
            return ret;
    }
    
}

std::string strip_whitespace(const std::string& input_string)
{
    
    unsigned input_len = input_string.length();
    
    std::string ret;
    ret.reserve(input_len);
    
    whitespace_state_t state = whitespace_state_t::NORMAL;
    for (unsigned ix = 0; ix < input_len; ix++)
    {
        
        char c = input_string[ix];
        
        switch (state)
        {
            case whitespace_state_t::NORMAL:
                if (c == '\"')
                {
                    state = whitespace_state_t::STRING;
                    ret += c;
                    continue;
                } else if (isspace(c) == 0) {
                    ret += c;
                    continue;
                }
                continue;
            case whitespace_state_t::ESCAPE_CHAR:
                ret += c;
                state = whitespace_state_t::STRING;
                continue;
            case whitespace_state_t::STRING:
                if (c == '\"')
                {
                    ret += '\"';
                    state = whitespace_state_t::NORMAL;
                    continue;
                } else if (c == '\\') {
                    ret += '\\';
                    state = whitespace_state_t::ESCAPE_CHAR;
                    continue;
                } else {
                    ret += c;
                    continue;
                }
        }
        
    }
    
    return ret;
    
}

std::vector<json_token_t> tokenize(const std::string input_string)
{
    
    std::vector<json_token_t> ret;
    
    unsigned input_len = input_string.length();
    unsigned line = 1;
    for (unsigned ix = 0; ix < input_len; ix++)
    {
        
        char c = input_string[ix];
        
        switch (c)
        {
            case 't':
                if
                (
                    ix + 3 < input_len
                 && input_string[ix + 1] == 'r'
                 && input_string[ix + 2] == 'u'
                 && input_string[ix + 3] == 'e'
                ){
                    ret.emplace_back(token_type_t::BOOL_TRUE, line);
                    ix += 3; // discard characters
                }
                continue;
            case 'f':
                if
                (
                    ix + 4 < input_len
                 && input_string[ix + 1] == 'a'
                 && input_string[ix + 2] == 'l'
                 && input_string[ix + 3] == 's'
                 && input_string[ix + 4] == 'e'
                ){
                    ret.emplace_back(token_type_t::BOOL_FALSE, line);
                    ix += 4;
                }
                continue;
            case '\n':
                line++;
                continue;
            case '{':
                ret.emplace_back(token_type_t::OPEN_CURLY, line);
                continue;
            case '}':
                ret.emplace_back(token_type_t::CLOSE_CURLY, line);
                continue;
            case '[':
                ret.emplace_back(token_type_t::OPEN_SQUARE, line);
                continue;
            case ']':
                ret.emplace_back(token_type_t::CLOSE_SQUARE, line);
                continue;
            case ':':
                ret.emplace_back(token_type_t::COLON, line);
                continue;
            case ',':
                ret.emplace_back(token_type_t::COMMA, line);
                continue;
            case '\"':
            {
                
                if (ix + 1 == input_len)
                {
                    
                    std::cerr
                        << "Error tokenizing line "
                        << line
                        << ".  File ends in opening quote\n";
                    return ret;
                    
                } else {
                    
                    // Discard opening quote and start copying in string
                    unsigned str_const_start = ix + 1;
                    while (true)
                    {
                        
                        ix++;
                        // First increment skips over opening quote
                        if (ix >= input_len)
                        {
                            std::cerr
                                << "Error tokenizing line "
                                << line
                                << ".  Unterminated string constant\n";
                            return ret;
                        }
                        
                        c = input_string[ix];
                        
                        // Terminating quote
                        if (c == '\"')
                            break;
                        
                        // Escaped character
                        if (c == '\\')
                            ix++;
                        
                        // Newline inside string constant
                        if (c == '\n')
                            line++;
                        
                    }
                    
                    ret.emplace_back
                    (
                        token_type_t::STRING,
                        line,
                        input_string,
                        str_const_start,
                        ix - str_const_start
                    );
                    
                    continue;
                    
                }
                
            }
            
            case ' ':
            case '\t':
            case '\r':
            case '\v':
                // Ignore whitespace
                continue;
            
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '+':
            case '.':
            {
                
                // Assume integer unless a decimal point is found
                json_token_t numeric_token(token_type_t::INT, line);
                numeric_token.value = c;
                
                bool found_exponent = false;
                bool found_decimal = false;
                while (true)
                {
                    
                    ix++;
                    if (ix >= input_len)
                        break;
                    c = input_string[ix];
                    
                    if (isdigit(c) != 0)
                    {
                        numeric_token.value += c;
                        continue;
                    }
                    
                    if (c == ',' || c == '}' || c == ']' || isspace(c) != 0)
                    {
                        // End of numeric constant and unget character so it
                        // can be included in the token stream
                        ix--;
                        ret.push_back(numeric_token);
                        break;
                    }
                    
                    if (c == '.')
                    {
                        if (found_decimal)
                        {
                            std::cerr
                                << "Error tokenizing line "
                                << line
                                << ".  Multiple decimal points in numeric constant\n";
                            return ret;
                        } else {
                            found_decimal = true;
                            numeric_token.token_type = token_type_t::FLOAT;
                            numeric_token.value += '.';
                        }
                        continue;
                    }
                    
                    if (c == 'e' || c == 'E')
                    {
                        
                        if (found_exponent)
                        {
                            
                            std::cerr
                                << "Multiple exponent occurrences in numeric constant on line "
                                << line
                                << ".\n";
                            return ret;
                            
                        } else {
                            
                            found_exponent = true;
                            numeric_token.value += c;
                            
                            // Accept optional + or - at start of exponent
                            if (ix + 1 < input_len)
                            {
                                ix++;
                                c = input_string[ix];
                                if (isdigit(c) != 0 || c == '-' || c == '+')
                                    numeric_token.value += c;
                                else
                                    std::cerr
                                        << "Error tokenizing line "
                                        << line
                                        << ".  Unrecognized character \'"
                                        << c
                                        << "\' in numeric constant\n";
                                    return ret;
                            }
                            
                        }
                        
                        continue;
                        
                    }
                    
                    std::cerr
                        << "Error tokenizing line "
                        << line
                        << ".  Unrecognized character \'"
                        << c
                        << "\' in numeric constant\n";
                    return ret;
                    
                }
                
                continue;
            
            }
            
            default:
                std::cerr
                    << "Error tokenizing character \'"
                    << c
                    << "\' on line "
                    << line
                    << ".\n";
                return ret;
            
        }
        
    }
    
    return ret;
    
}

