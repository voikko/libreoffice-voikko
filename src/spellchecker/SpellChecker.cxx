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

#include <libvoikko/voikko.h>

#include "SpellChecker.hxx"
#include "SpellAlternatives.hxx"
#include "../common.hxx"

namespace voikko {

SpellChecker::SpellChecker(uno::Reference<uno::XComponentContext> const & context) :
	cppu::WeakComponentImplHelper5
	     <lang::XServiceInfo,
	      linguistic2::XSpellChecker,
	      linguistic2::XLinguServiceEventBroadcaster,
	      lang::XInitialization,
	      lang::XServiceDisplayName>(m_aMutex),
	compContext(context) { }

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

	// TODO: settings
	/*if (isSpellWithDigits) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	if (isSpellCapitalization) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 1);*/

	// VOIKKO_DEBUG_2("SpellChecker::isValid: c_str: '%s'\n", c_str);
	int result = voikko_spell_cstr(voikko_handle, c_str);
	// VOIKKO_DEBUG_2("SpellChecker::isValid: result = %i\n", result);
	if (result) return sal_True;
	else return sal_False;
}

uno::Reference<linguistic2::XSpellAlternatives> SAL_CALL SpellChecker::spell(
	const OUString & aWord, const lang::Locale & aLocale,
	const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	if (!voikko_initialized) return 0;
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	const char * c_str = oWord.getStr();

	// TODO: settings
	/*if (isSpellWithDigits) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_NUMBERS, 1);
	if (isSpellUpperCase) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_IGNORE_UPPERCASE, 1);
	if (isSpellCapitalization) voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 0);
	else voikko_set_bool_option(voikko_handle, VOIKKO_OPT_ACCEPT_ALL_UPPERCASE, 1);*/

	if (voikko_spell_cstr(voikko_handle, c_str)) return 0;
	char ** suggestions = voikko_suggest_cstr(voikko_handle, c_str);
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
		free(suggestions[i]);
		suggStrings[i] = OStringToOUString(ostr, RTL_TEXTENCODING_UTF8);
	}
	free(suggestions);

	alternatives->alternatives = suggSeq;
	return alternatives;
}

sal_Bool SAL_CALL SpellChecker::addLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("SpellChecker::addLinguServiceEventListener");
	if (thePropertyManager != 0)
		return thePropertyManager->addLinguServiceEventListener(xLstnr);
	else return sal_False;
}

sal_Bool SAL_CALL SpellChecker::removeLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("SpellChecker::removeLinguServiceEventListener");
	if (thePropertyManager != 0)
		return thePropertyManager->removeLinguServiceEventListener(xLstnr);
	else return sal_False;
}

void SAL_CALL SpellChecker::initialize(const uno::Sequence<uno::Any> & aArguments)
	throw (uno::RuntimeException, uno::Exception) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("SpellChecker::initialize");
	if (thePropertyManager == 0) thePropertyManager = new PropertyManager(compContext);
	thePropertyManager->initialize();
}

OUString SAL_CALL SpellChecker::getServiceDisplayName(const lang::Locale & aLocale)
	throw (uno::RuntimeException) {
	if (aLocale.Language == A2OU("fi"))
		return A2OU("Suomen kielen oikoluku (Voikko)");
	else
		return A2OU("Finnish spellchecker (Voikko)");
}

void SAL_CALL SpellChecker::disposing() {
	VOIKKO_DEBUG("SpellChecker::disposing");
}

}
