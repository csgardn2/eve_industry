/// \file
/// File Name:                      fetcher.cpp \n
/// Date created:                   Wed Dec 21 2016 \n
/// Engineers:                      Conor Gardner \n
///                                 Adapted from example by Daniel Stenberg on https://curl.haxx.se/libcurl/c/ftpsget.html
/// Special Compile Instructions:   --std=c++11 -lcurl \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html or fetcher\.h for documentation.

//***************************************************************************
// *                                  _   _ ____  _
// *  Project                     ___| | | |  _ \| |
// *                             / __| | | | |_) | |
// *                            | (__| |_| |  _ <| |___
// *                             \___|\___/|_| \_\_____|
// *
// * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
// *
// * This software is licensed as described in the file COPYING, which
// * you should have received as part of this distribution. The terms
// * are also available at https://curl.haxx.se/docs/copyright.html.
// *
// * You may opt to use, copy, modify, merge, publish, distribute and/or sell
// * copies of the Software, and permit persons to whom the Software is
// * furnished to do so, under the terms of the COPYING file.
// *
// * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
// * KIND, either express or implied.
// *
// ***************************************************************************

#include <curl/curl.h>
#include <iostream>
#include <string>

/// \brief Recieve a burst of data from the network and append it to a more
/// permanent buffer.
static size_t recieve_burst(void* buffer, size_t size, size_t nmemb, void* str)
{
    unsigned num_chars = nmemb * size;
    ((std::string*)(str))->append((char*)(buffer), num_chars);
    return size_t(num_chars);
}

std::string ftps_get(const std::string& url)
{
    
    // This buffer will accumulate all the text recieved from the FTP GET
    // request
    std::string ret;
    
    // Initialize curl library
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a handle and see if initialization was successful
    CURL* curl = curl_easy_init();
    if (curl == NULL)
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        std::cerr << "Error.  Failed to initialize CURL library\n";
        return "";
    }
    
    // Set the URL to request data from
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Define our callback to get called when there's data to be written
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recieve_burst);
    
    // Set a pointer to our struct to pass to the callback
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    
    // We activate SSL and we require it for both control and data
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    
    // Switch on full protocol/debug output
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    
    // Up until now, we have just initialzed the CURL state machine.  Now we
    // initiate the network transaction.
    CURLcode error_code = curl_easy_perform(curl);
    if (error_code != CURLE_OK)
    {
        std::cerr
            << "Error.  Failed to complete HTTPS GET request from URL \""
            << url
            << "\".  Curl returned error code "
            << error_code << '\n';
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return "";
    }
    
    // always cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return ret;
    
}

