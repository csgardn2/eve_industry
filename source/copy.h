/// @file copy.h
/// @brief Declaration of @ref copy_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 24 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_COPY
#define HEADER_GUARD_COPY

// Don't include anything
// Let blueprint.h include all your files
// This way you won't actually make any extra inner classes accidently.

/// @brief All of the blueprint attributes related to copy runs.
class copy_t
{
        
    public:
        
        /// @brief Default constructor
        inline copy_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.

        // Add member read and write functions
        
        inline bool valid() const
        {
            return this->valid_;
        }
        
        inline void valid(bool new_valid) {
            this->valid_ = new_valid;
        }
        
        unsigned time() const
        {
            if (!this->valid_)
                throw error_message_t(error_code_t::READ_INVALID_COPY, "Error.  Tried to read blueprint_t::copy_t::time_ from an invalid object.\n");
            return this->time_;
        }
        
        void time(unsigned new_time)
        {
            this->time_ = new_time;
        }
        
        const item_quantities_t& input_materials() const
        {
            if (!this->valid_)
                throw error_message_t(error_code_t::READ_INVALID_COPY, "Error.  Tried to read blueprint_t::copy_t::input_materials_ from an invalid object.\n");
            return this->input_materials_;
        }
        
        item_quantities_t& input_materials()
        {
            return this->input_materials_;
        }
        
        // Add special-purpose functions here
        
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
        
        /// @brief Set to true if this is a blueprint original that can be copied
        /// False if you can only acquire copies of this blueprint.
        ///
        /// A value of false indicates that an copy field was not found while
        /// decoding a @ref blueprint_t.
        ///
        /// A value of false also indicates that this copy field should not be
        /// encoded when writing a blueprint_t.
        bool valid_;
        
        /// @brief The number of seconds to create a single 1-run copy of this
        /// blueprint at 0TE.
        unsigned time_;
        
        /// @brief List of materials and quantities consumed during 1 copy run
        /// at 0 ME.  Usually empty.
        item_quantities_t input_materials_;
        
};

#endif // Header Guard

