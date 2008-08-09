/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2008 Harri Pitkänen <hatapitk@iki.fi>
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

#ifndef _GRAMMARCHECKER_HXX_
#define _GRAMMARCHECKER_HXX_

#include <cppuhelper/compbase4.hxx>
#include <cppuhelper/basemutex.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/linguistic2/XGrammarChecker.hpp>
#include <com/sun/star/linguistic2/GrammarCheckingResult.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/XServiceDisplayName.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "../macros.hxx"

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

class GrammarChecker:
	private cppu::BaseMutex,
	public cppu::WeakComponentImplHelper4
	            <lang::XServiceInfo,
	             linguistic2::XGrammarChecker,
	             lang::XInitialization,
	             lang::XServiceDisplayName> {

	public:
	explicit GrammarChecker(uno::Reference<uno::XComponentContext> const & context);
	virtual ~GrammarChecker() {}

	// ::com::sun::star::lang::XServiceInfo:
	virtual OUString SAL_CALL getImplementationName()
		throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL supportsService(const OUString & serviceName)
		throw (uno::RuntimeException);
	virtual uno::Sequence<OUString> SAL_CALL getSupportedServiceNames()
		throw (uno::RuntimeException);

	// linguistic2::XSupportedLocales:
	virtual uno::Sequence<lang::Locale> SAL_CALL getLocales()
		throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL hasLocale(const lang::Locale & aLocale)
		throw (uno::RuntimeException);

	// linguistic2::XGrammarChecker:
	virtual sal_Bool SAL_CALL isSpellChecker() throw (uno::RuntimeException);
	virtual void SAL_CALL startDocument(sal_Int32 nDocId)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual void SAL_CALL startParagraph(sal_Int32 nDocId)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual void SAL_CALL endParagraph(sal_Int32 nDocId)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual void SAL_CALL endDocument(sal_Int32 nDocId)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual linguistic2::GrammarCheckingResult SAL_CALL doGrammarChecking(sal_Int32 nDocId,
		const OUString & aText,
		const lang::Locale & aLocale,
		sal_Int32 nStartOfSentencePos, sal_Int32 nSuggestedSentenceEndPos,
		const uno::Sequence<sal_Int32> & aLanguagePortions,
		const uno::Sequence<lang::Locale> & aLanguagePortionsLocales)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual sal_Bool SAL_CALL hasOptionsDialog() throw (uno::RuntimeException);
	virtual void SAL_CALL runOptionsDialog()
		throw (uno::RuntimeException);

	// ::com::sun::star::lang::XInitialization:
	virtual void SAL_CALL initialize(const uno::Sequence<uno::Any> & aArguments)
		throw (uno::RuntimeException, uno::Exception);

	// ::com::sun::star::lang::XServiceDisplayName:
	virtual OUString SAL_CALL getServiceDisplayName(const lang::Locale & aLocale)
		throw (uno::RuntimeException);

	// Static methods
	static inline OUString getImplementationName_static();
	static inline uno::Sequence<OUString> getSupportedServiceNames_static();

	private:
	virtual void SAL_CALL disposing();
	uno::Reference<uno::XComponentContext> compContext;
};

// Static method implementations
inline OUString GrammarChecker::getImplementationName_static() {
	VOIKKO_DEBUG("GrammarChecker::getImplementationName_static");
	return A2OU("voikko.GrammarChecker");
}
inline uno::Sequence<OUString> GrammarChecker::getSupportedServiceNames_static() {
	uno::Sequence<OUString> snames(1);
	VOIKKO_DEBUG("GrammarChecker::getSupportedServiceNames_static");
	snames.getArray()[0] = A2OU("com.sun.star.linguistic2.GrammarChecker");
	//snames.getArray()[1] = A2OU("voikko.GrammarChecker");
	return snames;
}

}

#endif
