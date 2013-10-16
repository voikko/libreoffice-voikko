/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2007 - 2009 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#ifndef _HYPHENATOR_HXX_
#define _HYPHENATOR_HXX_

#include <cppuhelper/compbase5.hxx>
#include <cppuhelper/basemutex.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/linguistic2/XHyphenator.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventBroadcaster.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/XServiceDisplayName.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "../PropertyManager.hxx"
#include "../macros.hxx"

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

class Hyphenator:
	private cppu::BaseMutex,
	public cppu::WeakComponentImplHelper5
	            <lang::XServiceInfo,
	             linguistic2::XHyphenator,
	             linguistic2::XLinguServiceEventBroadcaster,
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

	// ::com::sun::star::linguistic2::XSupportedLocales:
	virtual uno::Sequence<lang::Locale> SAL_CALL getLocales()
		throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL hasLocale(const lang::Locale & aLocale)
		throw (uno::RuntimeException);

	// ::com::sun::star::linguistic2::XHyphenator:
	virtual uno::Reference<linguistic2::XHyphenatedWord> SAL_CALL
		hyphenate(const OUString & aWord, const lang::Locale & aLocale,
		          sal_Int16 nMaxLeading,
		          const uno::Sequence<beans::PropertyValue> & aProperties)
		throw (uno::RuntimeException, lang::IllegalArgumentException);

	virtual uno::Reference<linguistic2::XHyphenatedWord> SAL_CALL
		queryAlternativeSpelling(const OUString & aWord, const lang::Locale & aLocale,
		                         sal_Int16 nIndex,
		                         const uno::Sequence<beans::PropertyValue> & aProperties)
		throw (uno::RuntimeException, lang::IllegalArgumentException);

	virtual uno::Reference<linguistic2::XPossibleHyphens> SAL_CALL
		createPossibleHyphens(const OUString & aWord, const lang::Locale & aLocale,
		                      const uno::Sequence<beans::PropertyValue> & aProperties)
		throw (uno::RuntimeException, lang::IllegalArgumentException);

	// ::com::sun::star::linguistic2::XLinguServiceEventBroadcaster:
	virtual sal_Bool SAL_CALL addLinguServiceEventListener(
		const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
		throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL removeLinguServiceEventListener(
		const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
		throw (uno::RuntimeException);

	// ::com::sun::star::lang::XInitialization:
	virtual void SAL_CALL initialize(const uno::Sequence<uno::Any> & aArguments)
		throw (uno::RuntimeException, uno::Exception);

	// ::com::sun::star::lang::XServiceDisplayName:
	virtual OUString SAL_CALL getServiceDisplayName(const lang::Locale & aLocale)
		throw (uno::RuntimeException);

	// Static methods
	static OUString getImplementationName_static();
	static uno::Sequence<OUString> getSupportedServiceNames_static();
	static uno::Reference<XInterface> get(uno::Reference<uno::XComponentContext> const & context);

	private:
	explicit Hyphenator(uno::Reference<uno::XComponentContext> const & context);
	virtual ~Hyphenator();
	virtual void SAL_CALL disposing();

	uno::Reference<uno::XComponentContext> compContext;
};

}

#endif
