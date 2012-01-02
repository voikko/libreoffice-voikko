/* Libreoffice-voikko: Finnish linguistic extension for LibreOffice
 * Copyright (C) 2008 - 2010 Harri Pitkänen <hatapitk@iki.fi>
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

#include <set>
#include <cppuhelper/compbase4.hxx>
#include <cppuhelper/basemutex.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/linguistic2/XProofreader.hpp>
#include <com/sun/star/linguistic2/ProofreadingResult.hpp>
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
	             linguistic2::XProofreader,
	             lang::XInitialization,
	             lang::XServiceDisplayName> {

	public:
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

	// linguistic2::XProofreader:
	virtual sal_Bool SAL_CALL isSpellChecker() throw (uno::RuntimeException);
	virtual linguistic2::ProofreadingResult SAL_CALL doProofreading(
		const OUString & aDocumentIdentifier,
		const OUString & aText,
		const lang::Locale & aLocale,
		sal_Int32 nStartOfSentencePos, sal_Int32 nSuggestedBehindEndOfSentencePosition,
		const uno::Sequence<beans::PropertyValue> & aProperties)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual void SAL_CALL ignoreRule(const OUString & aRuleIdentifier, const lang::Locale & aLocale)
		throw (lang::IllegalArgumentException, uno::RuntimeException);
    virtual void SAL_CALL resetIgnoreRules() throw (uno::RuntimeException);

	// ::com::sun::star::lang::XInitialization:
	virtual void SAL_CALL initialize(const uno::Sequence<uno::Any> & aArguments)
		throw (uno::RuntimeException, uno::Exception);

	// ::com::sun::star::lang::XServiceDisplayName:
	virtual OUString SAL_CALL getServiceDisplayName(const lang::Locale & aLocale)
		throw (uno::RuntimeException);

	// Static methods
	static inline OUString SAL_CALL getImplementationName_static();
	static inline uno::Sequence<OUString> SAL_CALL getSupportedServiceNames_static();
	static uno::Reference<XInterface> SAL_CALL get(uno::Reference<uno::XComponentContext> const & context);

	private:
	explicit GrammarChecker(uno::Reference<uno::XComponentContext> const & context);
	virtual ~GrammarChecker();
	virtual void SAL_CALL disposing();
	uno::Reference<uno::XComponentContext> compContext;
	
	/** Grammar checker error codes that should be ignored */
	std::set<OUString> ignoredErrors;
};

// Static method implementations
inline OUString SAL_CALL GrammarChecker::getImplementationName_static() {
	VOIKKO_DEBUG("GrammarChecker::getImplementationName_static");
	return A2OU("voikko.GrammarChecker");
}
inline uno::Sequence<OUString> SAL_CALL GrammarChecker::getSupportedServiceNames_static() {
	uno::Sequence<OUString> snames(1);
	VOIKKO_DEBUG("GrammarChecker::getSupportedServiceNames_static");
	snames.getArray()[0] = A2OU("com.sun.star.linguistic2.Proofreader");
	return snames;
}

}

#endif
