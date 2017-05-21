/// \file
/// File Name:                      test_utility.cpp \n
/// Date created:                   Thurs Dec 29 2016 \n
/// Engineers:                      Conor Gardner \n
/// Special Compile Instructions:   --std=c++11 \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html for documentation.

#include <iostream>
#include <string>
#include <vector>
#include "token.h"
#include "utility.h"

/// \brief Validate the \ref strip_comments function, printing diagnostics
/// to the console
/// \return true if validation was successful and false otherwise
bool validate_strip_comments()
{
    
    std::cout << " *** Welcome to the strip_comments() validator ***\n";
    
    static const unsigned num_tests = 30;
    
    static const std::string inputs[num_tests] =
    {
        // Troll tests
        "",
        "//",
        "/*",
        "/**/",
        " ",
        "\"",
        "\"\"",
        "//\n",
        "//\n\n",
        "//poop\n",
        "poop = /* stripped // */ kept",
        "poop =    kept // */ stripped",
        
        // NORMAL -> NORMAL
        "Nothing should\nbe stripped\n",
        
        // NORMAL -> BEGIN_COMMENT -> NORMAL
        "/Almost / A / Comment /",
        "/ Multi\nLine /\nTrolling",
        
        // NORMAL -> BEGIN_COMMENT -> SINGLE_COMMENT
        "Fudge // remove this",
        "// Remove it all",
        "Don't remove much //",
        "Flab //\njiggling // merrily\nIn the breeze",
        "Evil //\\\ngah\nmultiline\ngoonies // lard\\\nfuzz\nboogers",
        
        // NORMAL -> BEGIN_COMMENT -> MULTI_COMMENT -> BEGIN_MULTI_CLOSE
        "Nuggets /* are not */ tasty\nSalad is /*\namazingly */ stinky",
        
        // NORMAL -> STRING
        "Fuzz",
        "Toilet \"paper\"",
        "\"Noob // /* */ cannon\" mount",
        "// \"Bruiser Cruiser\"\nFrigate\"Frizzer\"",
        "\"Lots\" of \"strings\" in \"a\" row \"\"\"\"",
        
        // STRING -> NORMAL
        "\"\" \"\"\"\" \"donuts\"\n\"fat\"",
        
        // STRING -> ESCAPE_CHAR
        "\"\\b is for bold and \\0 is for null we end in a newline \\n\"",
        
        // BACKSLASH_COMMENT
        "// Extended \\\nComment",
        "// Don't extend \\ \nDoesn't get commented"
    };
    
    static const std::string answers[num_tests] = 
    {
        "",
        "",
        "",
        "",
        " ",
        "\"",
        "\"\"",
        "",
        "\n",
        "",
        "poop =  kept",
        "poop =    kept ",
        
        // NORMAL -> NORMAL
        "Nothing should\nbe stripped\n",
        
        // NORMAL -> BEGIN_COMMENT -> NORMAL
        "/Almost / A / Comment /",
        "/ Multi\nLine /\nTrolling",
        
        // NORMAL -> BEGIN_COMMENT -> SINGLE_COMMENT
        "Fudge ",
        "",
        "Don't remove much ",
        "Flab jiggling In the breeze",
        "Evil multiline\ngoonies boogers",
        
        // NORMAL -> BEGIN_COMMENT -> MULTI_COMMENT -> BEGIN_MULTI_CLOSE
        "Nuggets  tasty\nSalad is  stinky",
        
        // NORMAL -> STRING
        "Fuzz",
        "Toilet \"paper\"",
        "\"Noob // /* */ cannon\" mount",
        "Frigate\"Frizzer\"",
        "\"Lots\" of \"strings\" in \"a\" row \"\"\"\"",
        
        // STRING -> NORMAL
        "\"\" \"\"\"\" \"donuts\"\n\"fat\"",
        
        // STRING -> ESCAPE_CHAR
        "\"\\b is for bold and \\0 is for null we end in a newline \\n\"",
        
        // BACKSLASH_COMMENT
        "",
        "Doesn't get commented"
    };
    
    bool global_success = true;
    for (unsigned ix = 0; ix < num_tests; ix++)
    {
        std::string ret = strip_comments(inputs[ix]);
        if (ret != answers[ix])
        {
            global_success = false;
            std::cout
                << "     Test "
                << ix
                << " failed.  input \""
                << inputs[ix]
                << "\" resulted in \""
                << ret
                << "\" but should have been \""
                << answers[ix]
                << "\"\n";
        }
    }
    
    if (global_success)
        std::cout << "     strip_comments() validated successfully\n";
    else
        std::cout << "     strip_comments() fails at least one test case\n";
    
    return global_success;
    
}

