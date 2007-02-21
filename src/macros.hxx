/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2005 - 2007 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *********************************************************************************/

#ifndef _MACROS_HXX_
#define _MACROS_HXX_

#include <cstdio>

// Conversions
#define A2OU(x) ::rtl::OUString::createFromAscii(x)
#define OU2DEBUG(x) ::rtl::OUStringToOString((x), RTL_TEXTENCODING_UTF8).getStr()

// Debug macros
#ifdef VOIKKO_DEBUG_OUTPUT
#define VOIKKO_DEBUG(x) printf("VOIKKO_DEBUG: " x "\n")
#define VOIKKO_DEBUG_2(x, y) printf("VOIKKO_DEBUG: " x "\n", y)
#else
#define VOIKKO_DEBUG(x)
#define VOIKKO_DEBUG_2(x, y)
#endif


#endif
