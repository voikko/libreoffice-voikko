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

#include "SettingsEventHandler.hxx"

namespace voikko {

SettingsEventHandler::SettingsEventHandler(uno::Reference<uno::XComponentContext> const & context) :
        cppu::WeakComponentImplHelper2
             <lang::XServiceInfo,
              awt::XContainerWindowEventHandler>(m_aMutex),
        compContext(context) { }

OUString SAL_CALL SettingsEventHandler::getImplementationName() throw (uno::RuntimeException) {
	return getImplementationName_static();
}

sal_Bool SAL_CALL SettingsEventHandler::supportsService(const OUString & serviceName)
	throw (uno::RuntimeException) {
	uno::Sequence<OUString> serviceNames = getSupportedServiceNames();
	for (sal_Int32 i = 0; i < serviceNames.getLength(); i++)
		if (serviceNames[i] == serviceName) return sal_True;
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL SettingsEventHandler::getSupportedServiceNames() throw (uno::RuntimeException) {
	return getSupportedServiceNames_static();
}

sal_Bool SAL_CALL SettingsEventHandler::callHandlerMethod(const uno::Reference<awt::XWindow> & xWindow,
	const uno::Any & EventObject, const OUString & MethodName)
	throw (lang::WrappedTargetException, uno::RuntimeException) {
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL SettingsEventHandler::getSupportedMethodNames()
	throw (uno::RuntimeException) {
	uno::Sequence<OUString> methodNames(1);
	methodNames.getArray()[0] = A2OU("external_event");
	return methodNames;
}

}

