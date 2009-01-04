/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 - 2009 Harri Pitkänen <hatapitk@iki.fi>
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
#include <com/sun/star/linguistic2/LinguServiceEventFlags.hpp>
#include <osl/nlsupport.h>
#include <rtl/process.h>
#include <libvoikko/voikko.h>

#include "PropertyManager.hxx"
#include "common.hxx"

namespace voikko {

PropertyManager::PropertyManager(uno::Reference<uno::XComponentContext> cContext):
	compContext(cContext),
	linguEventListeners(getVoikkoMutex()) {
	VOIKKO_DEBUG("PropertyManager created");
	isInitialized = sal_False;
	linguPropSet = 0;
	hyphMinLeading = 2;
	hyphMinTrailing = 2;
	hyphMinWordLength = 5;
	hyphWordParts = sal_False;
}

PropertyManager::~PropertyManager() {
	VOIKKO_DEBUG("PropertyManager: running destructor");
	/* This might need locking, but since the property manager is never destroyed
	 * before the office shutdown, there should be no other sources for calls to
	 * libvoikko at this time. */
	if (voikko_initialized) {
		voikko_terminate(voikko_handle);
		voikko_initialized = sal_False;
	}
}

void SAL_CALL PropertyManager::propertyChange(const beans::PropertyChangeEvent &)
	throw (uno::RuntimeException) {
	// FIXME: property change notifications are not being received
	// VOIKKO_DEBUG_2("PropertyManager::propertyChange: %s", OU2DEBUG(pce.PropertyName));
}

void SAL_CALL PropertyManager::disposing(const lang::EventObject &)
	throw (uno::RuntimeException){
	VOIKKO_DEBUG("PropertyManager::disposing");
}

void PropertyManager::initialize() throw (uno::Exception) {
	VOIKKO_DEBUG("PropertyManager::initialize: starting");
	if (!voikko_initialized) {
		isInitialized = sal_False;
		#ifdef VOIKKO_STANDALONE_EXTENSION
			rtl_TextEncoding encoding = osl_getTextEncodingFromLocale(0);
			if (encoding == RTL_TEXTENCODING_DONTKNOW)
				encoding = RTL_TEXTENCODING_UTF8;
			voikkoErrorString = voikko_init_with_path(&voikko_handle, "fi_FI", 0,
				OUStringToOString(getInstallationPath(compContext), encoding).getStr());
		#else
			voikkoErrorString = voikko_init(&voikko_handle, "fi_FI", 0);
		#endif
		if (voikkoErrorString) {
			VOIKKO_DEBUG_2("Failed to initialize voikko: %s", voikkoErrorString);
			return;
		}
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_DOT, 1);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_NO_UGLY_HYPHENATION, 1);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_TITLES_IN_GC, 1);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_UNFINISHED_PARAGRAPHS_IN_GC, 1);
		
		// Determine UI language
		messageLanguage = "en_US";
		try {
			uno::Any langA = readFromRegistry(
				A2OU("org.openoffice.Office.Linguistic/General"),
				A2OU("UILocale"));
			OUString lang;
			langA >>= lang;
			VOIKKO_DEBUG_2("Specified UI locale = '%s'", OU2DEBUG(lang));
			if (lang.match(A2OU("fi"), 0)) messageLanguage = "fi_FI";
			else if (lang.getLength() == 0) { // Use system default language
				// FIXME: This does not check LC_MESSAGES. There is
				// also GetSystemUILanguage but that cannot be used
				// from extension.
				rtl_Locale * rtlLocale;
				osl_getProcessLocale(&rtlLocale);
				OUString localeLang(rtlLocale->Language);
				VOIKKO_DEBUG_2("Locale language = '%s'", OU2DEBUG(localeLang));
				if (localeLang.match(A2OU("fi"), 0))
					messageLanguage = "fi_FI";
			}
		}
		catch (beans::UnknownPropertyException) { }
		
		voikko_initialized = sal_True;
		VOIKKO_DEBUG("PropertyManager::initialize: libvoikko initalized");
	}
	if (!isInitialized) {
		uno::Reference<lang::XMultiComponentFactory> servManager =
			compContext->getServiceManager();
		uno::Reference<uno::XInterface> LPIFace = servManager->createInstanceWithContext(
			A2OU("com.sun.star.linguistic2.LinguProperties"), compContext);
		linguPropSet = uno::Reference<beans::XPropertySet>(LPIFace, uno::UNO_QUERY);
		linguPropSet->addPropertyChangeListener(A2OU("IsSpellWithDigits"), this);
		linguPropSet->addPropertyChangeListener(A2OU("IsSpellUpperCase"), this);
		linguPropSet->addPropertyChangeListener(A2OU("IsSpellCapitalization"), this);
		VOIKKO_DEBUG("PropertyManager::initialize: property manager initalized");
		isInitialized = sal_True;
	}
	// synchronize the local settings from global preferences
	setProperties(linguPropSet);
	readVoikkoSettings();
	// request that all users of linguistic services run the spellchecker and hyphenator
	// again with updated settings
	linguistic2::LinguServiceEvent event;
	event.nEvent =  linguistic2::LinguServiceEventFlags::SPELL_CORRECT_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::SPELL_WRONG_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::HYPHENATE_AGAIN;
	sendLinguEvent(event);
}


