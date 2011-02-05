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
