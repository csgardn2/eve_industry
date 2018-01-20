
----
High Level Description
----

EvE Industry refers to a blueprint profitability calculator and market data fetcher.
The tool is deigned be run via the command line and consumes and outputs data primarily as
JSON files.

Step by step use:

See data/json/schema.json for documentation on writing program inputs.

1.  Fetch the static attributes of all items in the game by calling eve_industry
    in --fetch-item-attributes mode.  You can skip this step and use the provided
    file at data/json/item_attributes.json.  This step only needs to be done once.
2.  Fetch the most recent market data using --fetch-prices mode.  You can fetch
    prices from any NPC station specified by the --station-attributes-in option.
3.  Specify what T1 and T2 blueprints you own by writing a file for --blueprints-in.
    A default file containing most of the blueprints in the game is provided
    in data/json/blueprints_all.json.
4.  Calculate the profit margins of each blueprint you own by running the program
    in --calculate-blueprint-profits mode.  You can re-use the same price file
    for multiple operations to avoid waiting for the data to download.  Your profit
    report will be dumped to --profits-out.  You can read this file directly,
    convert it to a spreadsheet, or use it with your own tools.
    

----
Command line usage:
----
````
./eve_industry
    --mode {fetch-item-attributes, fetch-prices, calculate-blueprint-profits}
    --stations-in FILE.json                                                                 # Specifies the capabilities of a station for manufacture, invention, trade, etc.
    --item-attributes-in FILE.json                                                          # Specifies item IDs used in the EvE API.
    --item-attributes-out FILE.json                                                         # Specifies item IDs used in the EvE API.
    --prices-in FILE.json                                                                   # SPecifies the prices of each item at each station
    --prices-out FILE.json                                                                  # Specifies a location to dump market data fetched from the EvE RESTful API
    --blueprints-in FILE.json                                                               # Specifies input materials, ME, TE, and other attributes of all the blueprints you own or want to own.
    --profits-in FILE.json                                                                  # Specifies the ratio of (input materials)/(output materials) for each blueprint.
    --profits-out FILE.json                                                                 # Generated after a --calculate-blueprint-profitability operation
    --dont-cull-stations                                                                    # If present on the command line, causes market data from all stations in fetched regions to be dumped to --prices-out
    --dont-cull-orders                                                                      # If present on the command line, causes all buy/sell orders other than the higest/lowest respectivly to be dumped
    --quiet                                                                                 # Do not output progress information to the terminal unless an error occurred.
    --decryptor-optimization-strategy {profit-amount, profit-percent, profit-per-second}    # Specifies what profit metric to use when choosing an optimal decryptor
    --output-order {profit-amount, profit-percent, profit-per-second}                       # Specifies how the file specified by --profits-out should be sorted
````

| Mode                        | Required Arguments                                        | Optional Arguments   |
| --------------------------- | --------------------------------------------------------- | -------------------- |
| fetch-item-attributes       | --item-attributes-out                                     | None                 |
| fetch-prices                | --item-attributes-in --station-attributes-in --prices-out | None                 |
| calculate-blueprint-profits | --blueprints-in --prices-in --profits-out                 | --item-attributes-in |

The operation you choose to perform with the --mode argument determines which
arguments are required.  This table below lists the required and optional
command line arguments for each mode.

See @ref args::eve_industry_t::mode_t for additional command line usage.

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

*   Install doxygen
````
sudo apt-get install doxygen
````

File IO Protocols
----
*   The preferred file format of this project is JSON
*   You can validate any JSON file consumed or produced by this script using data/eve_industry_schema.json
*   A good online schema validator can be found <a href="https://json-schema-validator.herokuapp.com/">here</a>

Access/Documentation to the fabulous EvE Swagger API can be found at <a href="https://esi.tech.ccp.is/latest/">here</a>
