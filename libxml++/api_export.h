/* document.h
 * this file is part of libxml++
 *
 * parts of the code copyright (C) 2003 by Stefan Seefeld
 * others copyright (C) 2003 by libxml++ developer's team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

/*
 * This file define some macros which permit to construct a dll with mingw32.
 * It is largely inspired from a part of the file sigcconfig.h.in of the
 * libsigc++ project. The adaptation for other compilers such as borland or
 * msvc should be quite easy and concern only this file.
 */

#ifndef __LIBXMLPP_API_EXPORT_H
#define __LIBXMLPP_API_EXPORT_H

#ifdef __MINGW32__
 #define LIBXMLPP_DLL
#endif // __MINGW32__

#ifdef LIBXMLPP_DLL
 #if defined(LIBXMLPP_COMPILATION) && defined(DLL_EXPORT)
  #define LIBXMLPP_API __declspec(dllexport)
 #elif !defined(LIBXMLPP_COMPILATION)
  #define LIBXMLPP_API __declspec(dllimport)
 #else
  #define LIBXMLPP_API
 #endif /* LIBXMLPP_COMPILATION - DLL_EXPORT */
#else
 #define LIBXMLPP_API
#endif // LIBXMLPP_DLL

#endif
