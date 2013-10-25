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

#ifndef _SPELLALTERNATIVES_HXX_
#define _SPELLALTERNATIVES_HXX_

#include <com/sun/star/linguistic2/XSpellAlternatives.hpp>
#include <com/sun/star/linguistic2/SpellFailure.hpp>
#include <cppuhelper/implbase1.hxx>

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

class SpellAlternatives : public cppu::WeakImplHelper1<linguistic2::XSpellAlternatives> {
	private:
	const OUString word;
	const uno::Sequence<OUString> alternatives;
	const lang::Locale locale;
	SpellAlternatives & operator=(const SpellAlternatives&);

	public:
	SpellAlternatives(const OUString & word, uno::Sequence<OUString> alternatives, const lang::Locale & locale);
	virtual OUString SAL_CALL getWord() throw (uno::RuntimeException);
	virtual lang::Locale SAL_CALL getLocale() throw (uno::RuntimeException);
	virtual sal_Int16 SAL_CALL getFailureType() throw (uno::RuntimeException);
	virtual sal_Int16 SAL_CALL getAlternativesCount() throw (uno::RuntimeException);
	virtual uno::Sequence<OUString> SAL_CALL getAlternatives() throw (uno::RuntimeException);
};

}

#endif
