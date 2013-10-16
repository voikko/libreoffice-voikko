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

#ifndef _POSSIBLEHYPHENS_HXX_
#define _POSSIBLEHYPHENS_HXX_

#include <com/sun/star/linguistic2/XPossibleHyphens.hpp>
#include <cppuhelper/implbase1.hxx>

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

/** Implementation of interface com::sun::star::linguistic2::XPossibleHyphens. See
 *  the interface documentation for more information.
 */
class PossibleHyphens : public cppu::WeakImplHelper1<linguistic2::XPossibleHyphens> {
	private:
	const OUString word;
	const OUString hyphenatedWord;
	const uno::Sequence<sal_Int16> hyphenationPositions;
	const lang::Locale locale;
	PossibleHyphens & operator=(const PossibleHyphens&);

	public:
	PossibleHyphens(const OUString & wrd, const OUString & hypWrd, uno::Sequence<sal_Int16> positions,
	                const lang::Locale & locale);
	virtual OUString SAL_CALL getWord() throw (uno::RuntimeException);
	virtual lang::Locale SAL_CALL getLocale() throw (uno::RuntimeException);
	virtual OUString SAL_CALL getPossibleHyphens() throw (uno::RuntimeException);
	virtual uno::Sequence<sal_Int16> SAL_CALL getHyphenationPositions()
		throw (uno::RuntimeException);

};

}

#endif
