/// @file args_slices.h
/// @brief Declaration of @ref args::slices_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday December 7 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_ARGS_SLICES
#define HEADER_GUARD_ARGS_SLICES

#include <fstream>
#include <string_view>
#include <unordered_map>

#include "error.h"
#include "json.h"

namespace args
{
    
    /// @brief A single command line argument starting with '-' plust all
    /// subsequent parameters up until but not including the next argument
    /// beginning with '-'.
    typedef std::unordered_map<std::string, std::vector<std::string> > arg_map_t;
    
    /// @brief Pre-processed command line arguments plus the parameters to each
    /// argument.
    ///
    /// The keys in @ref storage_ are arguments starting with '-'.
    /// The values in @ref storage_ are the strings following the key up until
    /// the next '-' prefixed argument.
    ///
    /// * input
    ///     {"--verbose", "--mode", "fetch", "--input-file", "a.txt", "b.txt", "c.txt"}
    /// * output
    ///     key             | values
    ///     :-------------: | :---
    ///     "--verbose"     | {}
    ///     "--mode"        | {"fetch"}
    ///     "--input-file"  | {"a.txt", "b.txt", "c.txt"}
    class slices_t
    {
            
        public:
            
            /// @brief Default constructor
            inline slices_t() = default;
            
            // Add member initialization constructors here
            // Try to use initializer lists when possible.
            
            // Add operators here if desired.
            
            // Add member read and write functions
            
            const arg_map_t& storage() const
            {
                return this->storage_;
            }
            
            // Add special-purpose functions here
            
            inline void clear()
            {
                this->storage_.clear();
            }
            
            /// @brief Initialize @ref storage_ to look like @ref slices_t
            /// "the class description" using raw command line arguments.
            void initialize_from_command_line(unsigned argc, char const* const* argv);
            
            // Add encoders/decoders here
            
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
            void read_from_json_structure(const Json::Value& json_root);
            
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
            
        protected:
            
            // Try to make your members protected, even if they don't have to be.
            
            /// @brief See @ref slices_t "class description".
            arg_map_t storage_;
            
    };
    
}

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const args::slices_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, args::slices_t& destination);

#endif // Header Guard
