/// \file
/// File Name:                      json.h \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_JSON
#define HEADER_GUARD_JSON

//#include <iostream>
#include <string>
#include <vector>

#include "token.h"

class json_array_t;
class json_struct_t;

/// \brief Used to self-identify \ref json_omni_t types.
enum class json_type_t
{
    BOOLEAN,
    INT,
    FLOAT,
    STRING,
    ARRAY,
    STRUCT
};

/// \brief Return a human readable string corresponding to a JSON type
const char* json_type_to_string(json_type_t json_type);

/// \brief This is used to create pointers to any type of datatype that could
/// be stored in a JSON structure.
class json_omni_t
{
    
    public:
        
        virtual ~json_omni_t() = default;
        
        /// \brief Call this to identify what type of data is actually stored
        /// at a particular JSON node.
        virtual json_type_t get_type() const = 0;
        
        /// \brief Returns false if the result of \ref get_type() is either
        /// \ref json_type_t "STRUCT" or \ref json_type_t "ARRAY" and returns
        /// true otherwise.
        virtual inline bool is_leaf() const = 0; 
        
};

/// \brief Leaf nodes that do not need any special memory allocation such as
/// \ref json_type_t "BOOLEAN", \ref json_type_t "INT", json_type_t "FLOAT", and
/// \ref json_type_t "STRING".
class json_leaf_t : public json_omni_t
{
    
    public:
        
        virtual ~json_leaf_t() = default;
        
        inline bool is_leaf() const
        {
            return true;
        }
        
};

/// \brief These are nodes that contain children, namely \ref json_array_t and
/// \ref json_struct_t that must have custom memory allocation functions
class json_interior_t : public json_omni_t
{
    
    protected:
        
        virtual ~json_interior_t() = default;
        
        /// \brief Self identify as an interior node
        inline bool is_leaf() const
        {
            return false;
        }
        
};

/// \brief This is the top level node of a JSON tree that you will typically
/// construct and destroy.  When it is destroyed, it recursivly de-allocates all 
/// its children before removing itself.
class json_root_t
{
    
    public:
        
        /// \brief Initialize an empty JSON node with no children.
        inline json_root_t()
        {
            this->start_node = NULL;
        }
        
        /// \brief Construct a JSON tree from another tree, recursivly making
        /// separate and independent copies of all child nodes.
        inline json_root_t(const json_root_t& other)
        {
            this->copy(other);
        }
        
        /// \brief Parse the raw text of a JSON file and use it to initialize
        /// the internal tree structure.
        inline json_root_t(const std::string& raw_json_data)
        {
            this->start_node = NULL;
            this->parse(raw_json_data);
        }
        
        /// \brief Recursivly destroy all children and then destroy the root
        /// itself
        inline ~json_root_t()
        {
            this->clear();
        }
        
        /// \brief Recursivly destroy current contents (if any) and make an
        /// independent copy of the source.
        inline json_root_t& operator=(const json_root_t& other)
        {
            
            // Check for self-assignment
            if (this == &other)
                return *this;
            
            this->clear();
            this->copy(other);
            return *this;
        }
        
        /// \brief Create a human readable, indented, display of the JSON tree
        /// for debugging and verification purposes.
        inline std::string print()
        {
            if (this->start_node == NULL)
            {
                return "JSON tree is empty\n";
            } else {
                std::string ret = "root";
                this->print_struct(this->start_node, &ret, 0);
                return ret;
            }
        }
        
        /// \brief Convert a json file stored as raw text into a tree
        /// representation, replacing any previous content if any.
        /// \return true if the parse was successful
        bool parse(const std::string raw_json_data);
        
        /// \brief Pointer to the first node of the JSON tree
        json_struct_t* start_node;
        
    protected:
        
        /// \brief Recursive helper function for \ref json_root_t::print()
        /// that appends to an output string and prints all child nodes
        void print_struct
        (
            
            /// [in] This node and all of its children will be printed
            const json_struct_t* cur_node,
            
            /// [out] A string to incrementally write the output to
            std::string* output,
            
            /// [in] The number of spaces to insert before any text printed
            /// by this call to the function.  Recursive calls will have
            /// increased indents.
            unsigned indent
            
        );
        
        /// \brief Recursive helper function for \ref json_root_t::print()
        /// that appends to an output string and prints all child nodes
        void print_array
        (
            
            /// [in] This node and all of its children will be printed
            const json_array_t* cur_node,
            
            /// [out] A string to incrementally write the output to
            std::string* output,
            
            /// [in] The number of spaces to insert before any text printed
            /// by this call to the function.  Recursive calls will have
            /// increased indents.
            unsigned indent
            
        );
        
