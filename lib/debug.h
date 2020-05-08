#pragma once

// Flag para activar los mensajes de debug
#define SHOW_DEBUG_INFO_0
#define SHOW_DEBUG_INFO_1

#if defined(SHOW_DEBUG_INFO_0) || defined(SHOW_DEBUG_INFO_1)
#   include <iostream>
#   include <string>
    using std::string;
#endif

/**
 * Mensaje de debug (solo actua si se encuentra definido el flag SHOW_DEBUG_INFO_0
 */
template<typename T>
void DEBUG_0(T t, const char* label = "", bool showHeader = true, bool newline = true)
{
#ifdef SHOW_DEBUG_INFO_0
    if (showHeader) std::cout << "DEBUG_0: ";
    std::cout << label;
    std::cout << t;
    if (newline) std::cout << std::endl;
#endif
}

template<typename T>
void DEBUG_1(T t, const char* label = "", bool showHeader = true, bool newline = true)
{
#ifdef SHOW_DEBUG_INFO_1
    if (showHeader) std::cout << "DEBUG_1: ";
    std::cout << label;
    std::cout << t;
    if (newline) std::cout << std::endl;
#endif
}

