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

#include "SettingsEventHandler.hxx"
#include "../common.hxx"
#include <libvoikko/voikko.h>
#include <osl/nlsupport.h>
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
        compContext(context),
        dictionaryVariantList(1) {
	VOIKKO_DEBUG("SettingsEventHandler:CTOR");
	dictionaryVariantList.getArray()[0] = A2OU("standard: suomen kielen perussanasto");
}

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
		PropertyManager::get(compContext)->reloadVoikkoSettings();
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

void SettingsEventHandler::initOptionsWindowFromRegistry(const uno::Reference<awt::XWindow> & window) {
	VOIKKO_DEBUG("initOptionsWindowFromRegistry()");
	sal_Bool hyphWordPartsValue = sal_False;
	sal_Bool hyphUnknownWordsValue = sal_True;
	uno::Any hyphWordPartsAValue;
	uno::Any hyphUnknownWordsAValue;
	try {
		hyphWordPartsAValue = PropertyManager::get(compContext)->readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphWordParts"));
		hyphWordPartsAValue >>= hyphWordPartsValue;
		
		hyphUnknownWordsAValue = PropertyManager::get(compContext)->readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphUnknownWords"));
		hyphUnknownWordsAValue >>= hyphUnknownWordsValue;
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("ERROR: UnknownPropertyException");
		return;
	}
	if (hyphWordPartsValue) {
		VOIKKO_DEBUG("hyphWordParts = true");
	}
	else {
		VOIKKO_DEBUG("hyphWordParts = false");
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
	if (hyphWordPartsValue) hyphWordPartsAValue <<= (sal_Int16) 1;
	else hyphWordPartsAValue <<= (sal_Int16) 0;
	hyphWordPartsProps->setPropertyValue(A2OU("State"), hyphWordPartsAValue);
	
	uno::Reference<awt::XControl> hyphUnknownWords = windowContainer->getControl(A2OU("hyphUnknownWords"));
	if (!hyphUnknownWords.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphUnknownWords");
		return;
	}
	uno::Reference<beans::XPropertySet> hyphUnknownWordsProps =
		uno::Reference<beans::XPropertySet>(hyphUnknownWords->getModel(), uno::UNO_QUERY);
	if (!hyphUnknownWordsProps.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphUnknownWordsProps");
		return;
	}
	if (hyphUnknownWordsValue) hyphUnknownWordsAValue <<= (sal_Int16) 1;
	else hyphUnknownWordsAValue <<= (sal_Int16) 0;
	hyphUnknownWordsProps->setPropertyValue(A2OU("State"), hyphUnknownWordsAValue);
	
	initVariantDropdown(windowContainer);
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
	
	uno::Reference<awt::XControl> hyphUnknownWords = windowContainer->getControl(A2OU("hyphUnknownWords"));
	if (!hyphUnknownWords.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphUnknownWords");
		return;
	}
	uno::Reference<beans::XPropertySet> hyphUnknownWordsProps =
		uno::Reference<beans::XPropertySet>(hyphUnknownWords->getModel(), uno::UNO_QUERY);
	if (!hyphUnknownWordsProps.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain hyphUnknownWordsProps");
		return;
	}
	uno::Any hyphUnknownWordsAValue = hyphUnknownWordsProps->getPropertyValue(A2OU("State"));
	sal_Int16 hyphUnknownWordsValue = 0;
	hyphUnknownWordsAValue >>= hyphUnknownWordsValue; // 0 = unchecked, 1 = checked
	
	uno::Reference<uno::XInterface> rootView =
		getRegistryProperties(A2OU("/org.puimula.ooovoikko.Config/hyphenator"), compContext);
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	if (!propSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain propSet");
		return;
	}
	
	hyphWordPartsAValue <<= (hyphWordPartsValue == 1 ? sal_True : sal_False);
	propSet->setHierarchicalPropertyValue(A2OU("hyphWordParts"), hyphWordPartsAValue);
	
	hyphUnknownWordsAValue <<= (hyphUnknownWordsValue == 1 ? sal_True : sal_False);
	propSet->setHierarchicalPropertyValue(A2OU("hyphUnknownWords"), hyphUnknownWordsAValue);
	
	uno::Reference<util::XChangesBatch> updateCommit(rootView, uno::UNO_QUERY);
	if (!updateCommit.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain updateCommit");
		return;
	}
	updateCommit->commitChanges();
	
	// dictionary variant
	OUString variantValue = getSelectedVariant(windowContainer);
	uno::Any variantAValue;
	variantAValue <<= variantValue;
	rootView = getRegistryProperties(A2OU("/org.puimula.ooovoikko.Config/dictionary"), compContext);
	uno::Reference<beans::XHierarchicalPropertySet> variantPropSet(rootView, uno::UNO_QUERY);
	if (!variantPropSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variantPropSet");
		return;
	}
	variantPropSet->setHierarchicalPropertyValue(A2OU("variant"), variantAValue);
	uno::Reference<util::XChangesBatch> variantUpdateCommit(rootView, uno::UNO_QUERY);
	if (!variantUpdateCommit.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variantUpdateCommit");
		return;
	}
	variantUpdateCommit->commitChanges();
}

void SettingsEventHandler::initVariantDropdown(const uno::Reference<awt::XControlContainer> & windowContainer) {
	uno::Reference<awt::XControl> variantDropdown = windowContainer->getControl(A2OU("variant"));
	if (!variantDropdown.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variant dropdown control");
		return;
	}
	uno::Reference<beans::XPropertySet> variantProps =
		uno::Reference<beans::XPropertySet>(variantDropdown->getModel(), uno::UNO_QUERY);
	if (!variantDropdown.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variantProps");
		return;
	}
	
	// populate dropdown list with available variants
	initAvailableVariants();
	uno::Any stringListAValue;
	stringListAValue <<= dictionaryVariantList;
	variantProps->setPropertyValue(A2OU("StringItemList"), stringListAValue);
	
	// read selected dictionary variant from registry
	OUString registryVariantValue(A2OU("standard"));
	try {
		uno::Any registryVariantAValue =
			PropertyManager::get(compContext)->readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/dictionary"),
			A2OU("variant"));
		registryVariantAValue >>= registryVariantValue;
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("ERROR: UnknownPropertyException dictionary/variant");
		return;
	}
	registryVariantValue += A2OU(": ");
	uno::Sequence<sal_Int16> selectedValues(1);
	selectedValues[0] = 0;
	for (sal_Int16 i = 0; i < dictionaryVariantList.getLength(); i++) {
		if (dictionaryVariantList[i].indexOf(registryVariantValue) == 0) {
			selectedValues[0] = i;
			break;
		}
	}
	
	// set the selected item in the dropdown list
	uno::Any selectedAValues;
	selectedAValues <<= selectedValues;
	variantProps->setPropertyValue(A2OU("SelectedItems"), selectedAValues);
}

