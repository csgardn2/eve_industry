/// \file
/// File Name:                      arguments.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or arguments.h for documentation.

#include <climits>
#include <cstring>
#include <iostream>
#include <vector>

#include "arguments.h"
#include "station.h"
#include "utility.h"

/// \brief returns the index of an argument given its long and short switches
/// returns UINT_MAX if the argument was not found
inline unsigned locate_arg
(
    unsigned argc,
    char** argv,
    const char* long_switch,
    const char* short_switch
){
    
    for (unsigned ix = 0; ix < argc; ix++)
    {
        const char* candidate = argv[ix];
        if (strcmp(candidate, long_switch) == 0 || strcmp(candidate, short_switch) == 0)
            return ix;
    }
    
    return UINT_MAX;
    
}

/// \brief Parse and partially validate arguments from the command line
bool arguments_t::parse_arguments(unsigned argc, char** argv)
{
    
    // If a --region or --station string is ambiguous, this limits the number
    // of matches that may be printed to the screen.
    static const char* default_blueprints_file_name = "blueprints.json";
    static const char* default_profitability_file_name = "profitability.csv";
    static const char* default_crest_root = "https://crest-tq.eveonline.com";
    static const char* default_item_id_cache_file_name = "item_id_cache.csv";
    static const char* default_order_cache_file_name = "order_cache.csv";
    static const float default_cycle_time = 86400; // 24 hours
    
    this->reset();
    
    bool processed_args[argc];
    for (unsigned ix = 0; ix < argc; ix++)
        processed_args[ix] = false;
    
    // --region -r
    unsigned cur_arg_index = locate_arg(argc, argv, "--region", "-r");
    if (cur_arg_index == UINT_MAX)
    {
        
        std::cerr << "Error.  --region not given as a command line switch\n";
        this->found_region = argument_status_t::FAILED;
        return false;
        
    } else if (cur_arg_index + 1 >= argc) {
        
        processed_args[cur_arg_index] = true;
        std::cerr
            << "Error.  "
            << argv[cur_arg_index]
            << " requires exactly one argument\n";
        this->found_region = argument_status_t::FAILED;
        // Abort, seince --region is a required argument
        return false;
        
    } else {
        
        processed_args[cur_arg_index] = true;
        processed_args[cur_arg_index + 1] = true;
        this->region = argv[cur_arg_index + 1];
        this->found_region = argument_status_t::FOUND;
        
    }
    
    // --station -s
    cur_arg_index = locate_arg(argc, argv, "--station", "-s");
    if (cur_arg_index != UINT_MAX)
    {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            std::cerr
                << "Warning.  "
                << argv[cur_arg_index]
                << " requires exactly one argument. "
                   "Gathering prices from all of \""
                << this->region
                << "\" instead.  Common station IDs include:\n"
                   "    Jita = 60003760\n"
                   "    Amarr = 60008494\n"
                   "    Dodixie = 60011866\n"
                   "    Rens = 60004588\n";
            this->found_station_id = argument_status_t::FAILED;
            
        } else {
            
            processed_args[cur_arg_index + 1] = true;
            this->station_id = atoi(argv[cur_arg_index + 1]);
            this->found_station_id = argument_status_t::FOUND;
            
        }
    }
    
    // --blueprints-file -b
    cur_arg_index = locate_arg(argc, argv, "--blueprints-file", "-b");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Argument not passed.  Attempt to open default file name
        this->blueprints_file.open(default_blueprints_file_name);
        if (this->blueprints_file.good())
        {
            
            // Opened default file successfully
            this->found_blueprints_file = argument_status_t::DEFAULTED;
            
        } else {
            
            // Failed to open default file
            this->found_blueprints_file = argument_status_t::FAILED;
            std::cerr
                << "Error.  A --blueprints-file was not specified so an "
                   "attempt was made to open the default name \""
                << default_blueprints_file_name
                << "\" but the file could not be opened for reading\n";
            return false;
            
        }
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but was not followed by a filename
            std::cerr
                << "Error.  "
                << argv[cur_arg_index + 1]
                << " requires exactly one argument.\n";
            
        } else {
            
            processed_args[cur_arg_index + 1] = true;
            
            this->blueprints_file.open(argv[cur_arg_index + 1]);
            if (this->blueprints_file.good())
            {
                
                // Opened blueprint file successfully
                this->found_blueprints_file = argument_status_t::FOUND;
                
            } else {
                
                // Failed to open blueprint file
                this->found_blueprints_file = argument_status_t::FAILED;
                std::cerr
                    << "Error.  Failed to open \""
                    << argv[cur_arg_index + 1]
                    << "\" for reading\n";
                return false;
                
            }
            
        }
        
    }
    
    // --profitability-file -p
    cur_arg_index = locate_arg(argc, argv, "--profitability-file", "-p");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Argument not passed.  Attempt to open default file name
        this->profitability_file.open(default_profitability_file_name);
        if (this->profitability_file.good())
        {
            
            // Default file opened successfully
            this->found_profitability_file = argument_status_t::DEFAULTED;
            
        } else {
            
            // Failed to open default file
            this->found_profitability_file = argument_status_t::FAILED;
            std::cerr
                << "Error.  A --profitability-file was not specified so an "
                   "attempt was made to open the default name \""
                << default_profitability_file_name
                << "\" but the file could not be opened for writing\n";
            return false;
            
        }
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but is not followed by a filename
            std::cerr
                << "Error.  "
                << argv[cur_arg_index + 1]
                << " requires exactly one argument.\n";
            
        } else {
            
            processed_args[cur_arg_index + 1] = true;
            
            this->profitability_file.open(argv[cur_arg_index + 1]);
            if (this->profitability_file.good())
            {
                
                // Failed to open specified file
                this->found_profitability_file = argument_status_t::FOUND;
                
            } else {
                
                // Argument parsed successfully
                this->found_profitability_file = argument_status_t::FAILED;
                std::cerr
                    << "Error.  Failed to open \""
                    << argv[cur_arg_index + 1]
                    << "\" for writing\n";
                return false;
                
            }
            
        }
        
    }
    
    // --crest-root -c
    cur_arg_index = locate_arg(argc, argv, "--crest-root", "-c");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Arguemnt not passed.  Setting to default
        this->found_crest_root = argument_status_t::DEFAULTED;
        this->crest_root = default_crest_root;
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but does not followed by web address
            this->found_crest_root = argument_status_t::FAILED;
            std::cerr
                << "Error.  "
                << argv[cur_arg_index]
                << " requires exactly one argument.\n";
            return false;
            
        } else {
            
            // Crest root initialized successfully
            this->crest_root = argv[cur_arg_index + 1];
            this->found_crest_root = argument_status_t::FOUND;
            processed_args[cur_arg_index + 1] = true;
            
        }
        
    }
    
    // -item-id-cache -i
    cur_arg_index = locate_arg(argc, argv, "--item-id-cache", "-i");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Argument not passed.  Setting to default.
        this->found_item_id_cache_file_name = argument_status_t::DEFAULTED;
        this->item_id_cache_file_name = default_item_id_cache_file_name;
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but not followed by a string
            this->found_item_id_cache_file_name = argument_status_t::FAILED;
            std::cerr
                << "Error.  "
                << argv[cur_arg_index]
                << " requires exactly one argument.\n";
            return false;
            
        } else {
            
            // Item ID cache file name initialized successfully
            this->item_id_cache_file_name = argv[cur_arg_index + 1];
            this->found_item_id_cache_file_name = argument_status_t::FOUND;
            processed_args[cur_arg_index + 1] = true;
            
        }
        
    }
    
    // -order-cache -o
    cur_arg_index = locate_arg(argc, argv, "--order-cache", "-o");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Argument not passed.  Setting to default.
        this->found_order_cache_file_name = argument_status_t::DEFAULTED;
        this->order_cache_file_name = default_order_cache_file_name;
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but not followed by a string
            this->found_order_cache_file_name = argument_status_t::FAILED;
            std::cerr
                << "Error.  "
                << argv[cur_arg_index]
                << " requires exactly one argument.\n";
            return false;
            
        } else {
            
            // Item ID cache file name initialized successfully
            this->order_cache_file_name = argv[cur_arg_index + 1];
            this->found_order_cache_file_name = argument_status_t::FOUND;
            processed_args[cur_arg_index + 1] = true;
            
        }
        
    }
    
    // -cycle-time -t
    cur_arg_index = locate_arg(argc, argv, "--cycle-time", "-t");
    if (cur_arg_index == UINT_MAX)
    {
        
        // Argument not passed.  Setting to default.
        this->found_cycle_time = argument_status_t::DEFAULTED;
        this->cycle_time = default_cycle_time;
        
    } else {
        
        processed_args[cur_arg_index] = true;
        
        if (cur_arg_index + 1 >= argc)
        {
            
            // Argument passed but not followed by a number
            this->found_cycle_time = argument_status_t::FAILED;
            std::cerr
                << "Error.  "
                << argv[cur_arg_index]
                << " requires exactly one argument.\n";
            return false;
            
        } else {
            
            float cycle_time = atof(argv[cur_arg_index + 1]);
            if (cycle_time <= 0.0f)
            {
                std::cerr << "Error.  Cycle time must be positive and greater than zero.\n";
                return false;
            }
            this->cycle_time = cycle_time;
            this->found_cycle_time = argument_status_t::FOUND;
            processed_args[cur_arg_index + 1] = true;
            
        }
        
    }
    
    // Print warnings for unrecognized command line parameters
    for (unsigned ix = 1; ix < argc; ix++)
    {
        if (!processed_args[ix])
        {
            std::cerr
                << "Warning.  Command line argument \""
                << argv[ix]
                << "\" was unrecognized and unparsed\n";
        }
    }
    
    this->valid = true;
    return true;
    
}

/// \brief Zero out all previous state.  The arguments_t object will be
/// the same as a default-constructed one.
void arguments_t::reset()
{
    
    this->valid = false;
    
    this->found_region = argument_status_t::UNINITIALIZED;
    this->region.clear();
    
    this->found_station_id = argument_status_t::UNINITIALIZED;
    this->station_id = 0;
    
    this->found_blueprints_file = argument_status_t::UNINITIALIZED;
    if (this->blueprints_file.is_open())
        this->blueprints_file.close();
    
    this->found_profitability_file = argument_status_t::UNINITIALIZED;
    if (this->profitability_file.is_open())
        this->profitability_file.close();
    
    this->found_crest_root = argument_status_t::UNINITIALIZED;
    this->crest_root.clear();
    
    this->found_item_id_cache_file_name = argument_status_t::UNINITIALIZED;
    this->item_id_cache_file_name.clear();
    
}