        /// \brief Append spaces to a string
        inline void print_indent(std::string* output, unsigned indent)
        {
            for (unsigned ix = 0; ix < indent; ix++)
                *output += ' ';
        }
        
        /// \brief Recursive helper function for \ref json_root_t::parse() "parse"
        /// that is called when the current token being decoded is inside a JSON
        /// array (aka square brackets).  You should call this function with start_ix
        /// positioned at the first token after the \ref token_type_t "OPEN_SQUARE" of
        /// the struct you want to parse.
        /// \return The index of the \ref token_type_t "CLOSE_SQUARE" used to close this
        /// array.  If parsing fails, then this function returns 0.
        unsigned parse_array
        (
            
            /// [in] Parsed list of tokens to be converted to the json tree.
            const std::vector<json_token_t>& tokens,
            
            /// [in] The index into tokens of the first token
            /// to be parsed.  This index should be the first token after the opening
            /// \ref token_type_t "OPEN_CURLY".
            unsigned start_ix,
            
            /// [out] \ref json_struct_t::values "values" members of this array
            /// will be filled as more tokens are parsed.
            json_array_t* output
            
        );
        
        /// \brief Recursive helper function for \ref json_root_t::parse() "parse"
        /// that is called when the current token being decoded is inside a JSON
        /// struct (aka curly brackets).  You should call this function with start_ix
        /// positioned at the first token after the \ref token_type_t "OPEN_CURLY" of
        /// the struct you want to parse.
        /// \return The index of the \ref token_type_t "CLOSE_CURLY" used to close this
        /// struct.  If parsing fails, then this function returns 0.
        unsigned parse_struct
        (
            
            /// [in] Parsed list of tokens to be converted to the json tree.
            const std::vector<json_token_t>& tokens,
            
            /// [in] The index into tokens of the first token
            /// to be parsed.  This index should be the first token after the opening
            /// \ref token_type_t "OPEN_CURLY".
            unsigned start_ix,
            
            /// [out] The \ref json_struct_t::names "names" and \ref
            /// json_struct_t::values "values" members of this struct will be filled
            /// as more tokens are parsed.
            json_struct_t* output
            
        );
        
        /// \brief Recursivly copy the contents of another JSON tree into this
        /// tree under the assumption that this object has been default
        /// constructed or \ref json_root_t::clear "clear"ed.
        void copy(const json_root_t& other);
        
        /// \brief Recursivly destroy all child nodes if they exist.
        void clear();
        
};

/// \brief A leaf node of a JSON tree containing a single true/false value
class json_bool_t : public json_leaf_t
{
    
    public:
        
        /// \brief Initialize an empty JSON node with no children.
        json_bool_t() = default;
        
        /// \brief Initialization constrctor
        inline json_bool_t(const json_bool_t& other) = default;
        
        /// \brief Copy constructor
        inline json_bool_t(const bool other)
        {
            this->value = other;
        }
        
        /// \brief Destructor
        ~json_bool_t() = default;
        
        /// \brief Assignment operator
        inline json_bool_t& operator=(const json_bool_t& other) = default;
        
        /// \brief Assignment operator
        inline json_bool_t& operator=(const bool other)
        {
            this->value = other;
            return *this;
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::BOOLEAN;
        }
        
        /// \brief Value extracted from the JSON text
        bool value;
        
};

/// \brief a leaf node of a JSON tree containing an signed integer value
class json_int_t : public json_leaf_t
{
    
    public:
        
        /// \brief Default constructor
        inline json_int_t() = default;
        
        /// \brief Copy constructor
        inline json_int_t(const json_int_t& other) = default;
        
        /// \brief Initialization constructor
        inline json_int_t(signed other)
        {
            this->value = other;
        }
        
        /// \brief Destructor
        ~json_int_t() = default;
        
        /// \brief Assignment operator
        inline json_int_t& operator=(const json_int_t& other) = default;
        
        /// \brief Assignment operator
        inline json_int_t& operator=(signed other)
        {
            this->value = other;
            return *this;
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::INT;
        }
        
        /// \brief Value extracted from the JSON text
        signed value;
        
};

/// \brief A leaf node of a JSON tree containing a floating point value
class json_float_t : public json_leaf_t
{
    
    public:
        
        /// \brief Default constructor
        inline json_float_t() = default;
        
        /// \brief Copy constructor
        inline json_float_t(const json_float_t& other) = default;
        
        /// \brief Initialization constructor
        inline json_float_t(float other)
        {
            this->value = other;
        }
        
