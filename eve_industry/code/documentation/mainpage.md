High Level Workflow
----
@image html high_level_workflow.png

Command line usage:
----
````
./eve_industry
    --mode {fetch-item-attributes, fetch-station-attributes}
    --item-attributes-in FILE.json              # Specifies item IDs used in the EvE API.
    --item-attributes-out FILE>json             # Specifies item IDs used in the EvE API.
    --station-attributes-in FILE.json           # Specifies the capabilities of a station for manufacture, invention, trade, etc.
    --station-attributes-out FILE.json          # Specifies the capabilities of a station for manufacture, invention, trade, etc.
````

The operation you choose to perform with the --mode argument determines which
arguments are required.  This table below lists the required and optional
command line arguments for each mode.

<table>
    <tr>
        <th>Mode</th>
        <th>Required Additional Arguments</th>
        <th>Optional Additional Arguments</th>
    </tr>
    <tr>
        <td>--fetch-item-attributes</td>
        <td>--item-attributes-out</td>
        <td>None</td>
    </tr>
    <tr>
        <td>--fetch-station-attributes</td>
        <td>--station-attributes-out</td>
        <td>None</td>
    </tr>
</table>

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
