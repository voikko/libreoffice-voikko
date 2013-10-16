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
	
	/** Returns the language code for user messages */
	const char * getMessageLanguage();
	
	/** Reload Voikko specific settings after they have been changed
	    in the settings dialog. */
	void reloadVoikkoSettings();
	
	/** Returns the singleton instance of this object */
	static uno::Reference<voikko::PropertyManager> get(uno::Reference<uno::XComponentContext> const & context);
	
	private:
	explicit PropertyManager(uno::Reference<uno::XComponentContext> cContext);
	virtual ~PropertyManager();
	
	void setUiLanguage();
	
	void setProperties(const uno::Reference<beans::XPropertySet> & properties);
	void setValue(const beans::PropertyValue & value);
	void syncHyphenatorSettings();
	void sendLinguEvent(const linguistic2::LinguServiceEvent & event);
	
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