/// \brief Validate the \ref strip_whitespace function, printing diagnostics
/// to the console
/// \return true if validation was successful and false otherwise
bool validate_strip_whitespace()
{
    
    std::cout << " *** Welcome to the strip_comments() validator ***\n";
    
    static const unsigned num_tests = 12;
    
    static const std::string inputs[num_tests] =
    {
        
        // Troll tests
        "",
        " ",
        "\"",
        "\"\"",
        "\n",
        "\n\n",
        
        // NORMAL -> NORMAL
        "guineapigbridge",
        " balls to the    walls ",
        "fudge\nballs"
        
        // NORMAL -> STRING
        // STRING -> NORMAL
        "stuff \"with constants\tin\" it",
        "\"\" multiple \"string\" constants \"are \" here",
        
        // STRING -> ESCAPE_CHAR
        "strings \"can \\n\\t sometimes \\b\" have \nannoyances"
        
    };
    
    static const std::string answers[num_tests] = 
    {
        "",
        "",
        "\"",
        "\"\"",
        "",
        "",
        "guineapigbridge",
        "ballstothewalls",
        "fudgeballs"
        "stuff\"with constants\tin\"it",
        "\"\"multiple\"string\"constants\"are \"here",
        "strings\"can \\n\\t sometimes \\b\"haveannoyances"
    };
    
    bool global_success = true;
    for (unsigned ix = 0; ix < num_tests; ix++)
    {
        std::string ret = strip_whitespace(inputs[ix]);
        if (ret != answers[ix])
        {
            global_success = false;
            std::cout
                << "     Test "
                << ix
                << " failed.  input \""
                << inputs[ix]
                << "\" resulted in \""
                << ret
                << "\" but should have been \""
                << answers[ix]
                << "\"\n";
        }
    }
    
    if (global_success)
        std::cout << "     strip_whitespace() validated successfully\n";
    else
        std::cout << "     strip_whitespace() fails at least one test case\n";
    
    return global_success;
    
};

