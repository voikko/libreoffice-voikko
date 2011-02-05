/* Libreoffice-voikko: Finnish linguistic extension for LibreOffice
 * Copyright (C) 2007 - 2010 Harri Pitkänen <hatapitk@iki.fi>
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
#include <com/sun/star/deployment/PackageInformationProvider.hpp>
#include <com/sun/star/deployment/XPackageInformationProvider.hpp>
#include <osl/file.hxx>
#include <osl/nlsupport.h>
#include <rtl/process.h>
#include <libvoikko/voikko.h>

#include "PropertyManager.hxx"
#include "VoikkoHandlePool.hxx"
#include "common.hxx"

namespace voikko {

#ifdef VOIKKO_STANDALONE_EXTENSION
static OUString getInstallationPath(uno::Reference<uno::XComponentContext> & compContext) {
	try {
		VOIKKO_DEBUG("getInstallationPath");
		uno::Reference<deployment::XPackageInformationProvider> provider(deployment::PackageInformationProvider::get(compContext));
		OUString locationFileURL = provider->getPackageLocation(A2OU("org.puimula.ooovoikko"));
		VOIKKO_DEBUG_2("%s", OU2DEBUG(locationFileURL));
		OUString locationSystemPath;
		osl::FileBase::getSystemPathFromFileURL(locationFileURL, locationSystemPath);
		VOIKKO_DEBUG_2("%s", OU2DEBUG(locationSystemPath));
		return locationSystemPath;
	}
	catch (uno::Exception e) {
		// TODO: something more useful here
		VOIKKO_DEBUG("getInstallationPath(): ERROR");
		return OUString();
	}
}
#endif

PropertyManager::PropertyManager(uno::Reference<uno::XComponentContext> cContext):
	compContext(cContext),
	linguEventListeners(getVoikkoMutex()),
	messageLanguage("en_US") {
	#ifdef VOIKKO_STANDALONE_EXTENSION
		rtl_TextEncoding encoding = osl_getTextEncodingFromLocale(0);
		if (encoding == RTL_TEXTENCODING_DONTKNOW) {
			encoding = RTL_TEXTENCODING_UTF8;
		}
		VoikkoHandlePool::getInstance()->setInstallationPath(OUStringToOString(getInstallationPath(cContext), encoding));
	#endif
	VOIKKO_DEBUG("PropertyManager:CTOR");
	linguPropSet = 0;
	hyphMinLeading = 2;
	hyphMinTrailing = 2;
	hyphMinWordLength = 5;
	hyphWordParts = sal_False;
	hyphUnknownWords = sal_True;
	try {
		uno::Any dictVariantA = readFromRegistry(
		              A2OU("/org.puimula.ooovoikko.Config/dictionary"),
		              A2OU("variant"));
		OUString dictVariant;
		dictVariantA >>= dictVariant;
		VoikkoHandlePool::getInstance()->setPreferredGlobalVariant(dictVariant);
		VOIKKO_DEBUG_2("Initial dictionary variant '%s'", OU2DEBUG(dictVariant));
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("Setting initial dictionary variant to default");
		VoikkoHandlePool::getInstance()->setPreferredGlobalVariant(OUString());
	}
	initialize();
}

PropertyManager::~PropertyManager() {
	VOIKKO_DEBUG("PropertyManager:DTOR");
	/* This might need locking, but since the property manager is never destroyed
	 * before the office shutdown, there should be no other sources for calls to
	 * libvoikko at this time. */
	VoikkoHandlePool::getInstance()->closeAllHandles();
}

void SAL_CALL PropertyManager::propertyChange(const beans::PropertyChangeEvent & /*pce*/)
	throw (uno::RuntimeException) {
	VOIKKO_DEBUG("PropertyManager::propertyChange");
	setProperties(linguPropSet);
	linguistic2::LinguServiceEvent event;
	event.nEvent =  linguistic2::LinguServiceEventFlags::SPELL_CORRECT_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::SPELL_WRONG_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::HYPHENATE_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::PROOFREAD_AGAIN;
	sendLinguEvent(event);
}

void SAL_CALL PropertyManager::disposing(const lang::EventObject &)
	throw (uno::RuntimeException){
}

