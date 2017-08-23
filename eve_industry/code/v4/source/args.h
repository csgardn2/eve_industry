#ifndef HEADER_GUARD_ARGS
#define HEADER_GUARD_ARGS

#include <fstream>
#include <string>

/// @brief Utilities for command line parsing.
namespace args
{
    
    enum class mode_t
    {
        
        /// Connect to the EvE RESTful API and download the current market
        /// prices of every item in @ref items_in_file_ using prices from every
        /// station in @ref stations_in_file_
        FETCH_PRICES,
        
        /// Compute the ratio of (output material price) / (input material price)
        /// for each @ref blueprint_file_in_ using the prices in @ref
        /// prices_in_.
        CALCULATE_BLUEPRINT_PROFITABILITY,
        
        /// Generate a text file in no par TODO
        GENERATE_TODO_LIST
    };
    
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
            
            std::ifstream stations_in_file_;
            
            std::ifstream items_in_file_;
            
            std::ifstream prices_in_file_;
            
            std::ofstream prices_out_file_;
            
            std::ifstream blueprints_in_file_;
            
            std::ifstream assembly_lines_in_file_;
            
            std::ifstream jobs_in_file_;
            
            std::ofstream jobs_out_file_;
            
            std::ifstream profitability_in_file_;
            
            std::ofstream profitability_out_file_;
            
            std::ofstream todo_list_out_file_;
            
            /// Set to true if @ref parse is called and all required arguments 
            /// were parsed successfully.
            bool validated_;
            
    };
    
}

#endif // Header guard
