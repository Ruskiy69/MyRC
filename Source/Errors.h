/*****************************************************
*                                                    *
*                     Errors.h                       *
*                                                    *
*   A simple Windows-only function to display a      *
*   window in case of any error messages.            *
*                                                    *
*   FUNCTIONS:                                       *
*                                                    *
*       - handleError(errorMessage, quit = true)     *
*           Taking a C_string as a parameter, this   *
*           will create a simple window with your    *
*           message displayed. It uses the Windows   *
*           API, meaning if any external API fails   *
*           to initialize, this will natively handle *
*           the error.                               *
*           Once the user clicks "Okay", the         *
*           program terminates, unless quit is set   *
*            to false.                               *
*                                                    *
*****************************************************/
#pragma once

#ifdef _WIN32
#ifndef WS2_INCLUDED
#include <Windows.h>
#endif // WS2_INCLUDED
#else
#include <cstdlib>
#include <cstdio>
#endif // _WIN32

void handleError(const char* errorMessage, const bool quit = true);
