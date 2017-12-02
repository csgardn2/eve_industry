/// @file manufacture.h
/// @brief Declaration of @ref manufacture_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Friday November 24 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_MANUFACTURE
#define HEADER_GUARD_MANUFACTURE

// Don't include anything
// Let blueprint.h include all your files
// This way you won't actually make any extra inner classes accidently.

/// @brief All of the blueprint attributes related to manufacturing runs.
class manufacture_t
{
        
    public:
        
        /// @brief Default constructor
        inline manufacture_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        // Add operators here if desired.
        
        // Add member read and write functions
        
        inline unsigned time() const
        {
            return this->time_;
        }
        
        inline void time(unsigned new_time)
        {
            this->time_ = new_time;
        }
        
        inline const item_quantities_t& input_materials() const
        {
            return this->input_materials_;
        }
        
        inline item_quantities_t& input_materials()
        {
            return this->input_materials_;
        }
        
        inline const item_quantities_t& output_materials() const
        {
            return this->output_materials_;
        }
        
        inline item_quantities_t& output_materials()
        {
            return this->output_materials_;
        }
        
        inline unsigned material_efficiency() const
        {
            return this->material_efficiency_;
        }
        
        /// @exception error_message_t.
        inline void material_efficiency(unsigned new_material_efficiency)
        {
            validate_material_efficiency(new_material_efficiency);
            this->material_efficiency_ = new_material_efficiency;
        }
        
        inline unsigned time_efficiency() const
        {
            return this->time_efficiency_;
        }
        
        /// @exception error_message_t.
        inline void time_efficiency(unsigned new_time_efficiency)
        {
            validate_time_efficiency(new_time_efficiency);
            this->time_efficiency_ = new_time_efficiency;
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
        
        /// @brief The number of seconds required for 1 manufacturing run at 0 TE.
        unsigned time_;
        
        /// @brief List of materials and quantities consumed during 1
        /// manufacturing run at 0 ME.
        item_quantities_t input_materials_;
        
        /// @brief The items produced by a single successfull manufacturing run
        /// of this blueprint.
        item_quantities_t output_materials_;
        
        /// @brief Percent reduction in required input materials.
        /// 
        /// Valid values
        /// are [0 to 10].
        unsigned material_efficiency_;
        
        /// @brief Percent reduction in the time required for manufacture.
        ///
        /// Valid values are [0 to 20].
        unsigned time_efficiency_;
        
};

#endif // Header Guard

