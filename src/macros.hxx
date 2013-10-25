/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2005 - 2009 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#ifndef _MACROS_HXX_
#define _MACROS_HXX_

#include <cstdio>

// Conversions
#define A2OU(x) ::rtl::OUString::createFromAscii(x)
#define UTF82OU(x) ::rtl::OStringToOUString(::rtl::OString(x), RTL_TEXTENCODING_UTF8)
#define OU2DEBUG(x) ::rtl::OUStringToOString((x), RTL_TEXTENCODING_UTF8).getStr()

// Debug macros
#ifdef VOIKKO_DEBUG_OUTPUT
#define VOIKKO_DEBUG(x) fprintf(stderr, "VOIKKO_DEBUG: " x "\n")
#define VOIKKO_DEBUG_2(x, y) fprintf(stderr, "VOIKKO_DEBUG: " x "\n", y)
#else
#define VOIKKO_DEBUG(x)
#define VOIKKO_DEBUG_2(x, y)
#endif


#endif