void SettingsEventHandler::initAvailableVariants() {
	voikko_dict ** dicts;
	#ifdef VOIKKO_STANDALONE_EXTENSION
		rtl_TextEncoding encoding = osl_getTextEncodingFromLocale(0);
		if (encoding == RTL_TEXTENCODING_DONTKNOW) {
			encoding = RTL_TEXTENCODING_UTF8;
		}
		dicts = voikko_list_dicts(OUStringToOString(getInstallationPath(compContext), encoding).getStr());
	#else
		dicts = voikko_list_dicts(0);
	#endif
	
	if (!dicts) {
		VOIKKO_DEBUG("ERROR: Failed to list available dictionaries");
		return;
	}
	
	size_t nDicts = 0;
	for (voikko_dict ** i = dicts; *i; i++) {
		nDicts++;
	}
	dictionaryVariantList = uno::Sequence<OUString>(nDicts);
	for (size_t i = 0; i < nDicts; i++) {
		OUString dictName = A2OU(voikko_dict_variant(dicts[i]));
		dictName += A2OU(": ");
		dictName += UTF82OU(voikko_dict_description(dicts[i]));
		dictionaryVariantList[i] = dictName;
	}
	voikko_free_dicts(dicts);
}

OUString SettingsEventHandler::getSelectedVariant(
		const uno::Reference<awt::XControlContainer> & windowContainer) {
	uno::Reference<awt::XControl> variantDropdown = windowContainer->getControl(A2OU("variant"));
	if (!variantDropdown.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variant dropdown control");
		return A2OU("standard");
	}
	uno::Reference<beans::XPropertySet> variantProps =
		uno::Reference<beans::XPropertySet>(variantDropdown->getModel(), uno::UNO_QUERY);
	if (!variantDropdown.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain variantProps");
		return A2OU("standard");
	}
	
	// get all values
	uno::Any stringListAValue = variantProps->getPropertyValue(A2OU("StringItemList"));
	uno::Sequence<OUString> stringListValue;
	stringListAValue >>= stringListValue;
	
	// get the selected item index
	uno::Any selectedAValues = variantProps->getPropertyValue(A2OU("SelectedItems"));
	uno::Sequence<sal_Int16> selectedValues(1);
	selectedAValues >>= selectedValues;
	
	// parse the variant id from the string
	OUString selectedValue = stringListValue[selectedValues[0]];
	sal_Int32 variantIdEnd = selectedValue.indexOf(A2OU(":"));
	if (variantIdEnd != -1) {
		return selectedValue.copy(0, variantIdEnd);
	}
	VOIKKO_DEBUG("ERROR: failed to get the selected variant, returning default");
	return A2OU("standard");
}

}

