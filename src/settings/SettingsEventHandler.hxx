/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2008 - 2009 Harri Pitkänen <hatapitk@iki.fi>
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

	/** Initialize list of available dictionary variants */
	void initAvailableVariants(int voikkoHandle);

	// Static methods
	static inline OUString getImplementationName_static();
	static inline uno::Sequence<OUString> getSupportedServiceNames_static();

	private:

	/** Initialize options window from registry */
	void initOptionsWindowFromRegistry(const uno::Reference<awt::XWindow> & window);

	/** Save options from options window to registry */
	void saveOptionsFromWindowToRegistry(const uno::Reference<awt::XWindow> & window);

	/** Initialize the dropdown box for dictionary variant */
	void initVariantDropdown(uno::Reference<awt::XControlContainer> windowContainer);

	uno::Reference<uno::XComponentContext> compContext;

	uno::Sequence<OUString> dictionaryVariantList;
};

// Static method implementations
inline OUString SettingsEventHandler::getImplementationName_static() {
        return A2OU("org.puimula.ooovoikko.SettingsEventHandlerImplementation");
}
inline uno::Sequence<OUString> SettingsEventHandler::getSupportedServiceNames_static() {
        uno::Sequence<OUString> snames(1);
        snames.getArray()[0] = A2OU("org.puimula.ooovoikko.SettingsEventHandlerService");
        return snames;
}

}
#endif