sal_Int16 PropertyManager::getHyphMinLeading() { return hyphMinLeading; };
sal_Int16 PropertyManager::getHyphMinTrailing() { return hyphMinTrailing; };
sal_Int16 PropertyManager::getHyphMinWordLength() { return hyphMinWordLength; };

sal_Bool PropertyManager::addLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	VOIKKO_DEBUG("PropertyManager::addLinguServiceEventListener");
	if (xLstnr.is()) {
		sal_Int32 listenerCount = linguEventListeners.getLength();
		return (linguEventListeners.addInterface(xLstnr) != listenerCount);
	}
	else return sal_False;
}

sal_Bool PropertyManager::removeLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	VOIKKO_DEBUG("PropertyManager::removeLinguServiceEventListener");
	if (xLstnr.is()) {
		sal_Int32 listenerCount = linguEventListeners.getLength();
		return (linguEventListeners.removeInterface(xLstnr) != listenerCount);
	}
	else return sal_False;
}

void PropertyManager::setHyphWordParts(sal_Bool value) {
	hyphWordParts = value;
	syncHyphenatorSettings();
}

uno::Any PropertyManager::readFromRegistry(const OUString group, const OUString & key)
	throw (beans::UnknownPropertyException) {
	uno::Reference<uno::XInterface> rootView =
		getRegistryProperties(group, compContext);
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	if (!propSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain propSet");
		throw new beans::UnknownPropertyException();
	}
	uno::Any value = propSet->getHierarchicalPropertyValue(key);
	return value;
}

void PropertyManager::readVoikkoSettings() {
	try {
		uno::Any hyphWordParts = readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphWordParts"));
		hyphWordParts >>= this->hyphWordParts;
		
		uno::Any hyphUnknownWords = readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphUnknownWords"));
		hyphUnknownWords >>= this->hyphUnknownWords;
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("ERROR: readVoikkoSettings: UnknownPropertyException");
	}
	syncHyphenatorSettings();
}

OUString PropertyManager::getInitializationStatus() {
	if (isInitialized) return A2OU("OK");
	else return A2OU(voikkoErrorString);
}

const char * PropertyManager::getMessageLanguage() {
	return messageLanguage;
}

void PropertyManager::setProperties(const uno::Reference<beans::XPropertySet> & properties) {
	beans::PropertyValue pValue;
	uno::Sequence<beans::Property> pSeq = properties->getPropertySetInfo()->getProperties();
	for (sal_Int32 i = 0; i < pSeq.getLength(); i++) {
		pValue.Name = pSeq[i].Name;
		pValue.Value = properties->getPropertyValue(pSeq[i].Name);
		setValue(pValue);
	}
}

void PropertyManager::setValues(const uno::Sequence<beans::PropertyValue> & values) {
	for (sal_Int32 i = 0; i < values.getLength(); i++) setValue(values[i]);
}

void PropertyManager::resetValues(const uno::Sequence<beans::PropertyValue> & values) {
	beans::PropertyValue globalValue;
	for (sal_Int32 i = 0; i < values.getLength(); i++) {
		globalValue.Name = values[i].Name;
		globalValue.Value = linguPropSet->getPropertyValue(values[i].Name);
		setValue(globalValue);
	}
}

void PropertyManager::setValue(const beans::PropertyValue & value) {
	sal_Bool bValue = sal_False;
	sal_Int16 iValue = 0;
	int vbValue = 0;
	// VOIKKO_DEBUG_2("PropertyManager::setValue: name %s", OU2DEBUG(value.Name));
	if (value.Name == A2OU("IsSpellWithDigits")) {
		value.Value >>= bValue;
		if (!bValue) vbValue = 1;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, vbValue);
	}
	else if (value.Name == A2OU("IsSpellUpperCase")) {
		value.Value >>= bValue;
		if (!bValue) vbValue = 1;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, vbValue);
	}
	else if (value.Name == A2OU("IsSpellCapitalization")) {
		// FIXME: should ignore ALL errors in capitalization
		value.Value >>= bValue;
		if (!bValue) vbValue = 1;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, vbValue);
	}
	else if (value.Name == A2OU("HyphMinLeading")) {
		if (value.Value >>= iValue) {
			hyphMinLeading = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
	else if (value.Name == A2OU("HyphMinTrailing")) {
		if (value.Value >>= iValue) {
			hyphMinTrailing = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
	else if (value.Name == A2OU("HyphMinWordLength")) {
		if (value.Value >>= iValue) {
			hyphMinWordLength = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
}

inline void PropertyManager::syncHyphenatorSettings() {
	if (hyphWordParts) {
		voikko_set_int_option(voikko_handle, VOIKKO_MIN_HYPHENATED_WORD_LENGTH,
		                      hyphMinWordLength);
	}
	else {
		voikko_set_int_option(voikko_handle, VOIKKO_MIN_HYPHENATED_WORD_LENGTH, 2);
	}
	
	voikko_set_bool_option(voikko_handle, VOIKKO_OPT_HYPHENATE_UNKNOWN_WORDS, hyphUnknownWords);
}

void PropertyManager::sendLinguEvent(const linguistic2::LinguServiceEvent & event) {
	cppu::OInterfaceIteratorHelper iterator(linguEventListeners);
	while (iterator.hasMoreElements()) {
		uno::Reference<linguistic2::XLinguServiceEventListener>
			ref(iterator.next(), uno::UNO_QUERY);
		if (ref.is()) ref->processLinguServiceEvent(event);
	}
}

}
