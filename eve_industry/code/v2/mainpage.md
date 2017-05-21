Welcome to the "EvE Industry" documentation!  This program is designed to fetch
the most up-to-date market data from the EvE online universe using the official
CREST API as well as a user specified list of blueprints to aid in deciding
which items are profitable to manufacture.

For command line usage, see \ref arguments_t

Note that not all functions (such as \ref string_to_region) nor enums (such as
\ref region_t) are members of a class, so you will need to follow a link or use
Files > File Members > Functions.  This is a style choice - I chose to build
classes that are general-purpose containers that are passed to and returned from
C-style functions to emphasize the data-flow nature of this program's workload.

I will fill in a high-level block diagram of the code architecture once most of
it is written and stabilized.