// Test coverage is rather crappy.  It doesn't really test any trollish
// cases like invalid numeric constants.  It it only designed to cover every
// line of code (every case) in tokenize() at least once.
bool validate_tokenize()
{
    
    std::cout << " *** Welcome to the strip_comments() validator ***\n";
    
    static const std::string input_json =
        "{\n"
        "    \"bool 1\": true,\n"
        "    \"bool 2\" :false,\n"
        "    \"int 3\": 0,\n"
        "    \"int 4\": 9,\n"
        "    \"int 5\": 42,\n"
        "    \"int 6\": 12345,\n"
        "    \"int 7\": -1,\n"
        "    \"int 8\": -69,\n"
        "    \"int 9\": -24680,\n"
        "    \"float 10\": 0.3,\n"
        "    \"float 11\": 10.54,\n"
        "    \"float 12\": 13.33,\n"
        "    \"float 13\": 2.99e+6,\n"
        "    \"float 14\": 3.14e10,\n"
        "    \"float 15\": 411.12E-3,\n"
        "    \"float 16\": -0.6,\n"
        "    \"float 17\": -76.24,\n"
        "    \"float 18\": -24.01,\n"
        "    \"float 19\": -4.18E+2,\n"
        "    \"float 20\": -2.55e25,\n"
        "    \"float 21\": -71.03e-3,\n"
        "    \"string 22\": \"\",\n"
        "    \"string 23\": \"lardio\",\n"
        "    \"string 24\": \"flab's magic\\t\\\"carpet\\\"\",\n"
        "    \"struct 25\": {},\n"
        "    \"struct 26\":\n"
        "    {\n"
        "        \"bool 27\": true,\n"
        "        \"int 28\": 64,\n"
        "        \"float 29\": 7.8,\n"
        "        \"string 30\": \"booze cruise\"\n"
        "    },\n"
        "    \"array 31\": [],\n"
        "    \"array 32\":\n"
        "    [\n"
        "        0.5,\n"
        "        4.1274,\n"
        "        8.88\n"
        "    ]\n"
        "}";
    
    static const unsigned num_answers = 137;
    static const json_token_t gold_tokens[num_answers] = 
    {
        {.token_type = token_type_t::OPEN_CURLY, .line = 1, .value = ""},
        {.token_type = token_type_t::STRING, .line = 2, .value = "bool 1"},
        {.token_type = token_type_t::COLON, .line = 2, .value = ""},
        {.token_type = token_type_t::BOOL_TRUE, .line = 2, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 2, .value = ""},
        {.token_type = token_type_t::STRING, .line = 3, .value = "bool 2"},
        {.token_type = token_type_t::COLON, .line = 3, .value = ""},
        {.token_type = token_type_t::BOOL_FALSE, .line = 3, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 3, .value = ""},
        {.token_type = token_type_t::STRING, .line = 4, .value = "int 3"},
        {.token_type = token_type_t::COLON, .line = 4, .value = ""},
        {.token_type = token_type_t::INT, .line = 4, .value = "0"},
        {.token_type = token_type_t::COMMA, .line = 4, .value = ""},
        {.token_type = token_type_t::STRING, .line = 5, .value = "int 4"},
        {.token_type = token_type_t::COLON, .line = 5, .value = ""},
        {.token_type = token_type_t::INT, .line = 5, .value = "9"},
        {.token_type = token_type_t::COMMA, .line = 5, .value = ""},
        {.token_type = token_type_t::STRING, .line = 6, .value = "int 5"},
        {.token_type = token_type_t::COLON, .line = 6, .value = ""},
        {.token_type = token_type_t::INT, .line = 6, .value = "42"},
        {.token_type = token_type_t::COMMA, .line = 6, .value = ""},
        {.token_type = token_type_t::STRING, .line = 7, .value = "int 6"},
        {.token_type = token_type_t::COLON, .line = 7, .value = ""},
        {.token_type = token_type_t::INT, .line = 7, .value = "12345"},
        {.token_type = token_type_t::COMMA, .line = 7, .value = ""},
        {.token_type = token_type_t::STRING, .line = 8, .value = "int 7"},
        {.token_type = token_type_t::COLON, .line = 8, .value = ""},
        {.token_type = token_type_t::INT, .line = 8, .value = "-1"},
        {.token_type = token_type_t::COMMA, .line = 8, .value = ""},
        {.token_type = token_type_t::STRING, .line = 9, .value = "int 8"},
        {.token_type = token_type_t::COLON, .line = 9, .value = ""},
        {.token_type = token_type_t::INT, .line = 9, .value = "-69"},
        {.token_type = token_type_t::COMMA, .line = 9, .value = ""},
        {.token_type = token_type_t::STRING, .line = 10, .value = "int 9"},
        {.token_type = token_type_t::COLON, .line = 10, .value = ""},
        {.token_type = token_type_t::INT, .line = 10, .value = "-24680"},
        {.token_type = token_type_t::COMMA, .line = 10, .value = ""},
        {.token_type = token_type_t::STRING, .line = 11, .value = "float 10"},
        {.token_type = token_type_t::COLON, .line = 11, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 11, .value = "0.3"},
        {.token_type = token_type_t::COMMA, .line = 11, .value = ""},
        {.token_type = token_type_t::STRING, .line = 12, .value = "float 11"},
        {.token_type = token_type_t::COLON, .line = 12, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 12, .value = "10.54"},
        {.token_type = token_type_t::COMMA, .line = 12, .value = ""},
        {.token_type = token_type_t::STRING, .line = 13, .value = "float 12"},
        {.token_type = token_type_t::COLON, .line = 13, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 13, .value = "13.33"},
        {.token_type = token_type_t::COMMA, .line = 13, .value = ""},
        {.token_type = token_type_t::STRING, .line = 14, .value = "float 13"},
        {.token_type = token_type_t::COLON, .line = 14, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 14, .value = "2.99e+6"},
        {.token_type = token_type_t::COMMA, .line = 14, .value = ""},
        {.token_type = token_type_t::STRING, .line = 15, .value = "float 14"},
        {.token_type = token_type_t::COLON, .line = 15, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 15, .value = "3.14e10"},
        {.token_type = token_type_t::COMMA, .line = 15, .value = ""},
        {.token_type = token_type_t::STRING, .line = 16, .value = "float 15"},
        {.token_type = token_type_t::COLON, .line = 16, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 16, .value = "411.12E-3"},
        {.token_type = token_type_t::COMMA, .line = 16, .value = ""},
        {.token_type = token_type_t::STRING, .line = 17, .value = "float 16"},
        {.token_type = token_type_t::COLON, .line = 17, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 17, .value = "-0.6"},
        {.token_type = token_type_t::COMMA, .line = 17, .value = ""},
        {.token_type = token_type_t::STRING, .line = 18, .value = "float 17"},
        {.token_type = token_type_t::COLON, .line = 18, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 18, .value = "-76.24"},
        {.token_type = token_type_t::COMMA, .line = 18, .value = ""},
        {.token_type = token_type_t::STRING, .line = 19, .value = "float 18"},
        {.token_type = token_type_t::COLON, .line = 19, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 19, .value = "-24.01"},
        {.token_type = token_type_t::COMMA, .line = 19, .value = ""},
        {.token_type = token_type_t::STRING, .line = 20, .value = "float 19"},
        {.token_type = token_type_t::COLON, .line = 20, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 20, .value = "-4.18E+2"},
        {.token_type = token_type_t::COMMA, .line = 20, .value = ""},
        {.token_type = token_type_t::STRING, .line = 21, .value = "float 20"},
        {.token_type = token_type_t::COLON, .line = 21, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 21, .value = "-2.55e25"},
        {.token_type = token_type_t::COMMA, .line = 21, .value = ""},
        {.token_type = token_type_t::STRING, .line = 22, .value = "float 21"},
        {.token_type = token_type_t::COLON, .line = 22, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 22, .value = "-71.03e-3"},
        {.token_type = token_type_t::COMMA, .line = 22, .value = ""},
        {.token_type = token_type_t::STRING, .line = 23, .value = "string 22"},
        {.token_type = token_type_t::COLON, .line = 23, .value = ""},
        {.token_type = token_type_t::STRING, .line = 23, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 23, .value = ""},
        {.token_type = token_type_t::STRING, .line = 24, .value = "string 23"},
        {.token_type = token_type_t::COLON, .line = 24, .value = ""},
        {.token_type = token_type_t::STRING, .line = 24, .value = "lardio"},
        {.token_type = token_type_t::COMMA, .line = 24, .value = ""},
        {.token_type = token_type_t::STRING, .line = 25, .value = "string 24"},
        {.token_type = token_type_t::COLON, .line = 25, .value = ""},
        {.token_type = token_type_t::STRING, .line = 25, .value = "flab's magic\\t\\\"carpet\\\""},
        {.token_type = token_type_t::COMMA, .line = 25, .value = ""},
        {.token_type = token_type_t::STRING, .line = 26, .value = "struct 25"},
        {.token_type = token_type_t::COLON, .line = 26, .value = ""},
        {.token_type = token_type_t::OPEN_CURLY, .line = 26, .value = ""},
        {.token_type = token_type_t::CLOSE_CURLY, .line = 26, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 26, .value = ""},
        {.token_type = token_type_t::STRING, .line = 27, .value = "struct 26"},
        {.token_type = token_type_t::COLON, .line = 27, .value = ""},
        {.token_type = token_type_t::OPEN_CURLY, .line = 28, .value = ""},
        {.token_type = token_type_t::STRING, .line = 29, .value = "bool 27"},
        {.token_type = token_type_t::COLON, .line = 29, .value = ""},
        {.token_type = token_type_t::BOOL_TRUE, .line = 29, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 29, .value = ""},
        {.token_type = token_type_t::STRING, .line = 30, .value = "int 28"},
        {.token_type = token_type_t::COLON, .line = 30, .value = ""},
        {.token_type = token_type_t::INT, .line = 30, .value = "64"},
        {.token_type = token_type_t::COMMA, .line = 30, .value = ""},
        {.token_type = token_type_t::STRING, .line = 31, .value = "float 29"},
        {.token_type = token_type_t::COLON, .line = 31, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 31, .value = "7.8"},
        {.token_type = token_type_t::COMMA, .line = 31, .value = ""},
        {.token_type = token_type_t::STRING, .line = 32, .value = "string 30"},
        {.token_type = token_type_t::COLON, .line = 32, .value = ""},
        {.token_type = token_type_t::STRING, .line = 32, .value = "booze cruise"},
        {.token_type = token_type_t::CLOSE_CURLY, .line = 33, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 33, .value = ""},
        {.token_type = token_type_t::STRING, .line = 34, .value = "array 31"},
        {.token_type = token_type_t::COLON, .line = 34, .value = ""},
        {.token_type = token_type_t::OPEN_SQUARE, .line = 34, .value = ""},
        {.token_type = token_type_t::CLOSE_SQUARE, .line = 34, .value = ""},
        {.token_type = token_type_t::COMMA, .line = 34, .value = ""},
        {.token_type = token_type_t::STRING, .line = 35, .value = "array 32"},
        {.token_type = token_type_t::COLON, .line = 35, .value = ""},
        {.token_type = token_type_t::OPEN_SQUARE, .line = 36, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 37, .value = "0.5"},
        {.token_type = token_type_t::COMMA, .line = 37, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 38, .value = "4.1274"},
        {.token_type = token_type_t::COMMA, .line = 38, .value = ""},
        {.token_type = token_type_t::FLOAT, .line = 39, .value = "8.88"},
        {.token_type = token_type_t::CLOSE_SQUARE, .line = 40, .value = ""},
        {.token_type = token_type_t::CLOSE_CURLY, .line = 41, .value = ""}
    };
    
    std::vector<json_token_t> dut_tokens = tokenize(input_json);
    
    bool global_success = true;
    for (unsigned ix = 0, bound = min(num_answers, dut_tokens.size()); ix < bound; ix++)
    {
        if (gold_tokens[ix] != dut_tokens[ix])
        {
            global_success = false;
            std::cout
                << "     Error.  Token "
                << ix
                << " mismatch.  Parser produced "
                << dut_tokens[ix].print()
                << " but should have been "
                << gold_tokens[ix].print()
                << '\n';
        }
    }
    
    if (global_success)
        std::cout << "     tokenize() validated successfully\n";
    else
        std::cout << "     tokenize() fails at least one test case\n";
    
    return global_success;
    
}

/// \brief Validate the functions in utility.cpp.  This function will print some
/// diagnostics to the screen but the human is also responsible for verifying
/// that no memory leaks or crashes occur.
int main()
{
    
    validate_strip_comments();
    validate_strip_whitespace();
    validate_tokenize();
    
    return 0;
    
}

