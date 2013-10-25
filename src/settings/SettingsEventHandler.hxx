/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2008 - 2009 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#ifndef _SETTINGSEVENTHANDLER_HXX_
#define _SETTINGSEVENTHANDLER_HXX_

#include <cppuhelper/compbase2.hxx>
#include <cppuhelper/basemutex.hxx>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/awt/XContainerWindowEventHandler.hpp>
#include <com/sun/star/awt/XControlContainer.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include "../macros.hxx"

using namespace ::com::sun::star;
using namespace ::rtl;

namespace voikko {

class SettingsEventHandler:
	private cppu::BaseMutex,
	public cppu::WeakComponentImplHelper2
	            <lang::XServiceInfo,
	             awt::XContainerWindowEventHandler> {

	public:
	explicit SettingsEventHandler(uno::Reference<uno::XComponentContext> const & context);
	virtual ~SettingsEventHandler() {}

	// ::com::sun::star::lang::XServiceInfo:
	virtual OUString SAL_CALL getImplementationName()
	        throw (uno::RuntimeException);
	virtual sal_Bool SAL_CALL supportsService(const OUString & serviceName)
	        throw (uno::RuntimeException);
	virtual uno::Sequence<OUString> SAL_CALL getSupportedServiceNames()
	        throw (uno::RuntimeException);

	// ::com::sun::star::awt::XContainerWindowEventHandler:
	virtual sal_Bool SAL_CALL callHandlerMethod(const uno::Reference<awt::XWindow> & xWindow,
	        const uno::Any & EventObject, const OUString & MethodName)
	        throw (lang::WrappedTargetException, uno::RuntimeException);
	virtual uno::Sequence<OUString> SAL_CALL getSupportedMethodNames()
	        throw (uno::RuntimeException);

	// Static methods
	static inline OUString getImplementationName_static();
	static inline uno::Sequence<OUString> getSupportedServiceNames_static();

	private:

	/** Initialize options window from registry */
	void initOptionsWindowFromRegistry(const uno::Reference<awt::XWindow> & window);

	/** Save options from options window to registry */
	void saveOptionsFromWindowToRegistry(const uno::Reference<awt::XWindow> & window);

	/** Initialize the dropdown box for dictionary variant */
	void initVariantDropdown(const uno::Reference<awt::XControlContainer> & windowContainer);

	/** Initialize list of available dictionary variants */
	void initAvailableVariants();

	/** Get the currently selected dictionary variant from the dropdown box */
	OUString getSelectedVariant(const uno::Reference<awt::XControlContainer> & windowContainer);

	uno::Reference<uno::XComponentContext> compContext;

	uno::Sequence<OUString> dictionaryVariantList;
};

// Static method implementations
inline OUString SettingsEventHandler::getImplementationName_static() {
        return A2OU("org.puimula.ooovoikko.SettingsEventHandlerImplementation");
}
inline uno::Sequence<OUString> SAL_CALL SettingsEventHandler::getSupportedServiceNames_static() {
        uno::Sequence<OUString> snames(1);
        snames.getArray()[0] = A2OU("org.puimula.ooovoikko.SettingsEventHandlerService");
        return snames;
}

}
#endif

