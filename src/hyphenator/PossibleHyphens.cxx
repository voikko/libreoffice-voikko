/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
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
