/// \file
/// File Name:                      blueprint.h \n
/// Date created:                   Sun Jan 8 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_BLUEPRINT
#define HEADER_GUARD_BLUEPRINT

#include <string>
#include <vector>

/// \brief Used to mark blueprints as in-use so that they can be omitted from
/// a profitability report if desired
enum class availability_t
{
    AVAILABLE,
    MANUFACTURING,
    SELLING,
    UNAVAILABLE
};

/// \brief Return a human readable string corresponding to an input enum.
const char* availability_to_string(availability_t availability);

/// \brief Container which describes a single blueprint in the EvE universe.
class blueprint_t
{
    
    public:
        
        /// \brief Used for diagnostics and debugging.  Prints all the members
        /// of a blueprint on multiple lines.
        std::string print() const;
        
        /// \brief The name of the output product that is produced by this
        /// blueprint
        std::string name;
        
        /// \brief See \ref availability_t
        availability_t availability;
        
        /// \brief The number of seconds it takes to perform a single run of a
        /// blueprint.  This value is always that of an unresearched blueprint
        /// as if it were at 0 TE.
        float time;
        
        /// \brief The volume (in m3) of a single output product
        float volume;
        
        /// \brief The number of output items produced by a single run of this
        /// blueprint
        float output_quantity;
        
        /// \brief The material efficiency of a blueprint.  0.0 indicates
        /// and unresearched blueprint and 10. indicates a fully researced
        /// blueprint (only 90% of the input materials are required)
        float me;
        
        /// \brief The time efficiency of a blueprint.  0.0 indicates
        /// and unresearched blueprint and 20. indicates a fully researced
        /// blueprint (only 80% of the \ref blueprint_t::time "time" value is
        /// required).
        float te;
        
        /// \brief A list of input items required for a blueprint.  For example,
        /// if a blueprint required 1200 Tritanium and 400 pyerite for a single
        /// run then \ref blueprint_t::input_materials "input_materials" would
        /// be {"Tritanium", "Pyerite"} and \ref blueprint_t::input_quantities 
        /// "input_quantities" would be {1200.0, 400.0}.  The size of
        /// input_materials and input_quantities must always be the same.
        std::vector<std::string> input_materials;
        
        /// \brief A list of input items quantities required for a blueprint.
        /// For example, if a blueprint required 1200 Tritanium and 400 pyerite
        /// for a single run then \ref blueprint_t::input_materials
        /// "input_materials" would be {"Tritanium", "Pyerite"} and \ref
        /// blueprint_t::input_quantities "input_quantities" would be
        /// {1200.0, 400.0}.  The size of input_materials and input_quantities
        /// must always be the same.
        std::vector<float> input_quantities;
        
};

#endif // header guard

