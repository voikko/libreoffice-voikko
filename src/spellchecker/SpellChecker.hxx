/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 Harri Pitkänen <hatapitk@iki.fi>
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

#ifndef _SPELLCHECKER_HXX_
#define _SPELLCHECKER_HXX_

//#include "sal/config.h"
#include <cppuhelper/compbase5.hxx>
#include <cppuhelper/basemutex.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/linguistic2/XSpellChecker.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventBroadcaster.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/XServiceDisplayName.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "../PropertyManager.hxx"
#include "../macros.hxx"

using namespace ::com::sun::star;
using namespace ::rtl;

namespace hspell {

class SpellChecker:
	private cppu::BaseMutex,
	public cppu::WeakComponentImplHelper5
	            <lang::XServiceInfo,
	             linguistic2::XSpellChecker,
	             linguistic2::XLinguServiceEventBroadcaster,
	             lang::XInitialization,
	             lang::XServiceDisplayName> {

	public:
	explicit SpellChecker(uno::Reference<uno::XComponentContext> const & context);
	virtual ~SpellChecker() {}

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

	// ::com::sun::star::linguistic2::XSpellChecker:
	virtual sal_Bool SAL_CALL isValid(const OUString & aWord, const lang::Locale & aLocale,
	                                  const uno::Sequence<beans::PropertyValue> & aProperties)
		throw (uno::RuntimeException, lang::IllegalArgumentException);
	virtual uno::Reference<linguistic2::XSpellAlternatives> SAL_CALL
		spell(const OUString & aWord, const lang::Locale & aLocale,
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
	static inline OUString getImplementationName_static();
	static inline uno::Sequence<OUString> getSupportedServiceNames_static();

	private:
	// SpellChecker(SpellChecker &); // not defined
	// void operator =(SpellChecker &); // not defined

	// overload WeakComponentImplHelperBase::disposing()
	// This function is called upon disposing the component,
	// if your component needs special work when it becomes
	// disposed, do it here.
	virtual void SAL_CALL disposing();

	uno::Reference<uno::XComponentContext> compContext;
};

// Static method implementations
inline OUString SpellChecker::getImplementationName_static() {
	return A2OU("hspell.SpellChecker");
}
inline uno::Sequence<OUString> SpellChecker::getSupportedServiceNames_static() {
	uno::Sequence<OUString> snames(1);
	snames.getArray()[0] = A2OU("com.sun.star.linguistic2.SpellChecker");
	return snames;
}


}

#endif