        /// \brief Destrctor
        ~json_float_t() = default;
        
        /// \brief Assignment operator
        inline json_float_t& operator=(const json_float_t& other) = default;
        
        /// \brief Assignment operator
        inline json_float_t& operator=(const float other)
        {
            this->value = other;
            return *this;
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::FLOAT;
        }
        
        /// \brief Value extracted from the JSON text
        float value;
        
};

/// \brief A leaf node of a JSON tree containing a string of zero or more
/// characters
class json_string_t : public json_leaf_t
{
    
    public:
        
        /// \brief Default constructor
        inline json_string_t() = default;
        
        /// \brief Copy constructor
        inline json_string_t(const json_string_t& other) = default;
        
        /// \brief Initialization constructor
        inline json_string_t(const std::string& other)
            : value(other)
        {
           // Intentionally left blank
        }
        
        /// \brief Initialization constructor
        inline json_string_t(const char* other)
            : value(other)
        {
           // Intentionally left blank
        }
        
        /// \brief Initialization constructor
        inline json_string_t(const char* buffer, unsigned length)
            : value(buffer, length)
        {
           // Intentionally left blank
        }
        
        /// \brief Destructor
        ~json_string_t() = default;
        
        /// \brief Assignment operator
        inline json_string_t& operator=(const json_string_t& other)
        {
            if (this == &other)
                return *this;
            this->value = other.value;
        }
        
        /// \brief Assignment operator
        inline json_string_t& operator=(const std::string& other)
        {
            this->value = other;
            return *this;
        }
        
        /// \brief Assignment operator
        inline json_string_t& operator=(const char* other)
        {
            this->value = other;
            return *this;
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::STRING;
        }
        
        /// \brief Value extracted from the JSON text
        std::string value;
                
};

/// \brief An interior node of a JSON tree containing zero or more child
/// elements which may be of different types stored in an array
class json_array_t : public json_interior_t
{
    
    public:
        
        /// \brief Initialize an empty JSON node with no children.
        json_array_t() = default;
        
        /// \brief Copy constructor
        inline json_array_t(const json_array_t& other)
        {
            this->copy(other);
        }
        
        /// \brief Destructor
        ~json_array_t()
        {
            this->clear();
        }
        
        /// \brief Assignment operator.  Makes an independent copy of other
        /// overwriting and destroying previous content (if any).
        inline json_array_t& operator=(const json_array_t& other)
        {
            
            // Check for self assignment
            if (this == &other)
                return *this;
            
            this->clear();
            this->copy(other);
            return *this;
            
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::ARRAY;
        }
        
        /// \brief The name of the JSON array
        std::string name;
        
        /// \brief List of child elements in this array.  Each element may be
        /// a different datatype.
        std::vector<json_omni_t*> values;
        
    protected:
        
        /// \brief Recursivly copy the contents of another JSON tree into this
        /// tree without checking if there is any previous content.
        void copy(const json_array_t& other);
        
        /// \brief Recursivly destroy all children in the \ref
        /// json_array_t::values "values" vector and sets the vector size to 0.
        void clear();
        
};

/// \brief An interior node containing zero or more child elements each
/// with their own name, value, and type.  This is similar to a hast or
/// dictionary datatype.
class json_struct_t : public json_interior_t
{
    
    public:
        
        /// \brief Initialize an empty JSON node with no children.
        json_struct_t() = default;
        
        /// \brief Copy constructor
        inline json_struct_t(const json_struct_t& other)
        {
            this->copy(other);
        }
        
        /// \brief Destructor
        ~json_struct_t()
        {
            this->clear();
        }
        
        /// \brief Assignment operator
        inline json_struct_t& operator=(const json_struct_t& other)
        {
            
            // Check for self assignment
            if (this == &other)
                return *this;
            
            this->clear();
            this->copy(other);
            return *this;
            
        }
        
        /// \brief Self identify
        inline json_type_t get_type() const
        {
            return json_type_t::STRUCT;
        }
        
        /// \brief Similar to keys in a hash.  Each name corresponds to the
        /// corresponding element in the \ref json_struct_t::values vector.
        std::vector<std::string> names;
        
        /// \brief List of elements extracted from the JSON text of potentially
        /// different datatypes.
        std::vector<json_omni_t*> values;
        
    protected:
        
        /// \brief Recursivly copy the contents of another JSON tree into this
        /// tree without checking if there is any previous content.
        void copy(const json_struct_t& other);
        
        /// \brief Recursivly destroy all child nodes.
        void clear();
        
};

#endif // header guard

