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
	HyphenatedWord & operator=(const HyphenatedWord&);

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
