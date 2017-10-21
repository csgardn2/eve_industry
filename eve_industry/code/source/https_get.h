/// \file
/// File Name:                      fetcher.h \n
/// Date created:                   Wed Dec 21 2016 \n
/// Engineers:                      Conor Gardner \n
///                                 Adapted from example by Daniel Stenberg on https://curl.haxx.se/libcurl/c/ftpsget.html
/// Special Compile Instructions:   --std=c++11 -lcurl \n
/// Compiler:                       g++ \n
/// Target OS:                      Ubuntu Linux 16.04 \n
/// Target architecture:            x86 64-bit \n
/// See html/index.html for documentation.
 
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

#ifndef HEADER_GUARD_HTTPS_GET
#define HEADER_GUARD_HTTPS_GET

#include <string>

/// \brief Fetches the raw webpage or file at the given URL and packages the
/// results in the returned string.
std::string https_get(const std::string& url);

#endif // header guard

