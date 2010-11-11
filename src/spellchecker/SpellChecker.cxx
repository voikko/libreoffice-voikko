/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
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

#include <libvoikko/voikko.h>

#include "SpellChecker.hxx"
#include "SpellAlternatives.hxx"
#include "../VoikkoHandlePool.hxx"
#include "../common.hxx"

namespace voikko {

SpellChecker::SpellChecker(uno::Reference<uno::XComponentContext> const & context) :
	cppu::WeakComponentImplHelper5
	     <lang::XServiceInfo,
	      linguistic2::XSpellChecker,
	      linguistic2::XLinguServiceEventBroadcaster,
	      lang::XInitialization,
	      lang::XServiceDisplayName>(m_aMutex),
	compContext(context) {
	VOIKKO_DEBUG("SpellChecker:CTOR");
	PropertyManager::get(compContext);
}

OUString SAL_CALL SpellChecker::getImplementationName() throw (uno::RuntimeException) {
	return getImplementationName_static();
}

sal_Bool SAL_CALL SpellChecker::supportsService(const OUString & serviceName)
	throw (uno::RuntimeException) {
	uno::Sequence<OUString> serviceNames = getSupportedServiceNames();
	for (sal_Int32 i = 0; i < serviceNames.getLength(); i++)
		if (serviceNames[i] == serviceName) return sal_True;
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL SpellChecker::getSupportedServiceNames() throw (uno::RuntimeException) {
	return getSupportedServiceNames_static();
}

uno::Sequence<lang::Locale> SAL_CALL SpellChecker::getLocales() throw (uno::RuntimeException) {
	uno::Sequence<lang::Locale> locales(1);
	locales.getArray()[0] = lang::Locale(A2OU("fi"), A2OU("FI"), OUString());
	return locales;
}

sal_Bool SAL_CALL SpellChecker::hasLocale(const lang::Locale & aLocale) throw (uno::RuntimeException) {
	if (aLocale.Language == A2OU("fi")) return sal_True;
	else return sal_False;
}

sal_Bool SAL_CALL SpellChecker::isValid(const OUString & aWord, const lang::Locale & aLocale,
	                              const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	if (!voikko_initialized) return sal_False;
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	const char * c_str = oWord.getStr();

	PropertyManager::get(compContext)->setValues(aProperties);
	// VOIKKO_DEBUG_2("SpellChecker::isValid: c_str: '%s'\n", c_str);
	int result = voikkoSpellCstr(VoikkoHandlePool::getInstance()->getHandle(aLocale), c_str);
	// VOIKKO_DEBUG_2("SpellChecker::isValid: result = %i\n", result);
	PropertyManager::get(compContext)->resetValues(aProperties);
	if (result) return sal_True;
	else return sal_False;
}

uno::Reference<linguistic2::XSpellAlternatives> SAL_CALL SpellChecker::spell(
	const OUString & aWord, const lang::Locale & aLocale,
	const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	
	// Check if diagnostic message should be returned
	if (aWord.equals(A2OU("VoikkoGetStatusInformation"))) {
		SpellAlternatives * alternatives = new SpellAlternatives();
		alternatives->word = aWord;
		uno::Sequence<OUString> suggSeq(1);
		suggSeq.getArray()[0] = PropertyManager::get(compContext)->getInitializationStatus();
		alternatives->alternatives = suggSeq;
		return alternatives;
	}
	
	osl::MutexGuard vmg(getVoikkoMutex());
	if (!voikko_initialized) return 0;
	VoikkoHandle * handle = VoikkoHandlePool::getInstance()->getHandle(aLocale);
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	const char * c_str = oWord.getStr();

	PropertyManager::get(compContext)->setValues(aProperties);
	if (voikkoSpellCstr(handle, c_str)) {
		PropertyManager::get(compContext)->resetValues(aProperties);
		return 0;
	}
	char ** suggestions = voikkoSuggestCstr(handle, c_str);
	PropertyManager::get(compContext)->resetValues(aProperties);
	SpellAlternatives * alternatives = new SpellAlternatives();
	alternatives->word = aWord;
	if (suggestions == 0 || suggestions[0] == 0) return alternatives;
	int scount = 0;
	while (suggestions[scount] != 0) scount++;
	uno::Sequence<OUString> suggSeq(scount);
	OUString * suggStrings = suggSeq.getArray();

	OString ostr;
	for (int i = 0; i < scount; i++) {
		ostr = OString(suggestions[i]);
		suggStrings[i] = OStringToOUString(ostr, RTL_TEXTENCODING_UTF8);
	}
	voikkoFreeCstrArray(suggestions);

	alternatives->alternatives = suggSeq;
	return alternatives;
}

sal_Bool SAL_CALL SpellChecker::addLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("SpellChecker::addLinguServiceEventListener");
	return PropertyManager::get(compContext)->addLinguServiceEventListener(xLstnr);
}

sal_Bool SAL_CALL SpellChecker::removeLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("SpellChecker::removeLinguServiceEventListener");
	return PropertyManager::get(compContext)->removeLinguServiceEventListener(xLstnr);
}

void SAL_CALL SpellChecker::initialize(const uno::Sequence<uno::Any> &)
	throw (uno::RuntimeException, uno::Exception) {
}

OUString SAL_CALL SpellChecker::getServiceDisplayName(const lang::Locale & aLocale)
	throw (uno::RuntimeException) {
	if (aLocale.Language == A2OU("fi"))
		return A2OU("Suomen kielen oikoluku (Voikko)");
	else
		return A2OU("Finnish spellchecker (Voikko)");
}

static uno::Reference<uno::XInterface> theSpellChecker;

void SAL_CALL SpellChecker::disposing() {
	VOIKKO_DEBUG("SpellChecker:DISPOSING");
	theSpellChecker = 0;
}

uno::Reference<uno::XInterface> SAL_CALL SpellChecker::get(uno::Reference<uno::XComponentContext> const & context) {
	VOIKKO_DEBUG("SpellChecker::get");
	if (!theSpellChecker.is()) {
		theSpellChecker = static_cast< ::cppu::OWeakObject * >(new SpellChecker(context));
	}
	return theSpellChecker;
}

}
