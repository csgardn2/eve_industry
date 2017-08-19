#ifndef HEADER_GUARD_ARGS
#define HEADER_GUARD_ARGS

#include <string>

/// @brief Utilities for command line parsing.
namespace args
{
    
    /// @brief This is thrown from
    /// args_t::parse() if the arguments were not valid.
    enum class error_t
    {
        BLUEPRINTS_IN_NOT_FOUND,
        ITEMS_IN_NOT_FOUND,
        STATIONS_IN_NOT_FOUND,
        CHARACTERS_IN_NOT_FOUND,
        /// @brief NUM_ENUMS must be last.
        NUM_ENUMS
    };
    
    /// @brief Generate a human-readable error message suitable for printing
    /// from an error code.
    const std::string& enum_to_string(error_t error);
    
    /// @brief Parse command line argument and package them into a structure.
    /// @todo Write args_t class
    class args_t
    {
        
        public:
            
            /// @brief Construct an argument structure in an uninitialized but
            /// valid state.
            inline args_t()
              : validated_(false)
            {
                // All work done in initialization list.
            }
            
            /// @brief Convenience constructor that immediatly @ref parse "parses"
            /// arguments.
            inline args_t(unsigned argc, char const* const* argv)
            {
                this->parse(argc, argv);
            }
            
            /// @brief Consume the arguments passed to this program and place them
            /// in the corresponding member variables of the object calling this
            /// member function.
            ///
            /// @exception error_t See @ref error_t "enum description".
            ///
            /// @return Value of @ref validated_ when parsing finishes.
            void parse
            (
                /// [in] The number of strings in the argv array.  You can copy
                /// this directly from @ref main(int argc, char** argv).
                unsigned argc,
                /// [in] Array of strings passed to this program on the command line.
                /// You can copy this directly from @ref main.
                char const* const* argv
            );
            
            /// @brief Read-only access to @ref validated_.
            inline bool validated() const
            {
                return this->validated_;
            }
            
        protected:
            
            /// Set to true if @ref parse is called and all required arguments 
            /// were parsed successfully.
            bool validated_;
            
    };
    
}

#endif // Header guard
