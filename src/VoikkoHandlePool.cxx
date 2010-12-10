/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2010 Harri Pitkänen <hatapitk@iki.fi>
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

#include "VoikkoHandlePool.hxx"
#include "common.hxx"

namespace voikko {

using namespace std;
using namespace ::rtl;
using namespace ::com::sun::star;

VoikkoHandlePool * VoikkoHandlePool::getInstance() {
	if (!instance) {
		instance = new VoikkoHandlePool();
	}
	return instance;
}

void VoikkoHandlePool::putHandle(VoikkoHandle * handle) {
	handles[OString("fi")] = handle;
}

VoikkoHandle * VoikkoHandlePool::getHandle(const lang::Locale & /*locale*/) {
	return handles[OString("fi")];
}

void VoikkoHandlePool::closeAllHandles() {
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); it++) {
		voikkoTerminate(it->second);
	}
	handles.clear();
	voikko_initialized = sal_False;
}

void VoikkoHandlePool::setGlobalBooleanOption(int option, bool value) {
	if (globalBooleanOptions.find(option) != globalBooleanOptions.end() && globalBooleanOptions[option] == value) {
		return;
	}
	globalBooleanOptions[option] = value;
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); it++) {
		voikkoSetBooleanOption(it->second, option, value ? 1 : 0);
	}
}

void VoikkoHandlePool::setGlobalIntegerOption(int option, int value) {
	if (globalIntegerOptions.find(option) != globalIntegerOptions.end() && globalIntegerOptions[option] == value) {
		return;
	}
	globalIntegerOptions[option] = value;
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); it++) {
		voikkoSetIntegerOption(it->second, option, value);
	}
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedSpellingLocales() {
	uno::Sequence<lang::Locale> locales(1);
	locales.getArray()[0] = lang::Locale(A2OU("fi"), A2OU("FI"), OUString());
	return locales;
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedHyphenationLocales() {
	return getSupportedSpellingLocales();
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedGrammarLocales() {
	return getSupportedSpellingLocales();
}

static bool containsLocale(const lang::Locale & localeToFind, const uno::Sequence<lang::Locale> & locales) {
	for (sal_Int32 i = 0; i < locales.getLength(); i++) {
		if (locales[i].Language == localeToFind.Language &&
		    locales[i].Country == localeToFind.Country) {
			return true;
		}
	}
	return false;
}

bool VoikkoHandlePool::supportsSpellingLocale(const lang::Locale & locale) {
	return containsLocale(locale, getSupportedSpellingLocales());
}

bool VoikkoHandlePool::supportsHyphenationLocale(const lang::Locale & locale) {
	return containsLocale(locale, getSupportedHyphenationLocales());
}

bool VoikkoHandlePool::supportsGrammarLocale(const lang::Locale & locale) {
	return containsLocale(locale, getSupportedGrammarLocales());
}

VoikkoHandlePool * VoikkoHandlePool::instance = 0;

}
