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
