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
#include <com/sun/star/awt/XControl.hpp>
#include <com/sun/star/awt/XControlContainer.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>
#include <com/sun/star/util/XChangesBatch.hpp>

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
	if (MethodName != A2OU("external_event")) return sal_False;
	OUString eventS;
	EventObject >>= eventS;
	if (eventS == A2OU("ok")) {
		saveOptionsFromWindowToRegistry(xWindow);
		return sal_True;
	}
	if (eventS == A2OU("back") || eventS == A2OU("initialize")) {
		initOptionsWindowFromRegistry(xWindow);
		return sal_True;
	}
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL SettingsEventHandler::getSupportedMethodNames()
	throw (uno::RuntimeException) {
	uno::Sequence<OUString> methodNames(1);
	methodNames.getArray()[0] = A2OU("external_event");
	return methodNames;
}

uno::Reference<uno::XInterface> SettingsEventHandler::getRegistryProperties(const OUString & group) {
	VOIKKO_DEBUG("getRegistryProperties");
	uno::Reference<uno::XInterface> rootView;
	uno::Reference<lang::XMultiComponentFactory> servManager = compContext->getServiceManager();
	if (!servManager.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain servManager");
		return rootView;
	}
	uno::Reference<uno::XInterface> iFace = servManager->createInstanceWithContext(
		A2OU("com.sun.star.configuration.ConfigurationProvider"), compContext);
	if (!iFace.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain iFace");
		return rootView;
	}
	uno::Reference<lang::XMultiServiceFactory> provider(iFace, uno::UNO_QUERY);
	if (!provider.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain provider");
		return rootView;
	}
	beans::PropertyValue pathArgument(A2OU("nodepath"), 0, (uno::Any) group,
		beans::PropertyState_DIRECT_VALUE);
	uno::Sequence<uno::Any> aArguments(1);
	aArguments.getArray()[0] = (uno::Any) pathArgument;
	rootView = provider->createInstanceWithArguments(
		A2OU("com.sun.star.configuration.ConfigurationUpdateAccess"), aArguments);
	if (!rootView.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain rootView");
	}
	return rootView;
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	return propSet;
}

void SettingsEventHandler::initOptionsWindowFromRegistry(const uno::Reference<awt::XWindow> & window) {
	VOIKKO_DEBUG("initOptionsWindowFromRegistry()");
	uno::Reference<uno::XInterface> rootView =
		getRegistryProperties(A2OU("/org.puimula.ooovoikko.Config/hyphenator"));
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	if (!propSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain propSet");
		return;
	}
	sal_Bool hyphWordPartsValue = sal_False;
	try {
		uno::Any hyphWordPartsAValue = propSet->getHierarchicalPropertyValue(A2OU("hyphWordParts"));
		hyphWordPartsAValue >>= hyphWordPartsValue;
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("ERROR: UnknownPropertyException");
		return;
	}
	if (hyphWordPartsValue) VOIKKO_DEBUG("hyphWordParts = true");
	else VOIKKO_DEBUG("hyphWordParts = false");

	uno::Reference<awt::XControlContainer> windowContainer =
		uno::Reference<awt::XControlContainer>(window, uno::UNO_QUERY);
	if (!windowContainer.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain windowContainer");
		return;
	}
	uno::Reference<awt::XControl> hyphWordParts = windowContainer->getControl(A2OU("hyphWordParts"));
	if (!hyphWordParts.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphWordParts");
		return;
	}
	uno::Reference<beans::XPropertySet> hyphWordPartsProps =
		uno::Reference<beans::XPropertySet>(hyphWordParts->getModel(), uno::UNO_QUERY);
	if (!hyphWordPartsProps.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphWordPartsProps");
		return;
	}
	uno::Any hyphWordPartsAValue;
	if (hyphWordPartsValue) hyphWordPartsAValue <<= (sal_Int16) 1;
	else hyphWordPartsAValue <<= (sal_Int16) 0;
	hyphWordPartsProps->setPropertyValue(A2OU("State"), hyphWordPartsAValue);
}

void SettingsEventHandler::saveOptionsFromWindowToRegistry(const uno::Reference<awt::XWindow> & window) {
	VOIKKO_DEBUG("saveOptionsFromWindowToRegistry()");
	if (!window.is()) {
		VOIKKO_DEBUG("ERROR: window is null");
		return;
	}
	uno::Reference<awt::XControlContainer> windowContainer =
		uno::Reference<awt::XControlContainer>(window, uno::UNO_QUERY);
	if (!windowContainer.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain windowContainer");
		return;
        }
	uno::Reference<awt::XControl> hyphWordParts = windowContainer->getControl(A2OU("hyphWordParts"));
	if (!hyphWordParts.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphWordParts");
		return;
	}
	uno::Reference<beans::XPropertySet> hyphWordPartsProps =
		uno::Reference<beans::XPropertySet>(hyphWordParts->getModel(), uno::UNO_QUERY);
	if (!hyphWordPartsProps.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphWordPartsProps");
		return;
	}
	uno::Any hyphWordPartsAValue = hyphWordPartsProps->getPropertyValue(A2OU("State"));
	sal_Int16 hyphWordPartsValue = 0;
	hyphWordPartsAValue >>= hyphWordPartsValue; // 0 = unchecked, 1 = checked

	uno::Reference<uno::XInterface> rootView =
		getRegistryProperties(A2OU("/org.puimula.ooovoikko.Config/hyphenator"));
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	if (!propSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain propSet");
		return;
	}
	hyphWordPartsAValue <<= (hyphWordPartsValue == 1 ? sal_True : sal_False);
	propSet->setHierarchicalPropertyValue(A2OU("hyphWordParts"), hyphWordPartsAValue);
	uno::Reference<util::XChangesBatch> updateCommit(rootView, uno::UNO_QUERY);
	if (!updateCommit.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain updateCommit");
		return;
	}
	updateCommit->commitChanges();
}

}