void PropertyManager::setUiLanguage() {
	try {
		uno::Any langA = readFromRegistry(
			A2OU("org.openoffice.Office.Linguistic/General"),
			A2OU("UILocale"));
		OUString lang;
		langA >>= lang;
		VOIKKO_DEBUG_2("Specified UI locale = '%s'", OU2DEBUG(lang));
		if (lang.match(A2OU("fi"), 0)) {
			messageLanguage = "fi_FI";
		} else if (lang.getLength() == 0) { // Use system default language
			// FIXME: This does not check LC_MESSAGES. There is
			// also GetSystemUILanguage but that cannot be used
			// from extension.
			rtl_Locale * rtlLocale;
			osl_getProcessLocale(&rtlLocale);
			OUString localeLang(rtlLocale->Language);
			VOIKKO_DEBUG_2("Locale language = '%s'", OU2DEBUG(localeLang));
			if (localeLang.match(A2OU("fi"), 0)) {
				messageLanguage = "fi_FI";
			}
		}
	}
	catch (beans::UnknownPropertyException) {
		VOIKKO_DEBUG("ERROR: PropertyManager::initialize caught UnknownPropertyException");
	}
}

void PropertyManager::initialize() throw (uno::Exception) {
	VOIKKO_DEBUG("PropertyManager::initialize: starting");
	setUiLanguage();
	
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_IGNORE_DOT, true);
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_NO_UGLY_HYPHENATION, true);
	
	// Set these options globally until OOo bug #97945 is resolved.
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_ACCEPT_TITLES_IN_GC, true);
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_ACCEPT_BULLETED_LISTS_IN_GC, true);
	
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_ACCEPT_UNFINISHED_PARAGRAPHS_IN_GC, true);
	
	uno::Reference<lang::XMultiComponentFactory> servManager =
		compContext->getServiceManager();
	uno::Reference<uno::XInterface> LPIFace = servManager->createInstanceWithContext(
		A2OU("com.sun.star.linguistic2.LinguProperties"), compContext);
	linguPropSet = uno::Reference<beans::XPropertySet>(LPIFace, uno::UNO_QUERY);
	linguPropSet->addPropertyChangeListener(A2OU("IsSpellWithDigits"), this);
	linguPropSet->addPropertyChangeListener(A2OU("IsSpellUpperCase"), this);
	linguPropSet->addPropertyChangeListener(A2OU("IsSpellCapitalization"), this);
	VOIKKO_DEBUG("PropertyManager::initialize: property manager initalized");
	
	// synchronize the local settings from global preferences
	setProperties(linguPropSet);
	readVoikkoSettings();
	// request that all users of linguistic services run the spellchecker and hyphenator
	// again with updated settings
	linguistic2::LinguServiceEvent event;
	event.nEvent =  linguistic2::LinguServiceEventFlags::SPELL_CORRECT_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::SPELL_WRONG_WORDS_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::HYPHENATE_AGAIN;
	event.nEvent |= linguistic2::LinguServiceEventFlags::PROOFREAD_AGAIN;
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
	if (!rootView.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain rootView");
		throw beans::UnknownPropertyException();
	}
	uno::Reference<beans::XHierarchicalPropertySet> propSet(rootView, uno::UNO_QUERY);
	if (!propSet.is()) {
		VOIKKO_DEBUG("ERROR: failed to obtain propSet");
		throw beans::UnknownPropertyException();
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

const char * PropertyManager::getMessageLanguage() {
	return messageLanguage;
}

void PropertyManager::reloadVoikkoSettings() {
	VoikkoHandlePool * pool = VoikkoHandlePool::getInstance();
	linguistic2::LinguServiceEvent event;
	event.nEvent = 0;
	try {
		uno::Any hyphWordParts = readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphWordParts"));
		sal_Bool hyphWordPartsNew = this->hyphWordParts;
		hyphWordParts >>= hyphWordPartsNew;
		if (hyphWordPartsNew != this->hyphWordParts) {
			event.nEvent |= linguistic2::LinguServiceEventFlags::HYPHENATE_AGAIN;
			this->hyphWordParts = hyphWordPartsNew;
		}
		
		uno::Any hyphUnknownWords = readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/hyphenator"),
			A2OU("hyphUnknownWords"));
		sal_Bool hyphUnknownWordsNew = this->hyphUnknownWords;
		hyphUnknownWords >>= hyphUnknownWordsNew;
		if (hyphUnknownWordsNew != this->hyphUnknownWords) {
			event.nEvent |= linguistic2::LinguServiceEventFlags::HYPHENATE_AGAIN;
			this->hyphUnknownWords = hyphUnknownWordsNew;
		}
		
		uno::Any dictVariantA = readFromRegistry(
			A2OU("/org.puimula.ooovoikko.Config/dictionary"),
			A2OU("variant"));
		OUString dictVariantNew = pool->getPreferredGlobalVariant();
		dictVariantA >>= dictVariantNew;
		if (dictVariantNew != pool->getPreferredGlobalVariant()) {
			event.nEvent |= linguistic2::LinguServiceEventFlags::SPELL_CORRECT_WORDS_AGAIN;
			event.nEvent |= linguistic2::LinguServiceEventFlags::SPELL_WRONG_WORDS_AGAIN;
			event.nEvent |= linguistic2::LinguServiceEventFlags::PROOFREAD_AGAIN;
			pool->setPreferredGlobalVariant(dictVariantNew);
		}
	}
	catch (beans::UnknownPropertyException e) {
		VOIKKO_DEBUG("ERROR: PropertyManager::reloadVoikkoSettings: UnknownPropertyException");
	}
	syncHyphenatorSettings();
	sendLinguEvent(event);
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
	// VOIKKO_DEBUG_2("PropertyManager::setValue: name %s", OU2DEBUG(value.Name));
	if (value.Name == A2OU("IsSpellWithDigits")) {
		value.Value >>= bValue;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_IGNORE_NUMBERS, !bValue);
	}
	else if (value.Name == A2OU("IsSpellUpperCase")) {
		value.Value >>= bValue;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_IGNORE_UPPERCASE, !bValue);
	}
	else if (value.Name == A2OU("IsSpellCapitalization")) {
		// FIXME: should ignore ALL errors in capitalization
		value.Value >>= bValue;
		// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", vbValue);
		VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, bValue);
	}
	else if (value.Name == A2OU("HyphMinLeading")) {
		sal_Int16 iValue;
		if (value.Value >>= iValue) {
			hyphMinLeading = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
	else if (value.Name == A2OU("HyphMinTrailing")) {
		sal_Int16 iValue;
		if (value.Value >>= iValue) {
			hyphMinTrailing = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
	else if (value.Name == A2OU("HyphMinWordLength")) {
		sal_Int16 iValue;
		if (value.Value >>= iValue) {
			hyphMinWordLength = iValue;
			// VOIKKO_DEBUG_2("PropertyManager::setValue: value %i", iValue);
			syncHyphenatorSettings();
		}
	}
}

void PropertyManager::syncHyphenatorSettings() {
	if (hyphWordParts) {
		VoikkoHandlePool::getInstance()->setGlobalIntegerOption(VOIKKO_MIN_HYPHENATED_WORD_LENGTH,
		                      hyphMinWordLength);
	}
	else {
		VoikkoHandlePool::getInstance()->setGlobalIntegerOption(VOIKKO_MIN_HYPHENATED_WORD_LENGTH, 2);
	}
	
	VoikkoHandlePool::getInstance()->setGlobalBooleanOption(VOIKKO_OPT_HYPHENATE_UNKNOWN_WORDS, hyphUnknownWords);
}

void PropertyManager::sendLinguEvent(const linguistic2::LinguServiceEvent & event) {
	VOIKKO_DEBUG("PropertyManager::sendLinguEvent");
	cppu::OInterfaceIteratorHelper iterator(linguEventListeners);
	while (iterator.hasMoreElements()) {
		uno::Reference<linguistic2::XLinguServiceEventListener>
			ref(iterator.next(), uno::UNO_QUERY);
		if (ref.is()) {
			VOIKKO_DEBUG("PropertyManager::sendLinguEvent sending event");
			ref->processLinguServiceEvent(event);
		}
	}
}

static uno::Reference<voikko::PropertyManager> thePropertyManager = 0;

uno::Reference<voikko::PropertyManager> PropertyManager::get(uno::Reference<uno::XComponentContext> const & context) {
	if (!thePropertyManager.is()) {
		VOIKKO_DEBUG("PropertyManager::get first");
		thePropertyManager = new PropertyManager(context);
	}
	return thePropertyManager;
}

}
