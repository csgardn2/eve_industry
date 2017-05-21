/// \file
/// File Name:                      calculate_blueprint_profitablity.h \n
/// Date created:                   Wed Dec 21 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 14.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_CALCULATE_BLUEPRINT_PROFITABILITY
#define HEADER_GUARD_CALCULATE_BLUEPRINT_PROFITABILITY

#include <string>
#include <vector>

#include "blueprint.h"
#include "calculate_blueprint_profitability.h"
#include "fetch_item_ids.h"
#include "fetch_orders.h"

/// \brief Reports how much ISK will be gained or lost by manufacturing a
/// certain item.
class profitability_t
{
    
    public:
        
        /// \brief Defauly constructor
        profitability_t() = default;
        
        /// \brief Initialization constructor
        profitability_t
        (
            const std::string& name_,
            float profit_per_run_,
            float isk_per_second_,
            float runs_per_cycle_,
            float break_even_price_
            
        )
            : name(name_),
              profit_per_run(profit_per_run_),
              isk_per_second(isk_per_second_),
              runs_per_cycle(runs_per_cycle_),
              break_even_price(break_even_price_)
        {
            // Intentionally left blank
        }
        
        /// \brief The name of an item to manufacture.  See \ref blueprint_t
        std::string name;
        
        /// \brief This value is 1.0 if the output products are worth exactly
        /// the same as the input materials, 2.0 if the outputs are worth twice
        /// as much, 0.5 if the outputs are worth half as much ect.  This is a
        /// unitless quantity.
        float profit_per_run;
        
        /// \brief The absolute amount of profit made per second as a
        /// manufacturing job is run.  In other words:
        /// (output value - input value) / (manufacturing time)
        float isk_per_second;
        
        /// \brief The number of job runs that can be fit into a single cycle
        /// time.
        float runs_per_cycle;
        
        /// \brief The cost of the input materials
        float break_even_price;
        
};

/// \brief Create a \ref profitability_t report for each blueprint you own.
std::vector<profitability_t> calculate_blueprint_profitability
(
    const std::vector<item_id_pair_t>& item_ids,
    const std::vector<order_t>& orders,
    const std::vector<blueprint_t>& blueprints,
    float cycle_time
);

#endif // header guard

