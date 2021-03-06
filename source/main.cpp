/// @file main.cpp
/// @brief Entry point for this program
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <iostream>
#include <unordered_set>

#include "args_eve_industry.h"
#include "blueprints.h"
#include "error.h"
#include "galactic_market.h"
#include "item_attributes.h"
#include "item_ids.h"
#include "galactic_profits.h"
#include "station_attributes.h"
#include "raw_regional_market.h"

/// @brief Code execution starts here
int main(int argc, char** argv)
{
    
    try
    {
        
        args::eve_industry_t args;
        args.parse(argc, argv);
        
        switch (args.mode())
        {
            
            case args::eve_industry_t::mode_t::FETCH_ITEM_ATTRIBUTES:
            {
                
                // Open output file first so that we can find out if it fails
                // before doing all the work of fetching.
                std::ofstream item_attributes_out_file(args.item_attributes_out());
                if (!item_attributes_out_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.item_attributes_out() << "\" for writing.\n";
                    return -1;
                }
                
                if (args.debug_mode().verbose())
                    std::cout << "Fetching item ids\n";
                item_ids_t item_ids;
                item_ids.debug_mode(args.debug_mode());
                item_ids.fetch();
                
                if (args.debug_mode().verbose())
                    std::cout << "Fetching item attributes\n";
                item_attributes_t item_attributes;
                item_attributes.debug_mode(args.debug_mode());
                item_attributes.fetch(item_ids);
                    
                if (args.debug_mode().verbose())
                    std::cout << "Writing item attributes to file.\n";
                item_attributes.write_to_json_file(item_attributes_out_file);
                item_attributes_out_file.close();
                
                break;
                
            }
            
            case args::eve_industry_t::mode_t::FETCH_PRICES:
            {
                
                // Open item attributes from file
                if (args.debug_mode().verbose())
                    std::cout << "Parsing item-attributes-in file \"" << args.item_attributes_in() << "\".\n";
                std::ifstream item_attributes_in_file(args.item_attributes_in());
                if (!item_attributes_in_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.item_attributes_in() << "\" for reading.\n";
                    return -1;
                }
                item_attributes_t item_attributes_in;
                item_attributes_in.read_from_json_file(item_attributes_in_file);
                item_attributes_in_file.close();
                
                // Open station attributes from file
                if (args.debug_mode().verbose())
                    std::cout << "Parsing station-attributes-in file\"" << args.station_attributes_in() << "\".\n";
                std::ifstream station_attributes_in_file(args.station_attributes_in());
                if (!station_attributes_in_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.station_attributes_in() << "\" for reading.\n";
                    return -1;
                }
                station_attributes_t station_attributes_in;
                station_attributes_in.read_from_json_file(station_attributes_in_file);
                station_attributes_in_file.close();
                
                // Open file to output market data to
                std::ofstream prices_out_file(args.prices_out());
                if (!prices_out_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.prices_out() << "\" for writing.\n";
                    return -1;
                }
                
                // Fetch market data for the regions of all requested stations.
                galactic_market_t galactic_market;
                for (const station_attribute_t& cur_station : station_attributes_in.stations())
                {
                    
                    if (args.debug_mode().verbose())
                        std::cout << "Fetching market data for station \"" << cur_station.name() << "\".\n";
                    
                    // Do not re-fetch data for the same region if we've already fetched it.
                    if (galactic_market.regions().count(cur_station.station_id()))
                        continue;
                    
                    uint64_t cur_region_id = cur_station.region_id();
                    if (args.debug_mode().verbose())
                        std::cout << "Fetching all market orders from region " << cur_region_id << '\n';
                    raw_regional_market_t raw_regional_market;
                    raw_regional_market.fetch(cur_region_id);
                    
                    // Post-process market data
                    if (args.debug_mode().verbose())
                        std::cout << "Post-processing regional market for region " << cur_region_id << '\n';
                    regional_market_t regional_market;
                    regional_market.initialize_from_raw_regional_market(raw_regional_market);
                    
                    // Augmenting galactic market
                    if (args.debug_mode().verbose())
                        std::cout << "Augmenting galactic market data.\n";
                    galactic_market.add_new_regional_market(std::move(regional_market));
                    
                }
                
                // Remove market data for stations we aren't interested in
                if (args.cull_stations())
                {
                    
                    if (args.debug_mode().verbose())
                        std::cout << "Culling orders from non-requested stations.\n";
                    
                    // Build list of stations to keep
                    std::unordered_set<uint64_t> stations_to_keep;
                    const std::vector<station_attribute_t>& stations = station_attributes_in.stations();
                    unsigned num_stations = stations.size();
                    stations_to_keep.reserve(num_stations);
                    for (unsigned ix = 0; ix < num_stations; ix++)
                        stations_to_keep.emplace(stations[ix].station_id());
                    
                    // Strip away orders for stations that we didn't ask for
                    galactic_market.cull_by_station(stations_to_keep);
                    
                }
                
                if (args.cull_orders())
                {
                    if (args.debug_mode().verbose())
                        std::cout << "Culling all except minimum sell and maximum buy orders.\n";
                    galactic_market.cull_unavailable_orders();
                }
                
                // Write market data to file
                if (args.debug_mode().verbose())
                    std::cout << "Writing galactic market data to file \"" << args.prices_out() << "\".\n";
                galactic_market.write_to_json_file(prices_out_file);
                prices_out_file.close();
                
                break;
                
            }
            
            case args::eve_industry_t::mode_t::CALCULATE_BLUEPRINT_PROFITS:
            {
                
                // Open prices file for reading
                if (args.debug_mode().verbose())
                    std::cout << "Parsing prices-in file \"" << args.prices_in() << "\".\n";
                std::ifstream prices_in_file(args.prices_in());
                if (!prices_in_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.prices_in() << "\" for reading.\n";
                    return -1;
                }
                galactic_market_t galactic_market;
                galactic_market.read_from_json_file(prices_in_file);
                
                // Open blueprints file for reading
                if (args.debug_mode().verbose())
                    std::cout << "Parsing blueprints-in file \"" << args.blueprints_in() << "\".\n";
                std::ifstream blueprints_in_file(args.blueprints_in());
                if (!blueprints_in_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.blueprints_in() << "\" for reading.\n";
                    return -1;
                }
                blueprints_t blueprints_in;
                blueprints_in.read_from_json_file(blueprints_in_file);
                
                // Open optional item_attributes_in file for reading
                std::unordered_map<uint64_t, std::string_view> blueprint_names; // <-- Empty and valid if argument not passed
                item_attributes_t item_attributes_in;
                if (!args.item_attributes_in().empty())
                {
                    
                    // Open file
                    if (args.debug_mode().verbose())
                        std::cout << "Parsing item-attributes-in file \"" << args.item_attributes_in() << "\".\n";
                    std::ifstream item_attributes_in_file(args.item_attributes_in());
                    if (!item_attributes_in_file.good())
                    {
                        std::cerr << "Error.  Failed to open \"" << args.item_attributes_in() << "\" for reading.\n";
                        return -1;
                    }
                    
                    // Parse JSON
                    item_attributes_in.read_from_json_file(item_attributes_in_file);
                    
                    // Generate ID -> Name mapping
                    for (const item_attribute_t& cur_item : item_attributes_in.items())
                        blueprint_names.emplace(cur_item.id(), std::string_view(cur_item.name()));
                    
                }
                
                // Calculate blueprint profit at each station
                if (args.debug_mode().verbose())
                    std::cout << "Calculating blueprint profitability.\n";
                galactic_profits_t galactic_profits_out;
                galactic_profits_out.caclulate_station_profits(blueprints_in, galactic_market, args.decryptor_optimization_strategy());
                
                // Write profits report to file.
                if (args.debug_mode().verbose())
                    std::cout << "Writing profits-out file \"" << args.profits_out() << "\".\n";
                std::ofstream profits_out_file(args.profits_out());
                if (!profits_out_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.profits_out() << "\" for reading.\n";
                    return -1;
                }
                galactic_profits_out.write_to_json_file(profits_out_file, args.output_order(), blueprint_names);
                profits_out_file.close();
                
                break;
                
            }
            
            default:
            {
                std::cerr << "Error.  Unimplemented mode \"" << args::eve_industry_t::enum_to_string(args.mode()) << "\".\n";
                return -1;
            }
            
        }
    } catch (Json::Exception error) {
        std::cerr << error.what() << "  Fatal.  Could not decode JSON input file.\n";
        return -1;
    } catch (error_message_t error) {
        std::cerr << error;
        return -1;
    }
    
    return 0;
    
}

