/// \file
/// File Name:                      regions.cpp \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html or regions.h for documentation.

#include <cctype>
#include <cstring>
#include <string>
#include <vector>

#include "region.h"
#include "utility.h"

/// The number of regions in the EvE galaxy + wormhole space.
static const unsigned num_regions = 100;

std::vector<region_t> string_to_region(const char* str)
{
    
    static const char* string_list[num_regions] =
    {
        "A821A",
        "AR00001",
        "AR00002",
        "AR00003",
        "ARIDIA",
        "BLACKRISE",
        "BR00004",
        "BR00005",
        "BR00006",
        "BR00007",
        "BR00008",
        "BRANCH",
        "CACHE",
        "CATCH",
        "CLOUDRING",
        "COBALTEDGE",
        "CR00009",
        "CR00010",
        "CR00011",
        "CR00012",
        "CR00013",
        "CR00014",
        "CR00015",
        "CURSE",
        "DEKLEIN",
        "DELVE",
        "DERELIK",
        "DETORID",
        "DEVOID",
        "DOMAIN",
        "DR00016",
        "DR00017",
        "DR00018",
        "DR00019",
        "DR00020",
        "DR00021",
        "DR00022",
        "DR00023",
        "ER00024",
        "ER00025",
        "ER00026",
        "ER00027",
        "ER00028",
        "ER00029",
        "ESOTERIA",
        "ESSENCE",
        "ETHERIUMREACH",
        "EVERYSHORE",
        "FADE",
        "FEYTHABOLIS",
        "FOUNTAIN",
        "FR00030",
        "GEMINATE",
        "GENESIS",
        "GR00031",
        "GREATWILDLANDS",
        "HEIMATAR",
        "HR00032",
        "IMMENSEA",
        "IMPASS",
        "INSMOTHER",
        "J7HZF",
        "KADOR",
        "KHANID",
        "KORAZOR",
        "KR00033",
        "LONETREK",
        "MALPAIS",
        "METROPOLIS",
        "MOLDENHEATH",
        "OASA",
        "OMIST",
        "OUTERPASSAGE",
        "OUTERRING",
        "PARAGONSOUL",
        "PERIODBASIS",
        "PERRIGENFALLS",
        "PLACID",
        "PROVIDENCE",
        "PUREBLIND",
        "QUERIOUS",
        "SCALDINGPASS",
        "SINQLAISON",
        "SOLITUDE",
        "STAIN",
        "SYNDICATE",
        "TASHMURKON",
        "TENAL",
        "TENERIFIS",
        "THEBLEAKLANDS",
        "THECITADEL",
        "THEFORGE",
        "THEKALEVALAEXPANSE",
        "THESPIRE",
        "TRIBUTE",
        "UUAF4",
        "VALEOFTHESILENT",
        "VENAL",
        "VERGEVENDOR",
        "WICKEDCREEK"
    };
    
    // Strip nonalphanumeric characters from input string
    std::string stripped_string = strip_and_upper(str);
    
    std::vector<region_t> ret;
    for (unsigned ix = 0; ix < num_regions; ix++)
    {
        if (strstr(string_list[ix], stripped_string.data()) != NULL)
            ret.push_back(region_t(ix));
    }
    
    return ret;
    
}

const char* region_to_string(region_t region)
{
    
    static const char* string_list[num_regions] = 
    {
        "A821-A",
        "A-R00001",
        "A-R00002",
        "A-R00003",
        "Aridia",
        "Black Rise",
        "B-R00004",
        "B-R00005",
        "B-R00006",
        "B-R00007",
        "B-R00008",
        "Branch",
        "Cache",
        "Catch",
        "Cloud Ring",
        "Cobalt Edge",
        "C-R00009",
        "C-R00010",
        "C-R00011",
        "C-R00012",
        "C-R00013",
        "C-R00014",
        "C-R00015",
        "Curse",
        "Deklein",
        "Delve",
        "Derelik",
        "Detorid",
        "Devoid",
        "Domain",
        "D-R00016",
        "D-R00017",
        "D-R00018",
        "D-R00019",
        "D-R00020",
        "D-R00021",
        "D-R00022",
        "D-R00023",
        "E-R00024",
        "E-R00025",
        "E-R00026",
        "E-R00027",
        "E-R00028",
        "E-R00029",
        "Esoteria",
        "Essence",
        "Etherium Reach",
        "Everyshore",
        "Fade",
        "Feythabolis",
        "Fountain",
        "F-R00030",
        "Geminate",
        "Genesis",
        "G-R00031",
        "Great Wildlands",
        "Heimatar",
        "H-R00032",
        "Immensea",
        "Impass",
        "Insmother",
        "J7HZ-F",
        "Kador",
        "Khanid",
        "Kor-Azor",
        "K-R00033",
        "Lonetrek",
        "Malpais",
        "Metropolis",
        "Molden Heath",
        "Oasa",
        "Omist",
        "Outer Passage",
        "Outer Ring",
        "Paragon Soul",
        "Period Basis",
        "Perrigen Falls",
        "Placid",
        "Providence",
        "Pure Blind",
        "Querious",
        "Scalding Pass",
        "Sinq Laison",
        "Solitude",
        "Stain",
        "Syndicate",
        "Tash-Murkon",
        "Tenal",
        "Tenerifis",
        "The Bleak Lands",
        "The Citadel",
        "The Forge",
        "The Kalevala Expanse",
        "The Spire",
        "Tribute",
        "UUA-F4",
        "Vale of the Silent",
        "Venal",
        "Verge Vendor",
        "Wicked Creek"
    };
    
    unsigned region_index = unsigned(region);
    if (region_index < num_regions)
        return string_list[region_index];
    else
        return NULL;
    
}


