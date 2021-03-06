/// @file decryptor.h
/// @brief Declaration of @ref decryptor_t class
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Wednesday November 29 2017
/// * Documentation is generated by doxygen, see html/index.html

#ifndef HEADER_GUARD_DECRYPTOR
#define HEADER_GUARD_DECRYPTOR

#include <fstream>
#include <string_view>

#include "error.h"
#include "json.h"

/// @brief Wrapper class that indicates a decryptor and its bonuses.
class decryptor_t
{
        
    public:
        
        /// @brief Indicates a type of decryptor.
        enum class type_t {
            NO_DECRYPTOR,
            AUGMENTATION,
            OPTIMIZED_AUGMENTATION,
            SYMMETRY,
            PROCESS,
            ACCELERANT,
            PARITY,
            ATTAINMENT,
            OPTIMIZED_ATTAINMENT,
            // NUM_ENUMS must be last.
            NUM_ENUMS
        };
        
        /// @brief Default constructor
        inline decryptor_t() = default;
        
        // Add member initialization constructors here
        // Try to use initializer lists when possible.
        
        inline decryptor_t(type_t new_type)
          : type_(new_type)
        {
            // All work done in initializer list
        }
        
        // Add operators here if desired.
        
        inline bool operator==(const decryptor_t& source) const
        {
            return this->type_ == source.type_;
        }
        
        inline bool operator==(const type_t& source) const
        {
            return this->type_ == source;
        }
        
        inline bool operator!=(const decryptor_t& source) const
        {
            return !(*this == source);
        }
        
        inline bool operator!=(const type_t& source) const
        {
            return !(*this == source);
        }
        
        inline decryptor_t& operator=(type_t new_type)
        {
            this->type_ = new_type;
            return *this;
        }
        
        // Add member read and write functions
        
        inline type_t type() const
        {
            return this->type_;
        }
        
        inline void type(type_t new_type)
        {
            this->type_ = new_type;
        }
        
        // Add special-purpose functions here
        
        /// @brief Multiply this number by @ref blueprint_t::invent_t::probability_
        /// to get a new invention probability.
        float raw_probability_multiplier() const;
        
        /// @brief Add this number to @ref blueprint_t::invent_t::runs_.
        signed raw_max_run_modifier() const;
        
        /// @brief Add this number to @ref blueprint_t::invent_t::material_efficiency_.
        signed raw_material_efficiency_modifier() const;
        
        /// @brief Add this number to @ref blueprint_t::invent_t::time_efficiency_.
        signed raw_time_efficiency_modifier() const;
        
        /// @brief Apply a decryptor to a blueprint invention run.
        inline float modify_probability(float base_probability) const
        {
            return base_probability * this->raw_probability_multiplier();
        }
        
        /// @brief Apply a decryptor to a blueprint invention run.
        inline unsigned modify_max_runs(unsigned base_runs) const
        {
            signed ret = signed(base_runs) + this->raw_max_run_modifier();
            return ret < 0 ? 0 : ret;
        }
        
        /// @brief Apply a decryptor to a blueprint invention run.
        inline unsigned modify_material_efficiency(unsigned base_material_efficiency) const
        {
            signed ret = signed(base_material_efficiency) + this->raw_material_efficiency_modifier();
            if (ret < 0)
                return 0;
            else if (ret > 10)
                return 10;
            else
                return ret;
        }
        
        /// @brief Apply a decryptor to a blueprint invention run.
        inline unsigned modify_time_efficiency(unsigned base_time_efficiency) const
        {
            signed ret = signed(base_time_efficiency) + this->raw_time_efficiency_modifier();
            if (ret < 0)
                return 0;
            else if (ret > 20)
                return 20;
            else
                return ret;
        }
        
        /// @brief Get the item ID of this item used by the Swagger API.
        /// @exception error_message_t If this is a @ref type_t::NO_DECRYPTOR.
        uint64_t item_id() const;
        
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
        void write_to_json_file(std::ostream& file) const;
        
        /// @brief Serialize the content of this file into a string that
        /// conforms to the schema data/json/schema.json.
        ///
        /// @exception error_message_t
        void write_to_json_buffer(std::string& buffer) const;
        
    protected:
        
        // Try to make your members protected, even if they don't have to be.
        
        /// @brief See @ref type_t.
        type_t type_;
        
};

/// @brief Convenience alias to allow printing directly via cout or similar.
inline std::ostream& operator<<(std::ostream& stream, const decryptor_t& source)
{
    source.write_to_json_file(stream);
    return stream;
}

/// @brief Extraction operator for decoding.
std::istream& operator>>(std::istream& stream, decryptor_t& destination);

#endif // Header Guard

