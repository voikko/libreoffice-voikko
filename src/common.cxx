/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 - 2008 Harri Pitkänen <hatapitk@iki.fi>
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

#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/uno/XInterface.hpp>
#include <com/sun/star/deployment/XPackageInformationProvider.hpp>
#include <cppuhelper/bootstrap.hxx>
#include <osl/file.hxx>

#include "common.hxx"

namespace voikko {

osl::Mutex & getVoikkoMutex() {
	static osl::Mutex voikkoMutex;
	return voikkoMutex;
}

OUString getInstallationPath() {
	try {
	uno::Reference<uno::XComponentContext> compContext = cppu::defaultBootstrap_InitialComponentContext();
	uno::Reference<lang::XMultiComponentFactory> servManager = compContext->getServiceManager();
	uno::Reference<uno::XInterface> iFace = servManager->createInstanceWithContext(
		A2OU("com.sun.star.deployment.PackageInformationProvider"), compContext);
	uno::Reference<deployment::XPackageInformationProvider> provider(iFace, uno::UNO_QUERY);
	OUString locationFileURL = provider->getPackageLocation(A2OU("org.puimula.ooovoikko"));
	OUString locationSystemPath;
	osl::FileBase::getSystemPathFromFileURL(locationFileURL, locationSystemPath);
	VOIKKO_DEBUG_2("%s", OU2DEBUG(locationSystemPath));
	return locationSystemPath;
	}
	catch (uno::Exception e) {
	// TODO: something more useful here
	VOIKKO_DEBUG("getInstallationPath(): ERROR");
	return A2OU("");
	}
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
	rootView = provider->createInstanceWithArguments(
		A2OU("com.sun.star.configuration.ConfigurationUpdateAccess"), aArguments);
	if (!rootView.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain rootView");
	}
	return rootView;
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	return propSet;
}

sal_Bool voikko_initialized = sal_False;

int voikko_handle = 0;

uno::Reference<voikko::PropertyManager> thePropertyManager = 0;

}
