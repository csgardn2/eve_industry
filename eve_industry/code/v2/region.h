/// \file
/// File Name:                      regions.h \n
/// Date created:                   Sun Dec 18 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n */
/// See html/index.html for documentation.
 
#ifndef HEADER_GUARD_REGIONS
#define HEADER_GUARD_REGIONS

#include <vector>

/// Represents every region in the EvE galaxy
enum class region_t
{
    A821A,
    AR00001,
    AR00002,
    AR00003,
    ARIDIA,
    BLACKRISE,
    BR00004,
    BR00005,
    BR00006,
    BR00007,
    BR00008,
    BRANCH,
    CACHE,
    CATCH,
    CLOUDRING,
    COBALTEDGE,
    CR00009,
    CR00010,
    CR00011,
    CR00012,
    CR00013,
    CR00014,
    CR00015,
    CURSE,
    DEKLEIN,
    DELVE,
    DERELIK,
    DETORID,
    DEVOID,
    DOMAIN,
    DR00016,
    DR00017,
    DR00018,
    DR00019,
    DR00020,
    DR00021,
    DR00022,
    DR00023,
    ER00024,
    ER00025,
    ER00026,
    ER00027,
    ER00028,
    ER00029,
    ESOTERIA,
    ESSENCE,
    ETHERIUMREACH,
    EVERYSHORE,
    FADE,
    FEYTHABOLIS,
    FOUNTAIN,
    FR00030,
    GEMINATE,
    GENESIS,
    GR00031,
    GREATWILDLANDS,
    HEIMATAR,
    HR00032,
    IMMENSEA,
    IMPASS,
    INSMOTHER,
    J7HZF,
    KADOR,
    KHANID,
    KORAZOR,
    KR00033,
    LONETREK,
    MALPAIS,
    METROPOLIS,
    MOLDENHEATH,
    OASA,
    OMIST,
    OUTERPASSAGE,
    OUTERRING,
    PARAGONSOUL,
    PERIODBASIS,
    PERRIGENFALLS,
    PLACID,
    PROVIDENCE,
    PUREBLIND,
    QUERIOUS,
    SCALDINGPASS,
    SINQLAISON,
    SOLITUDE,
    STAIN,
    SYNDICATE,
    TASHMURKON,
    TENAL,
    TENERIFIS,
    THEBLEAKLANDS,
    THECITADEL,
    THEFORGE,
    THEKALEVALAEXPANSE,
    THESPIRE,
    TRIBUTE,
    UUAF4,
    VALEOFTHESILENT,
    VENAL,
    VERGEVENDOR,
    WICKEDCREEK,
    UNKNOWN
};

/// \brief Performs a lookup that returns a station ID given the name of the
/// station as a string.  This function is not case sensitive and ignores
/// nonalphanumeric characters.  Additionally, as station names can be quite
/// long, you may pass any substring of the station name to get all partial
/// matches.
/// Number | Situation | Example Input str | Example Return Value
/// :----: | :-----------------------------------------------------------------------------------------------------: | :-----------------: | :-------------------------------------------------------------------------------------:
/// 1      | If the str parameter does not match any regions, you will get an empty vector.  This is the ideal case. | "Equestria"         | \<Empty Vector\>
/// 2      | If the str parameter uniquly identifies a region, you will get exactly one answer.                      | "Domain" or "^m Ai" | \ref region_t "DOMAIN"
/// 3      | If the str parameter matches multiple regions, you will get a list of all the matching regions.         | "OLI"               | \ref region_t "FEYTHABOLIS" and \ref region_t "METROPOLIS" and \ref region_t "SOLITUDE"
std::vector<region_t> string_to_region(const char* str);

/// \brief Performs a lookup that returns a pretty human-readable string
/// corresponding to that region.
/// Note that region != region_to_string(string_to_region(region)) due to
/// the case insensitivity and special character removal done in \ref
/// string_to_region.  If a match is not found, then NULL is returned.
const char* region_to_string(region_t region);

#endif // header guard

