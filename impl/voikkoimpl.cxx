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
#ifdef UNX
#include <dlfcn.h>
#endif
#ifdef WNT
#include <windows.h>
#endif

#include <rtl/ustrbuf.hxx>

#define VOIKKO_SPELL_FAILED 0
#define VOIKKO_SPELL_OK 1
#define VOIKKO_INTERNAL_ERROR 2
#define VOIKKO_CHARSET_CONVERSION_FAILED 3

#define VOIKKO_OPT_IGNORE_DOT 0
#define VOIKKO_OPT_IGNORE_NUMBERS 1
#define VOIKKO_OPT_ENCODING 2
#define VOIKKO_OPT_IGNORE_UPPERCASE 3

#ifdef UNX
#define VOIKKO_LIB "libvoikko.so.0"
#endif
#ifdef WNT
#define VOIKKO_LIB "libvoikko.dll"
#endif



char * voikko_init();

int voikko_set_bool_option(int option, int value);

int voikko_set_string_option(int option, const char * value);

int voikko_spell_cstr(const char * word);

char ** voikko_suggest_cstr(const char * word);

char * voikko_hyphenate_cstr(const char * word);



namespace LF_NAMESPACE {

int closeDl(void *handle);
static void * openDl(const char *file);
static void * getFunc(void *handle, const char *name);

#define LOAD_FUNC(fptr_type, fptr_name, symbol_name) \
    fptr_name = (fptr_type) getFunc(voikko_handle, (symbol_name)); \
    if (! fptr_name)  { \
      LF_LOG(("Failed to load symbol %s\n", (symbol_name))); \
      closeDl(voikko_handle); \
      return; \
    }

/*
 * Opens a dynamic library
 */
static void * openDl(const char *file_name) {
#ifdef WNT
  return (void *)LoadLibrary(file_name);
#endif
#ifdef UNX
  return dlopen(file_name, RTLD_NOW);
#endif
  return NULL;
}

/*
 * Returns named function of the given dynamic library
 */
static void * getFunc(void *handle, const char *name) {
	LF_LOG(("getFunc(%s)\n", name));
#ifdef WNT
 	return (void *)GetProcAddress((HMODULE)handle, name);
#endif
#ifdef UNX
	return dlsym(handle, name);
#endif
	return NULL;
}

/*
 * Closes the dynamic library.
 * Returns 0 on success, non-zero on error.
 */
int closeDl(void *handle) {
	int ret = 0;
	if (handle == NULL) return 0;
#ifdef WNT
	ret = FreeLibrary((HMODULE)handle) ? 0 : 1;
#endif
#ifdef UNX
	ret =  dlclose(handle);
#endif
	return ret;
}


typedef int (*setbopt_t)(int, int);
typedef int (*spell_t)(const char *);
typedef char ** (*suggest_t)(const char *);
typedef char * (*hyphenate_t)(const char *);

sal_Bool voikko_initialised = FALSE;
void * voikko_handle = 0;
setbopt_t voikko_set_bool_option = 0;
spell_t voikko_spell_cstr = 0;
suggest_t voikko_suggest_cstr = 0;
hyphenate_t voikko_hyphenate_cstr = 0;

/* Begin function implementations */

void lfInitSpeller() {
    MutexGuard aGuard(GetLinguMutex());
    if (!voikko_initialised) {
        voikko_handle = openDl(VOIKKO_LIB);
	if (!voikko_handle) {
	    LF_LOG(("Failed to dlopen " VOIKKO_LIB "\n"));
	    return;
	}
	typedef char * (*initvoikko_t)();
	initvoikko_t initvoikko;
	LOAD_FUNC(initvoikko_t, initvoikko, "voikko_init");
	char * initerror = initvoikko();
	if (initerror) {
		LF_LOG(("Failed to initialise voikko: %s\n", initerror));
		closeDl(voikko_handle);
		return;
	}
	typedef int (*setsopt_t)(int, const char *);
	setsopt_t setsopt;
	LOAD_FUNC(setsopt_t, setsopt, "voikko_set_string_option");
	setsopt(VOIKKO_OPT_ENCODING, "UTF-8");
	LOAD_FUNC(setbopt_t, voikko_set_bool_option, "voikko_set_bool_option");
	voikko_set_bool_option(VOIKKO_OPT_IGNORE_DOT, 1);
	LOAD_FUNC(spell_t, voikko_spell_cstr, "voikko_spell_cstr");
	LOAD_FUNC(suggest_t, voikko_suggest_cstr, "voikko_suggest_cstr");
	LOAD_FUNC(hyphenate_t, voikko_hyphenate_cstr, "voikko_hyphenate_cstr");
	// TODO: check for errors
	LF_LOG(("lfInitSpeller completed\n"));
	voikko_initialised = TRUE;
    }
}

void lfInitHyphenator() {

}

void lfDisposeSpeller() {

}

void lfDisposeHyphenator() {

}

sal_Bool SAL_CALL lfIsValid( const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase) 
		throw(::com::sun::star::lang::IllegalArgumentException,	::com::sun::star::uno::RuntimeException) {

	MutexGuard aGuard( GetLinguMutex() );
	if (!voikko_initialised) return FALSE;
	char * c_str = (char *) OU2UTF8(rWord);
	if (isSpellWithDigits) voikko_set_bool_option(VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	
	//LF_LOG(("c_str: '%s'\n", c_str));
	if (voikko_spell_cstr(c_str)) return TRUE;
	else return FALSE;
}



Reference< XSpellAlternatives > SAL_CALL lfSpell(const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase) 
		throw(::com::sun::star::uno::RuntimeException) {

	MutexGuard aGuard(GetLinguMutex());
	if (!voikko_initialised) return FALSE;
	char * c_str = (char *) OU2UTF8(rWord);
	if (isSpellWithDigits) voikko_set_bool_option(VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	
	char ** suggestions = voikko_suggest_cstr(c_str);
	if (suggestions == 0 || suggestions[0] == 0) return 0;
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

	SpellAlternatives *alternatives = new SpellAlternatives;
	alternatives->word = rWord;
	alternatives->alternatives = suggSeq;
	return alternatives;
}


Reference< XHyphenatedWord > SAL_CALL lfHyphenate(const ::rtl::OUString& aWord, sal_Int16 nMaxLeading)
		throw(::com::sun::star::uno::RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	OString oWord(OU2UTF8(aWord));
	char * hyphenationPoints = voikko_hyphenate_cstr(oWord.getStr());
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
	OString oWord(OU2UTF8(aWord));
	Reference< XPossibleHyphens > xRes;
	char * hyphenationPoints = voikko_hyphenate_cstr(oWord.getStr());
	
	/* Count the number of hyphenation points but remove the ones that correspond
	* to a real hyphen in the word. This is required to prevent adding extra
	* soft hyphen where it is not needed. */
	sal_Int16 hpcount = 0;
	for (int i = 0; i < oWord.getLength(); i++) {
		if (hyphenationPoints[i] == '-') {
			if (aWord[i - 1] == '-') hyphenationPoints[i] = ' ';
			else hpcount++;
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
	LF_LOG(("\n  hyphenatedWord: '%s'\n", OU2A(hyphenatedWord)));
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

