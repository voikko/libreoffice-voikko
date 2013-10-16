/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2005 - 2010 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#include "HyphenatedWord.hxx"
#include "../macros.hxx"

namespace voikko {

HyphenatedWord::HyphenatedWord(const OUString & wrd, sal_Int16 pos, const lang::Locale & locale) :
	word(wrd),
	hyphenPos(pos),
	locale(locale) {
	if (wrd[pos+1] == '-') {
		hyphenatedWord = wrd.replaceAt(pos+1, 1, A2OU(""));
		isAlternative = sal_True;
	}
	else {
		hyphenatedWord = wrd;
		isAlternative = sal_False;
	}
	VOIKKO_DEBUG_2("%s", OU2DEBUG(wrd));
	//VOIKKO_DEBUG_2("%s", OU2DEBUG(hyphenatedWord));
	VOIKKO_DEBUG_2("%i", pos);
}

OUString SAL_CALL HyphenatedWord::getWord() throw (uno::RuntimeException) {
	return word;
}

OUString SAL_CALL HyphenatedWord::getHyphenatedWord() throw (uno::RuntimeException) {
	return hyphenatedWord;
}

lang::Locale SAL_CALL HyphenatedWord::getLocale() throw (uno::RuntimeException) {
	return locale;
}

sal_Int16 SAL_CALL HyphenatedWord::getHyphenationPos() throw (uno::RuntimeException) {
	return hyphenPos;
}

sal_Int16 SAL_CALL HyphenatedWord::getHyphenPos() throw (uno::RuntimeException) {
	return hyphenPos;
}

sal_Bool SAL_CALL HyphenatedWord::isAlternativeSpelling() throw (uno::RuntimeException) {
	return isAlternative;
}

}
