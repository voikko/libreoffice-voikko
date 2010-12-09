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

#include "SpellAlternatives.hxx"
#include "../macros.hxx"

namespace voikko {

SpellAlternatives::SpellAlternatives(const OUString & word, uno::Sequence<OUString> alternatives, const lang::Locale & locale) :
	word(word),
	alternatives(alternatives),
	locale(locale) {
}

OUString SAL_CALL SpellAlternatives::getWord() throw (uno::RuntimeException) {
	return word;
}

lang::Locale SAL_CALL SpellAlternatives::getLocale() throw (uno::RuntimeException) {
	return locale;
}

sal_Int16 SAL_CALL SpellAlternatives::getFailureType() throw (uno::RuntimeException) {
	// TODO: If Voikko supported specific failure types, we could use them here
	return linguistic2::SpellFailure::SPELLING_ERROR;
}

sal_Int16 SAL_CALL SpellAlternatives::getAlternativesCount() throw (uno::RuntimeException) {
	return static_cast<sal_Int16>(alternatives.getLength()); // Integer overflow is not possible
}

uno::Sequence<OUString> SAL_CALL SpellAlternatives::getAlternatives() throw (uno::RuntimeException) {
	return alternatives;
}

}
