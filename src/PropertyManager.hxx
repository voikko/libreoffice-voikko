/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 Harri Pitkänen <hatapitk@iki.fi>
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

#ifndef _PROPERTYMANAGER_HXX_
#define _PROPERTYMANAGER_HXX_

#include <cppuhelper/implbase1.hxx>
#include <cppuhelper/interfacecontainer.h>
#include <com/sun/star/beans/XPropertyChangeListener.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventListener.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "macros.hxx"

namespace voikko {

using namespace ::com::sun::star;
using namespace ::rtl;

class PropertyManager:
	public cppu::WeakImplHelper1<beans::XPropertyChangeListener> {
	public:
	explicit PropertyManager(uno::Reference<uno::XComponentContext> cContext);
	virtual ~PropertyManager();
	
	// XPropertyChangeListener
	virtual void SAL_CALL propertyChange(const beans::PropertyChangeEvent & pce)
		throw (uno::RuntimeException);

	// XEventListener
	virtual void SAL_CALL disposing(const lang::EventObject & source)
		throw (uno::RuntimeException);
	
	void initialize() throw (uno::Exception);
	
	// Functions to be used by the linguistic tool implementations
	sal_Int16 getHyphMinLeading();
	sal_Int16 getHyphMinTrailing();
	sal_Int16 getHyphMinWordLength();
	sal_Bool addLinguServiceEventListener(
		const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
		throw (uno::RuntimeException);
	sal_Bool removeLinguServiceEventListener(
		const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
		throw (uno::RuntimeException);
	void setValues(const uno::Sequence<beans::PropertyValue> & values);
	void resetValues(const uno::Sequence<beans::PropertyValue> & values);
	
	// Functions for setting Voikko specific configuration
	void setHyphWordParts(sal_Bool value);
	uno::Any readFromRegistry(const OUString group, const OUString & key)
		throw (beans::UnknownPropertyException);
	void readVoikkoSettings();
	
	/** Returns initialization status diagnostics */
	OUString getInitializationStatus();

	/** Returns the language code for user messages */
	const char * getMessageLanguage();
	
	private:
	void setProperties(const uno::Reference<beans::XPropertySet> & properties);
	void setValue(const beans::PropertyValue & value);
	void syncHyphenatorSettings();
	void sendLinguEvent(const linguistic2::LinguServiceEvent & event);
	
	sal_Bool isInitialized;
	uno::Reference<beans::XPropertySet> linguPropSet;
	uno::Reference<uno::XComponentContext> compContext;
	::cppu::OInterfaceContainerHelper linguEventListeners;
	
	// Active values
	sal_Int16 hyphMinLeading;
	sal_Int16 hyphMinTrailing;
	sal_Int16 hyphMinWordLength;
	sal_Bool hyphWordParts;
	sal_Bool hyphUnknownWords;
	
	/** Error message from libvoikko initialization function */
	const char * voikkoErrorString;

	/** Language code for user messages */
	const char * messageLanguage;
};

}

#endif
