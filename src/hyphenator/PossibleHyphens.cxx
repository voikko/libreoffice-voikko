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

#include "PossibleHyphens.hxx"
#include "../macros.hxx"

namespace voikko {

PossibleHyphens::PossibleHyphens(const OUString & wrd, const OUString & hypWrd,
	                         uno::Sequence<sal_Int16> positions, const lang::Locale & locale) :
	word(wrd),
	hyphenatedWord(hypWrd),
	hyphenationPositions(positions),
	locale(locale) {
}

OUString SAL_CALL PossibleHyphens::getWord() throw (uno::RuntimeException) {
	return word;
}

lang::Locale SAL_CALL PossibleHyphens::getLocale() throw (uno::RuntimeException) {
	return locale;
}

OUString SAL_CALL PossibleHyphens::getPossibleHyphens() throw (uno::RuntimeException) {
	return hyphenatedWord;
}

uno::Sequence<sal_Int16> SAL_CALL PossibleHyphens::getHyphenationPositions()
	throw (uno::RuntimeException) {
	return hyphenationPositions;
}

}
