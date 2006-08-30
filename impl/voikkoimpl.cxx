/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@cc.jyu.fi), 2006
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
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


#include <lfmacros.hxx>
#include <lfimpl.hxx>
#include <libvoikko/voikko.h>

#include <rtl/ustrbuf.hxx>


namespace LF_NAMESPACE {


sal_Bool voikko_initialised = FALSE;
int voikko_handle;

/* Begin function implementations */

void lfInitSpeller() {
    MutexGuard aGuard(GetLinguMutex());
    if (!voikko_initialised) {
	const char * initerror = voikko_init(&voikko_handle, "fi_FI", 0);
	if (initerror) {
		LF_LOG(("Failed to initialise voikko: %s\n", initerror));
		return;
	}
	voikko_set_string_option(voikko_handle, VOIKKO_OPT_ENCODING, "UTF-8");
	voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_DOT, 1);
	voikko_set_bool_option(voikko_handle, VOIKKO_OPT_NO_UGLY_HYPHENATION, 1);
	LF_LOG(("lfInitSpeller completed\n"));
	voikko_initialised = TRUE;
    }
}

void lfInitHyphenator() {
	lfInitSpeller();
}

void lfDisposeSpeller() {
	MutexGuard aGuard(GetLinguMutex());
	if (voikko_initialised) {
		voikko_terminate(voikko_handle);
		voikko_initialised = FALSE;
	}
}

void lfDisposeHyphenator() {
	lfDisposeSpeller();
}

sal_Bool SAL_CALL lfIsValid( const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase,
			     sal_Bool isSpellCapitalization) 
		throw(::com::sun::star::lang::IllegalArgumentException,	::com::sun::star::uno::RuntimeException) {

	MutexGuard aGuard( GetLinguMutex() );
	if (!voikko_initialised) return FALSE;
	OString oWord = OUStringToOString(rWord, RTL_TEXTENCODING_UTF8);
	const char * c_str = oWord.getStr();
	if (isSpellWithDigits) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	if (isSpellCapitalization) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 1);
	
	LF_LOG(("c_str: '%s'\n", c_str));
	int result = voikko_spell_cstr(voikko_handle, c_str);
	LF_LOG((" result = %i\n", result));
	if (result) return TRUE;
	else return FALSE;
}



Reference< XSpellAlternatives > SAL_CALL lfSpell(const OUString& rWord, sal_Bool isSpellWithDigits,
                                sal_Bool isSpellUpperCase, sal_Bool isSpellCapitalization) 
		throw(::com::sun::star::uno::RuntimeException) {

	MutexGuard aGuard(GetLinguMutex());
	if (!voikko_initialised) return FALSE;
	OString oWord = OUStringToOString(rWord, RTL_TEXTENCODING_UTF8);
	const char * c_str = oWord.getStr();
	if (isSpellWithDigits) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	if (isSpellCapitalization) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 1);

	if (voikko_spell_cstr(voikko_handle, c_str)) return 0;
	char ** suggestions = voikko_suggest_cstr(voikko_handle, c_str);
	SpellAlternatives *alternatives = new SpellAlternatives;
	alternatives->word = rWord;
	if (suggestions == 0 || suggestions[0] == 0) return alternatives;
	int scount = 0;
	while (suggestions[scount] != 0) scount++;
	Sequence< OUString > suggSeq(scount);
	OUString * suggStrings = suggSeq.getArray();
	
	OString ostr;
	for (int i = 0; i < scount; i++) {
		ostr = OString(suggestions[i]);
		free(suggestions[i]);
		suggStrings[i] = OStringToOUString(ostr, RTL_TEXTENCODING_UTF8);
	}
	free(suggestions);

	alternatives->alternatives = suggSeq;
	return alternatives;
}


Reference< XHyphenatedWord > SAL_CALL lfHyphenate(const ::rtl::OUString& aWord, sal_Int16 nMaxLeading)
		throw(::com::sun::star::uno::RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	char * hyphenationPoints = voikko_hyphenate_cstr(voikko_handle, oWord.getStr());
	LF_LOG(("lfHyphenate: aWord='%s'\n", oWord.getStr()));
	LF_LOG(("lfHyphenate: hyphenationPoints='%s'\n", hyphenationPoints));

	// find the hyphenation point
	sal_Int16 hyphenPos = -1;
	for (int i = nMaxLeading; i>0 && hyphenPos == -1; i--)
		if (hyphenationPoints[i] == '-') hyphenPos = i;
	LF_LOG(("lfHyphenate: nMaxLeading=%d, hyphenPos=%d\n", nMaxLeading, hyphenPos));
	
	// return the result
	free(hyphenationPoints);
	if (hyphenPos != -1) return new HyphenatedWord(aWord, hyphenPos - 1);
	else return NULL;
}


Reference< XPossibleHyphens > SAL_CALL lfCreatePossibleHyphens(const ::rtl::OUString& aWord)
		throw(::com::sun::star::uno::RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	Reference< XPossibleHyphens > xRes;
	char * hyphenationPoints = voikko_hyphenate_cstr(voikko_handle, oWord.getStr());
	
	/* Count the number of hyphenation points but remove the ones that correspond
	* to a real hyphen in the word. This is required to prevent adding extra
	* soft hyphen where it is not needed. */
	sal_Int16 hpcount = 0;
	for (int i = 0; i < oWord.getLength(); i++) {
		if (hyphenationPoints[i] == '-') {
			// if (aWord[i - 1] == '-') hyphenationPoints[i] = ' ';
			//else hpcount++;
			hpcount++;
		}
	}

	Sequence< sal_Int16 > hyphenSeq(hpcount);
	sal_Int16 *pPos = hyphenSeq.getArray();
	OUStringBuffer hyphenatedWordBuffer;
	OUString hyphenatedWord;
	sal_Int16 nHyphCount = 0;
	for (int i = 0; i < oWord.getLength(); i++) {
		hyphenatedWordBuffer.append(aWord[i]);
		if (hyphenationPoints[i + 1] == '-') {
			pPos[nHyphCount] = i;
			hyphenatedWordBuffer.append(sal_Unicode('='));
			nHyphCount++;
		}
	}

	hyphenatedWord = hyphenatedWordBuffer.makeStringAndClear();
	xRes = new PossibleHyphens(aWord, hyphenatedWord, hyphenSeq);

	free(hyphenationPoints);
	return xRes;
}

OUString lfHyphenatorDisplayName(const Locale& rLocale) {
	if (rLocale == Locale( A2OU("fi"), A2OU("FI"), OUString() ))
		return A2OU("Suomen kielen tavutus (Voikko)");
	else
		return A2OU("Finnish hyphenator (Voikko)");
}

OUString lfSpellerDisplayName(const Locale& rLocale) {
	if (rLocale == Locale( A2OU("fi"), A2OU("FI"), OUString() ))
		return A2OU( "Suomen kielen oikoluku (Voikko)" );
	else
		return A2OU( "Finnish spellchecker (Voikko)" );
}

}

