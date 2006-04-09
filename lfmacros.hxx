/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@iki.fi), 2005
 *
 *  This file contains macros and definitions from lingucomponent-fi 0.1.2
 *  (files source/spell/sspellimp.cxx and source/spell/sspellimp.hxx) and
 *  OOo_1.9.104 (files tools/inc/solar.h, svtools/inc/linguprops.hxx,
 *  linguistic/inc/misc.hxx and svtools/inc/lngmisc.hxx).
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301, USA
 *
 ************************************************************************/

#ifndef _LFMACROS_HXX_
#define _LFMACROS_HXX_



#include <cstdio>
#include <osl/mutex.hxx>
#include <rtl/string.hxx>
#include <rtl/ustring.hxx>

#include <com/sun/star/lang/Locale.hpp>


using namespace ::com::sun::star::lang;
using namespace ::rtl;

// conversions
#define A2OU(x)	::rtl::OUString::createFromAscii( x )
#define OU2ISO_1(rtlOUString)     ::rtl::OString((rtlOUString).getStr(), (rtlOUString).getLength(), RTL_TEXTENCODING_ISO_8859_1).getStr()
#define OU2ISO_15(rtlOUString)     ::rtl::OString((rtlOUString).getStr(), (rtlOUString).getLength(), RTL_TEXTENCODING_ISO_8859_15).getStr()
#define OU2A(rtlOUString)     ::rtl::OString((rtlOUString).getStr(), (rtlOUString).getLength(), RTL_TEXTENCODING_ASCII_US).getStr()
#define OU2UTF8(rtlOUString)     ::rtl::OString((rtlOUString).getStr(), (rtlOUString).getLength(), RTL_TEXTENCODING_UTF8).getStr()
#define MACROTOSTRING(macro) MACROTOSTRING_2(macro)
#define MACROTOSTRING_2(text) #text

// debugging
#ifdef LF_DEBUG_OUTPUT
#define LF_LOG(x) printf x
#else
#define LF_LOG(x) 
#endif


// FIXME
#define LF_ASSERT(x, y)

// <tools/solar.h>
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

// <svtools/linguprops.hxx>
// UNO property names for general options
#define UPN_IS_GERMAN_PRE_REFORM                        "IsGermanPreReform"
#define UPN_IS_USE_DICTIONARY_LIST              "IsUseDictionaryList"
#define UPN_IS_IGNORE_CONTROL_CHARACTERS        "IsIgnoreControlCharacters"
#define UPN_ACTIVE_DICTIONARIES                         "ActiveDictionaries"

// UNO property names for SpellChecker
#define UPN_IS_SPELL_UPPER_CASE                         "IsSpellUpperCase"
#define UPN_IS_SPELL_WITH_DIGITS                        "IsSpellWithDigits"
#define UPN_IS_SPELL_CAPITALIZATION                     "IsSpellCapitalization"

// UNO property names for Hyphenator
#define UPN_HYPH_MIN_LEADING                            "HyphMinLeading"
#define UPN_HYPH_MIN_TRAILING                           "HyphMinTrailing"
#define UPN_HYPH_MIN_WORD_LENGTH                        "HyphMinWordLength"

// uno property handles
#define UPH_IS_GERMAN_PRE_REFORM                         0
#define UPH_IS_USE_DICTIONARY_LIST                       1
#define UPH_IS_IGNORE_CONTROL_CHARACTERS         2
#define UPH_IS_SPELL_UPPER_CASE                          3
#define UPH_IS_SPELL_WITH_DIGITS                         4
#define UPH_IS_SPELL_CAPITALIZATION                      5
#define UPH_HYPH_MIN_LEADING                             6
#define UPH_HYPH_MIN_TRAILING                            7
#define UPH_HYPH_MIN_WORD_LENGTH                         8
#define UPH_DEFAULT_LOCALE                                       9
#define UPH_IS_SPELL_AUTO                                       10
#define UPH_IS_SPELL_HIDE                                       11
#define UPH_IS_SPELL_IN_ALL_LANGUAGES           12
#define UPH_IS_SPELL_SPECIAL                            13
#define UPH_IS_HYPH_AUTO                                        14
#define UPH_IS_HYPH_SPECIAL                                     15
#define UPH_IS_WRAP_REVERSE                                     16
#define UPH_DATA_FILES_CHANGED_CHECK_VALUE  17
#define UPH_DEFAULT_LANGUAGE                            21
#define UPH_DEFAULT_LOCALE_CJK                          22
#define UPH_DEFAULT_LOCALE_CTL                          23
#define UPH_ACTIVE_DICTIONARIES                         24
#define UPH_ACTIVE_CONVERSION_DICTIONARIES  25
#define UPH_IS_IGNORE_POST_POSITIONAL_WORD  26
#define UPH_IS_AUTO_CLOSE_DIALOG            27
#define UPH_IS_SHOW_ENTRIES_RECENTLY_USED_FIRST     28
#define UPH_IS_AUTO_REPLACE_UNIQUE_ENTRIES  29
#define UPH_IS_DIRECTION_TO_SIMPLIFIED      30
#define UPH_IS_USE_CHARACTER_VARIANTS       31
#define UPH_IS_TRANSLATE_COMMON_TERMS       32
#define UPH_IS_REVERSE_MAPPING              33




// svtools/inc/lngmisc.hxx
#define SVT_SOFT_HYPHEN ((sal_Unicode) 0x00AD)
#define SVT_HARD_HYPHEN ((sal_Unicode) 0x2011)

namespace LF_NAMESPACE {

OUString getInstallationPath();

inline sal_Bool IsHyphen( sal_Unicode cChar )
{
        return cChar == SVT_SOFT_HYPHEN  ||  cChar == SVT_HARD_HYPHEN;
}

inline sal_Bool IsControlChar( sal_Unicode cChar )
{
        return cChar < (sal_Unicode) ' ';
}


sal_Bool operator == ( const Locale &rL1, const Locale &rL2 );

// <linguistic/misc>
osl::Mutex &  GetLinguMutex();
sal_Int32   GetPosInWordToCheck( const rtl::OUString &rTxt, sal_Int32 nPos );

}

#endif
