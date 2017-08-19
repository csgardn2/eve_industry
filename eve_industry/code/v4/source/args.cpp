#include <string>
#include "args.h"

namespace args
{
    
    const std::string& enum_to_string(error_t error)
    {
        
        static const std::string error_messages[unsigned(error_t::NUM_ENUMS)] =
        {
            "Error.  You must specify a JSON input file containing your blueprint data with --blueprints-in FILE.", // BLUEPRINTS_IN_NOT_FOUND
            "Error.  You must specify a JSON input file containing your item data with --items-in FILE.", // ITEMS_IN_NOT_FOUND
            "Error.  You must specify a JSON input file containing your station data with --stations-in FILE.", // STATIONS_IN_NOT_FOUND
            "Error.  You must specify a JSON input file containing your character data with --characters-in FILE." // CHARACTERS_IN_NOT_FOUND
        };
        
        return error_messages[unsigned(error)];
        
    }
    
    void args_t::parse(unsigned argc, char const* const* argv)
    {
        /// @todo Conor, you left off here.  You can start by loading a blueprint file.
    }
    
}

