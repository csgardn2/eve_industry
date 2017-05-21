/// \file
/// File Name:                      arguments.h \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_ARGUMENTS
#define HEADER_GUARD_ARGUMENTS

#include <fstream>
#include <string>

#include "region.h"
#include "station.h"

/// \brief Used to track 
enum class argument_status_t
{
    
    /// \brief The state of an argument after the \ref arguments_t::arguments_t()
    /// "default constructor" or \ref arguments_t::reset() is called.
    UNINITIALIZED,
    
    /// \brief An argument was found but was invalid (such as attempting to open
    /// a read only file for writing)
    FAILED,
    
    /// \brief An argument was not specified so it took on a default value
    DEFAULTED,
    
    /// \brief An argument was specified and did not cause problems so far
    FOUND
    
};

/// Command line usage
/// Long version (two -'s) | Short Version | Arguments                        | Necessity                                                              | Comments
/// :--------------------: | :-----------: | :-------:                        | :--------------------------------------------------------------------: | :--:
/// --region               | -r            | [region name]                    | Mandatory                                                              | Specify a region in eve such as "The Forge" or "Domain" to fetch market data from.  This parameter is not case sensitive and ignores whitespace.
/// --station              | -s            | [station id]                     | Optional                                                               | Jita = 60003760.  Amarr = 60008494.  Dodixie = 60011866.  Rens = 60004588.  If omitted, market data from all stations in the given region will be used.
/// --blueprints-file      | -b            | [file.json]                      | Mandatory, but defaults to "blueprints.json" if omitted                | This is an input json file which contains a list of all the blueprints you own.  The blueprints file specifies things such as required input materials, material efficiency, time efficiency, and other statistics
/// --profitability-file   | -p            | [file.txt]                       | Mandaroty, but defaults to "profitability.csv" if omitted              | This is an output file which will contain a report on the profitability of all the blueprints you own based on the current eve market.
/// --crest-root           | -c            | [https://crest-tq.eveonline.com] | Mandatory, but defaults to "https://crest-tq.eveonline.com" if omitted | Specifies where to start looking for EvE market data
/// --item-id-cache        | -i            | [item_id_cache.csv]              | Mandatory, but defaults to "item_id_cache.csv" if omitted              | Pulling item IDs from the EvE server takes a long time.  IDs are saved to this file when fetched and read from this file if it exists, bypassing the server.
/// --order-cache          | -o            | [order_cache.csv]                | Mandatory, but defaults to "order_cache.csv" if omitted                | Pulling maket data from the EvE server takes a long time.  Orders are saved to this file when fetched and read from this file if it exists, bypassing the server.
/// --cycle-time           | -t            | Number of seconds                | Mandatory, but defaults to 86400 (24 hours) if omitted                 | The maximum number of seconds you want your manufacturing jobs to take.  Helps estimate your bill of materials but doesn't affect item profitability.
class arguments_t
{
    
    public:
        
        /// \brief Construct an empty but valid \ref arguments_t object.  You
        /// can initialize it later with \ref arguments_t::parse_arguments
        inline arguments_t()
        {
            this->reset();
        }
        
        /// Construct and initialize
        inline arguments_t(unsigned argc, char** argv)
        {
            this->parse_arguments(argc, argv);
        }
        
        /// \brief Parse and partially validate arguments from the command line
        /// \return the value of \ref valid
        bool parse_arguments(unsigned argc, char** argv);
        
        /// \brief Zero out all previous state.  The arguments_t object will be
        /// the same as a default-constructed one.
        void reset();
        
        /// \brief True if sufficient arguments were passed to create the final
        /// profitability file
        bool valid;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_region;
        
        /// \brief Argument to --region or -r if \ref found_region is DEFAULTED
        /// or FOUND
        std::string region;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_station_id;
        
        /// \brief Argument to --station or -s if \ref found_station_id is
        /// DEFAULTED or FOUND
        unsigned station_id;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_blueprints_file;
        
        /// \brief File opened for reading specified by --blueprints-file or -b
        /// if \ref found_blueprints_file is DEFAULTED or FOUND.
        std::ifstream blueprints_file;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_profitability_file;
        
        /// \brief File opened for writing specified by --profitability-file or
        /// -p if \ref found_profitability_file is DEFAULTED or FOUND.
        std::ofstream profitability_file;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_crest_root;
        
        /// \brief Argument to --crest-root or -c if \ref found_crest_root is
        /// true.
        std::string crest_root;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_item_id_cache_file_name;
        
        /// \brief Filename for a file which containes the names and IDs of 
        /// all the items in EvE.  This is kept as a string since the file
        /// will sometimes be opened for reading or writing.
        std::string item_id_cache_file_name;
        
        /// \brief See \ref argument_status_t
        argument_status_t found_order_cache_file_name;
        
        /// \brief Filename for a file which containes the market orders of 
        /// the last CREST fetch.  This is kept as a string since the file
        /// will sometimes be opened for reading or writing.
        std::string order_cache_file_name;
                
        /// \brief See \ref argument_status_t
        argument_status_t found_cycle_time;
        
        /// \brief The maximum number of seconds you want your manufacturing
        /// jobs to take.  Helps estimate your bill of materials but doesn't
        /// affect item profitability.
        float cycle_time;
        
};

#endif // header guard

