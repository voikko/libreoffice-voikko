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

#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/uno/XInterface.hpp>
#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>
#include <com/sun/star/util/XMacroExpander.hpp>
#include <cppuhelper/bootstrap.hxx>
#include <osl/file.hxx>

#include "common.hxx"

namespace hspell {

osl::Mutex & getVoikkoMutex() {
	static osl::Mutex voikkoMutex;
	return voikkoMutex;
}

OUString getInstallationPath() {
	try {
	uno::Reference<uno::XComponentContext> compContext = cppu::defaultBootstrap_InitialComponentContext();
	uno::Reference<lang::XMultiComponentFactory> servManager = compContext->getServiceManager();
	uno::Reference<uno::XInterface> iFace = servManager->createInstanceWithContext(
		A2OU("com.sun.star.configuration.ConfigurationProvider"), compContext);
	uno::Reference<lang::XMultiServiceFactory> provider(iFace, uno::UNO_QUERY);
	beans::PropertyValue pathArgument(A2OU("nodepath"), 0,
		(uno::Any) A2OU("/org.puimula.ooovoikko.Config/internal"), beans::PropertyState_DIRECT_VALUE);
	uno::Sequence<uno::Any> aArguments(1);
	aArguments.getArray()[0] = (uno::Any) pathArgument;
	VOIKKO_DEBUG("getInstallationPath() - 1");
	uno::Reference<uno::XInterface> rootView = provider->createInstanceWithArguments(
		A2OU("com.sun.star.configuration.ConfigurationAccess"), aArguments);
	VOIKKO_DEBUG("getInstallationPath() - 2");
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	uno::Any locationProp = propSet->getHierarchicalPropertyValue(A2OU("location"));
	OUString locationVal;
	locationProp >>= locationVal;
	// Cut the "vnd.sun.star.expand:" part
	OUString locationFileURL = locationVal.replaceAt(0, 20, A2OU(""));
	uno::Reference<util::XMacroExpander> expander(
		compContext->getValueByName(
		A2OU("/singletons/com.sun.star.util.theMacroExpander")), uno::UNO_QUERY);
	OUString expandedLocation = expander->expandMacros(locationFileURL);
	VOIKKO_DEBUG_2("%s", OU2DEBUG(expandedLocation));
	OUString locationSystemPath;
	osl::FileBase::getSystemPathFromFileURL(expandedLocation, locationSystemPath);
	VOIKKO_DEBUG_2("%s", OU2DEBUG(locationSystemPath));
	return locationSystemPath;
	}
	catch (uno::Exception e) {
	// TODO: something more useful here
	VOIKKO_DEBUG("getInstallationPath(): ERROR");
	return A2OU("");
	}
}


sal_Bool voikko_initialized = sal_False;

struct dict_radix *dict = 0;

uno::Reference<hspell::PropertyManager> thePropertyManager = 0;

}
