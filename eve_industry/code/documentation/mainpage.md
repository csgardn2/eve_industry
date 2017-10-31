High Level Workflow
----
@image html high_level_workflow.png

Command line usage:
----
````
./eve_industry
    --mode {fetch-prices, calculate-bluerint-profitability, generate-todo-list}
    --stations-in FILE.json         # Specifies the capabilities of a station for manufacture, invention, trade, etc.
    --items-in FILE.json            # Specifies item IDs used in the EvE API.
    --prices-in FILE.json           # SPecifies the prices of each item at each station
    --prices-out FILE.json          # Specifies a location to dump market data fetched from the EvE RESTful API
    --blueprints-in FILE.json       # Specifies input materials, ME, TE, and other attributes of all the blueprints you own or want to own.
    --assembly-lines-in FILE.json   # Specifies what multistep processes you want to perform such as "First buy materials, then invent, then manufacture, then sell."
    --jobs-in FILE.json             # Specifies what materials you have in-use at your various assembly-lines.
    --jobs-out FILE.json            # After a --generate-todo-list operation, a new file is created representing your production state after completing the todo list.
    --profitability-in FILE.json    # Specifies the ratio of (input materials)/(output materials) for each blueprint.
    --profitability-out FILE.json   # Generated after a --calculate-blueprint-profitability operation
    --todo-list-out FILE.txt        # A human readable list of instructions that you can perform to advance your production
````

The operation you choose to perform with the --mode argument determines which
arguments are required.  This table below lists the required and optional
command line arguments for each mode.

See @ref args_t::mode_t for additional command line usage.

Dependencies
----
*   Install g++ Version 7
````
sudo add-apt-repository ppa:jonathonf/gcc-7.1
sudo apt-get update
sudo apt-get install gcc-7 g++-7
````
    
*   Install libcurl
````
sudo apt-get install libcurl4-openssl-dev
````

File IO Protocols
----
*   The preferred file format of this project is JSON, although this program allows
    some outputs to be exported as CSV.
*   You can validate any JSON file consumed or produced by this script using
    data/eve_industry_schema.json
*   A good online schema validator can be found <a href="https://json-schema-validator.herokuapp.com/">here</a>

Access/Documentation to the fabulous EvE Swagger API can be found at <a href="https://esi.tech.ccp.is/latest/">here</a>
