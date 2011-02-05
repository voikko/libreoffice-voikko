/* Libreoffice-voikko: Finnish linguistic extension for LibreOffice
 * Copyright (C) 2005 - 2010 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

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
