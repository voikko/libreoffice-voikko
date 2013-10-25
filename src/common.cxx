/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2007 - 2010 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/uno/XInterface.hpp>
#include <cppuhelper/bootstrap.hxx>

#include "common.hxx"
#include "macros.hxx"

using namespace rtl;

namespace voikko {

osl::Mutex & getVoikkoMutex() {
	static osl::Mutex voikkoMutex;
	return voikkoMutex;
}


uno::Reference<uno::XInterface> getRegistryProperties(const OUString & group,
	uno::Reference<uno::XComponentContext> compContext) {
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
	try {
		// FIXME: this may crash if configuration is damaged
		rootView = provider->createInstanceWithArguments(
			A2OU("com.sun.star.configuration.ConfigurationUpdateAccess"), aArguments);
	}
	catch (uno::Exception e) {
		VOIKKO_DEBUG_2("ERROR: exception while trying to obtain rootView for '%s'", OU2DEBUG(group));
		return rootView;
	}
	if (!rootView.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain rootView");
	}
	return rootView;
}

}
