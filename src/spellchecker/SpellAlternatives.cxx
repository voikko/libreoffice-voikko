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

#include "SpellAlternatives.hxx"
#include "../macros.hxx"

namespace voikko {

OUString SAL_CALL SpellAlternatives::getWord() throw (uno::RuntimeException) {
	return word;
}

lang::Locale SAL_CALL SpellAlternatives::getLocale() throw (uno::RuntimeException) {
	return lang::Locale(A2OU("fi"), A2OU("FI"), A2OU(""));
}

sal_Int16 SAL_CALL SpellAlternatives::getFailureType() throw (uno::RuntimeException) {
	// TODO: If Voikko supported specific failure types, we could use them here
	return linguistic2::SpellFailure::SPELLING_ERROR;
}

sal_Int16 SAL_CALL SpellAlternatives::getAlternativesCount() throw (uno::RuntimeException) {
	return alternatives.getLength();
}

uno::Sequence<OUString> SAL_CALL SpellAlternatives::getAlternatives() throw (uno::RuntimeException) {
	return alternatives;
}

}
