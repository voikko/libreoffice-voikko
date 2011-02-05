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

#ifndef _HYPHENATEDWORD_HXX_
#define _HYPHENATEDWORD_HXX_

#include <com/sun/star/linguistic2/XHyphenatedWord.hpp>
#include <cppuhelper/implbase1.hxx>

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

/** Implementation of interface com::sun::star::linguistic2::XHyphenatedWord. See
 *  the interface documentation for more information.
 */
class HyphenatedWord : public cppu::WeakImplHelper1<linguistic2::XHyphenatedWord> {
	private:
	const OUString word;
	const sal_Int16 hyphenPos;
	const lang::Locale locale;
	OUString hyphenatedWord;
	sal_Bool isAlternative;

	public:
	HyphenatedWord(const OUString & wrd, sal_Int16 pos, const lang::Locale & locale);
	virtual OUString SAL_CALL getWord() throw (uno::RuntimeException);
	virtual lang::Locale SAL_CALL getLocale() throw (uno::RuntimeException);
	virtual sal_Int16 SAL_CALL getHyphenationPos() throw (uno::RuntimeException);
	virtual sal_Int16 SAL_CALL getHyphenPos() throw (uno::RuntimeException);
	virtual OUString SAL_CALL getHyphenatedWord() throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL isAlternativeSpelling() throw (uno::RuntimeException);
};

}

#endif
