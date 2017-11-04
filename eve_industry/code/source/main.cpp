/// @file main.cpp
/// @brief Entry point for this program
/// 
/// * Contact conor.gardner@arm.com if you have questions about this code.
/// * Date Created = Thursday September 14 2017
/// * Documentation is generated by doxygen, see documentation/html_out/index.html

#include <fstream>
#include <iostream>

#include "args.h"
#include "item_attributes.h"
#include "item_ids.h"
#include "station_attributes.h"
#include "regional_market_orders.h"

/// @brief Code execution starts here
int main(int argc, char** argv)
{
    
    // Parse command line arguments
    args_t args;
    try
    {
        args.parse(argc, argv);
    } catch (args_t::error_message_t error) {
        std::cerr << error.message();
        return -1;
    }
    
    try
    {
        switch (args.mode())
        {
            
            case args_t::mode_t::FETCH_ITEM_ATTRIBUTES:
            {
                
                // Open output file first so that we can find out if it fails
                // before doing all the work of fetching.
                std::ofstream item_attributes_out_file(args.item_attributes_out());
                if (!item_attributes_out_file.good())
                {
                    std::cerr << "Error.  Failed to open \"" << args.item_attributes_out() << "\" for writing.\n";
                    return -1;
                }
                
                std::cout << "Fetching item ids...\n";
                item_ids_t item_ids;
                item_ids.fetch();
                
                std::cout << "Fetching item attributes\n";
                item_attributes_t item_attributes;
                item_attributes.fetch(item_ids);
                
                std::cout << "Writing output file.\n";
                item_attributes.write_to_file(item_attributes_out_file);
                
                break;
                
            }
            
            case args_t::mode_t::FETCH_PRICES:
            {
                
                // Open and parse input files
                std::ifstream item_attributes_in_file(args.item_attributes_in());
                item_attributes_t item_attributes_in;
                item_attributes_in.read_from_file(item_attributes_in_file);
                
                std::ifstream station_attributes_in_file(args.station_attributes_in());
                station_attributes_t station_attributes_in;
                station_attributes_in.read_from_file(station_attributes_in_file);
                
                std::ofstream prices_out_file(args.prices_out());
                regional_market_orders_t regional_market_orders;
                regional_market_orders.fetch(10000043);
                
                // TODO Conor, you left off here.  Convert regional market to prices
                regional_market_orders.write_to_file(prices_out_file);
                
                break;
                
            }
            
            default:
            {
                std::cerr << "Error.  Unimplemented mode \"" << args_t::enum_to_string(args.mode()) << "\".\n";
                return -1;
            }
            
        }
    } catch (Json::Exception error) {
        std::cerr << error.what() << "  Fatal.  Could not decode JSON input file.\n";
        return -1;
    } catch (args_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (item_ids_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (item_attribute_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (item_attributes_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (order_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (regional_market_orders_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (station_attribute_t::error_message_t error) {
        std::cerr << error;
        return -1;
    } catch (station_attributes_t::error_message_t error) {
        std::cerr << error;
        return -1;
    }
    
    return 0;
    
}

