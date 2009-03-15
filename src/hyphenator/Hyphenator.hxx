/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 - 2009 Harri Pitkänen <hatapitk@iki.fi>
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
	virtual ~Hyphenator() {}
	virtual void SAL_CALL disposing();

	uno::Reference<uno::XComponentContext> compContext;
};

}

#endif
