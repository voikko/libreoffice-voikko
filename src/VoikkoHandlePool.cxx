/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2010 - 2012 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#include "VoikkoHandlePool.hxx"
#include "common.hxx"
#include "macros.hxx"

namespace voikko {

using namespace std;
using namespace ::rtl;
using namespace ::com::sun::star;

struct Bcp47ToOOoMapping {
	const char * bcpTag;
	const char * oooLanguage;
	const char * oooRegion;
};

static Bcp47ToOOoMapping const bcpToOOoMapping[] = {
	{"af", "af", "NA"},
	{"af", "af", "ZA"},
	{"am", "am", "ET"},
	{"ar", "ar", "AE"},
	{"ar", "ar", "BH"},
	{"ar", "ar", "DJ"},
	{"ar", "ar", "DZ"},
	{"ar", "ar", "EG"},
	{"ar", "ar", "ER"},
	{"ar", "ar", "IL"},
	{"ar", "ar", "IQ"},
	{"ar", "ar", "JO"},
	{"ar", "ar", "KM"},
	{"ar", "ar", "KW"},
	{"ar", "ar", "LB"},
	{"ar", "ar", "LY"},
	{"ar", "ar", "MA"},
	{"ar", "ar", "MR"},
	{"ar", "ar", "OM"},
	{"ar", "ar", "PS"},
	{"ar", "ar", "QA"},
	{"ar", "ar", "SA"},
	{"ar", "ar", "SD"},
	{"ar", "ar", "SO"},
	{"ar", "ar", "SY"},
	{"ar", "ar", "TD"},
	{"ar", "ar", "TN"},
	{"ar", "ar", "YE"},
	{"bn", "bn", "BD"},
	{"bn", "bn", "IN"},
	{"ca", "ca", "AD"},
	{"ca", "ca", "ES"},
	{"cs", "cs", "CZ"},
	{"csb", "csb", "PL"},
	{"cv", "cv", "RU"},
	{"cy", "cy", "GB"},
	{"da", "da", "DK"},
	{"de", "de", "DE"},
	{"de-AT", "de", "AT"},
	{"de-BE", "de", "BE"},
	{"de-CH", "de", "CH"},
	{"de-LU", "de", "LU"},
	{"el", "el", "GR"},
	{"en-US", "en", "US"},
	{"en-US", "en", "ZA"},
	{"eo", "eo", ""},
	{"es", "es", "ES"},
	{"es-MX", "es", "MX"},
	{"et", "et", "EE"},
	{"eu", "eu", ""},
	{"fa", "fa", "IR"},
	{"fi", "fi", "FI"},
	{"fo", "fo", "FO"},
	{"fr", "fr", "FR"},
	{"fr-BE", "fr", "BE"},
	{"fr-CA", "fr", "CA"},
	{"fr-CH", "fr", "CH"},
	{"fr-LU", "fr", "LU"},
	{"fr-MC", "fr", "MC"},
	{"fur", "fur", "IT"},
	{"fy", "fy", "NL"},
	{"ga", "ga", "IE"},
	{"gd", "gd", "GB"},
	{"gl", "gl", "ES"},
	{"gn", "gug", "PY"},
	{"gu", "gu", "IN"},
	{"he", "he", "IL"},
	{"hi", "hi", "IN"},
	{"hr", "hr", "BA"},
	{"hr", "hr", "HR"},
	{"hu", "hu", "HU"},
	{"ia", "ia", ""},
	{"id", "id", "ID"},
	{"is", "is", "IS"},
	{"it", "it", "CH"},
	{"it", "it", "IT"},
	{"kca", "kca", "RU"},
	{"kk", "kk", "KZ"},
	{"kl", "kl", "GL"},
	{"koi", "koi", "RU"},
	{"kpv", "kpv", "RU"},
	{"ku", "ku", "TR"},
	{"ku", "ku", "SY"},
	{"kum", "kum", "RU"},
	{"ky", "ky", "CN"},
	{"ky", "ky", "KG"},
	{"la", "la", "VA"},
	{"liv", "liv", "LV"},
	{"liv", "liv", "RU"},
	{"ln", "ln", "CD"},
	{"lt", "lt", "LT"},
	{"lv", "lv", "LV"},
	{"mdf", "mdf", "RU"},
	{"mhr", "mhr", "RU"},
	{"mk", "mk", "MK"},
	{"ml", "ml", "IN"},
	{"mrj", "mrj", "RU"},
	{"ms", "ms", ""},
	{"ms", "ms", "BN"},
	{"ms", "ms", "MY"},
	{"myv", "myv", "RU"},
	{"nb", "nb", "NO"},
	{"ne", "ne", "IN"},
	{"ne", "ne", "NP"},
	{"nio", "nio", "RU"},
	{"nl", "nl", "BE"},
	{"nl", "nl", "NL"},
	{"nn", "nn", "NO"},
	{"nog", "nog", "RU"},
	{"nr", "nr", "ZA"},
	{"nso", "ns", "ZA"},
	{"nso", "nso", "ZA"},
	{"ny", "ny", "MW"},
	{"oc", "oc", "FR"},
	{"olo", "olo", "RU"},
	{"or", "or", "IN"},
	{"pa", "pa", "IN"},
	{"pa", "pa", "PK"},
	{"pap-BQ", "pap", "BQ"},
	{"pap-CW", "pap", "CW"},
	{"pjt", "pjt", "AU"},
	{"pl", "pl", "PL"},
	{"pt", "pt", "PT"},
	{"pt-BR", "pt", "BR"},
	{"qu", "qu", "BO"},
	{"qu", "qu", "EC"},
	{"qu", "qu", "PE"},
	{"ro", "ro", "MD"},
	{"ro", "ro", "RO"},
	{"ru", "ru", "RU"},
	{"rw", "rw", "RW"},
	{"se", "se", "FI"},
	{"se", "se", "NO"},
	{"se", "se", "SE"},
	{"sid", "sid", "ET"},
	{"sjd", "sjd", "RU"},
	{"sk", "sk", "SK"},
	{"sl", "sl", "SI"},
	{"sma", "sma", "NO"},
	{"sma", "sma", "SE"},
	{"smj", "smj", "NO"},
	{"smj", "smj", "SE"},
	{"smn", "smn", "FI"},
	{"sms", "sms", "FI"},
	{"ss", "ss", "ZA"},
	{"st", "st", "ZA"},
	{"sv", "sv", "FI"},
	{"sv", "sv", "SE"},
	{"sw", "sw", "KE"},
	{"sw", "sw", "TZ"},
	{"ta", "ta", "IN"},
	{"tet", "tet", "ID"},
	{"tet", "tet", "TL"},
	{"tl", "tl", "PH"},
	{"tn", "tn", "BW"},
	{"tn", "tn", "ZA"},
	{"ts", "ts", "ZA"},
	{"tt", "tt", "RU"},
	{"udm", "udm", "RU"},
	{"uk", "uk", "UA"},
	{"vep", "vep", "RU"},
	{"vi", "vi", "VN"},
	{"vro", "vro", "EE"},
	{"xh", "xh", "ZA"},
	{"yrk", "yrk", "RU"},
// Added a new block of language codes - should be sorted and merged with the
// rest when accepted.
	{"an",	"an",	"ES"},
	{"as",	"as",	"IN"},
	{"as-BT",	"as",	"BT"},
	{"ast",	"ast",	"ES"},
	{"av",	"av",	"RU"},
	{"av-AZ",	"av",	"AZ"},
	{"av-GE",	"av",	"GE"},
	{"av-KZ",	"av",	"KZ"},
	{"azj",	"azj",	"AZ"},
	{"azj-RU",	"azj",	"RU"},
	{"bak",	"bak",	"RU"},
	{"bak-KZ",	"bak",	"KZ"},
	{"be",	"be",	"BY"},
	{"bg",	"bg",	"BG"},
	{"bla",	"bla",	"CA"},
	{"bla-US",	"bla",	"US"},
	{"br",	"br",	"FR"},
	{"bxr",	"bxr",	"RU"},
	{"ce",	"ce",	"RU"},
	{"ceb",	"ceb",	"PH"},
	{"chp",	"chp",	"CA"},
	{"ciw",	"ciw",	"US"},
	{"cos",	"cos",	"FR"},
	{"crk",	"crk",	"CA"},
	{"crk-US",	"crk",	"US"},
	{"dsb",	"dsb",	"DE"},
	{"eve",	"eve",	"RU"},
	{"evn",	"evn",	"CN"},
	{"evn-RU",	"evn",	"RU"},
	{"fkv",	"fkv",	"NO"},
	{"fry",	"fry",	"NL"},
	{"glk",	"glk",	"IR"},
	{"grn",	"grn",	"PY"},
	{"guc",	"guc",	"CO"},
	{"guc-VE",	"guc",	"VE"},
	{"gv",	"gv",	"IM"},
	{"hdn",	"hdn",	"CA"},
	{"hdn-US",	"hdn",	"US"},
	{"hsb",	"hsb",	"DE"},
	{"hy",	"hy",	"AM"},
	{"ik",	"ik",	"US"},
	{"izh",	"izh",	"RU"},
	{"kaa",	"kaa",	"UZ"},
	{"kaa-RU",	"kaa",	"RU"},
	{"kaz",	"kaz",	"KZ"},
	{"khk",	"khk",	"MN"},
	{"khk-KG",	"khk",	"KG"},
	{"khk-RU",	"khk",	"RU"},
	{"khk-CN",	"khk",	"CN"},
	{"kw",	"kw",	"UK"},
	{"ltz",	"ltz",	"LU"},
	{"ml",	"ml",	"IN"},
	{"mr",	"mr",	"IN"},
	{"mt",	"mt",	"MT"},
	{"ndl",	"ndl",	"CD"},
	{"quz",	"quz",	"PE"},
	{"qve",	"qve",	"PE"},
	{"rup",	"rup",	"MK"},
	{"rup-GR",	"rup",	"GR"},
	{"rup-RO",	"rup",	"RO"},
	{"sc",	"sc",	"IT"},
	{"sel",	"sel",	"RU"},
	{"si",	"si",	"LK"},
	{"sje",	"sje",	"SE"},
	{"so",	"so",	"SO"},
	{"sq",	"sq",	"AL"},
	{"sto",	"sto",	"CA"},
	{"te",	"te",	"IN"},
	{"tg",	"tg",	"TJ"},
	{"tg-UZ",	"tg",	"UZ"},
	{"tl",	"tl",	"PH"},
	{"tk",	"tk",	"TM"},
	{"tku",	"tku",	"MX"},
	{"tus",	"tus",	"CA"},
	{"tyv",	"tyv",	"RU"},
	{"ur",	"ur",	"PK"},
	{"uz",	"uz",	"UZ"},
	{"uz-KG",	"uz",	"KG"},
	{"zu",	"zu",	"ZA"},
	{"zu-LS",	"zu",	"LS"},
	{"zu-SZ",	"zu",	"SZ"},
	{0, 0, 0}
};

VoikkoHandlePool::VoikkoHandlePool() {
	for (const Bcp47ToOOoMapping * m = bcpToOOoMapping; m->bcpTag; ++m) {
		bcpToOOoMap.insert(pair<string, pair<string, string> >(m->bcpTag,
		                   pair<string, string>(m->oooLanguage, m->oooRegion)));
	}
}

VoikkoHandlePool * VoikkoHandlePool::getInstance() {
	if (!instance) {
		instance = new VoikkoHandlePool();
	}
	return instance;
}

const char * VoikkoHandlePool::getInstallationPath() {
	if (installationPath.getLength() == 0) {
		return 0;
	} else {
		return installationPath.getStr();
	}
}

VoikkoHandle * VoikkoHandlePool::openHandleWithVariant(const OString & language, const OString & fullVariant) {
	const char * errorString = 0;
	VOIKKO_DEBUG("VoikkoHandlePool::openHandleWithVariant");
	VoikkoHandle * voikkoHandle = voikkoInit(&errorString, fullVariant.getStr(), getInstallationPath());
	if (voikkoHandle) {
		handles[language] = voikkoHandle;
		for (map<int, bool>::const_iterator it = globalBooleanOptions.begin(); it != globalBooleanOptions.end(); ++it) {
			voikkoSetBooleanOption(voikkoHandle, it->first, it->second ? 1 : 0);
		}
		for (map<int, int>::const_iterator it = globalIntegerOptions.begin(); it != globalIntegerOptions.end(); ++it) {
			voikkoSetIntegerOption(voikkoHandle, it->first, it->second);
		}
		return voikkoHandle;
	} else {
		initializationErrors[language] = errorString;
		return 0;
	}
}

VoikkoHandle * VoikkoHandlePool::openHandle(const OString & language) {
	if (getPreferredGlobalVariant().getLength() > 0) {
		OString languageWithVariant = language + OString("-x-") +
			OUStringToOString(getPreferredGlobalVariant(), RTL_TEXTENCODING_UTF8);
		VoikkoHandle * handle = openHandleWithVariant(language, languageWithVariant);
		if (handle) {
			return handle;
		}
	}
	return openHandleWithVariant(language, language);
}

VoikkoHandle * VoikkoHandlePool::getHandle(const lang::Locale & locale) {
	OString language = OUStringToOString(locale.Language, RTL_TEXTENCODING_UTF8);
	if (handles.find(language) != handles.end()) {
		return handles[language];
	}
	if (initializationErrors.find(language) != initializationErrors.end()) {
		return 0;
	}
	return openHandle(language);
}

void VoikkoHandlePool::closeAllHandles() {
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); ++it) {
		voikkoTerminate(it->second);
	}
	handles.clear();
	initializationErrors.clear();
}

void VoikkoHandlePool::setGlobalBooleanOption(int option, bool value) {
	if (globalBooleanOptions.find(option) != globalBooleanOptions.end() && globalBooleanOptions[option] == value) {
		return;
	}
	globalBooleanOptions[option] = value;
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); ++it) {
		voikkoSetBooleanOption(it->second, option, value ? 1 : 0);
	}
}

void VoikkoHandlePool::setGlobalIntegerOption(int option, int value) {
	if (globalIntegerOptions.find(option) != globalIntegerOptions.end() && globalIntegerOptions[option] == value) {
		return;
	}
	globalIntegerOptions[option] = value;
	for (map<OString,VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); ++it) {
		voikkoSetIntegerOption(it->second, option, value);
	}
}

void VoikkoHandlePool::addLocale(uno::Sequence<lang::Locale> & locales, const char * language) {
	// TODO: n^2 performance because sequence is reconstructed on every add
	sal_Int32 position = locales.getLength();
	pair<multimap<string, pair<string, string> >::iterator, multimap<string, pair<string, string> >::iterator>
	   matchingLangs = bcpToOOoMap.equal_range(language);
	for (multimap<string, pair<string, string> >::iterator it = matchingLangs.first;
	     it != matchingLangs.second; ++it) {
		locales.realloc(++position);
		OUString lang = A2OU((*it).second.first.c_str());
		OUString region = A2OU((*it).second.second.c_str());
		locales.getArray()[position - 1] = lang::Locale(lang, region, OUString());
	}
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedSpellingLocales() {
	// optimization: if we already have found some locales, don't search for more
	if (supportedSpellingLocales.getLength() == 0) {
		char ** languages = voikkoListSupportedSpellingLanguages(getInstallationPath());
		for (char ** i = languages; *i; i++) {
			addLocale(supportedSpellingLocales, *i);
		}
		voikkoFreeCstrArray(languages);
	}
	return supportedSpellingLocales;
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedHyphenationLocales() {
	// optimization: if we already have found some locales, don't search for more
	if (supportedHyphenationLocales.getLength() == 0) {
		char ** languages = voikkoListSupportedHyphenationLanguages(getInstallationPath());
		for (char ** i = languages; *i; i++) {
			addLocale(supportedHyphenationLocales, *i);
		}
		voikkoFreeCstrArray(languages);
	}
	return supportedHyphenationLocales;
}

uno::Sequence<lang::Locale> VoikkoHandlePool::getSupportedGrammarLocales() {
	// optimization: if we already have found some locales, don't search for more
	if (supportedGrammarCheckingLocales.getLength() == 0) {
		char ** languages = voikkoListSupportedGrammarCheckingLanguages(getInstallationPath());
		for (char ** i = languages; *i; i++) {
			addLocale(supportedGrammarCheckingLocales, *i);
		}
		voikkoFreeCstrArray(languages);
	}
	return supportedGrammarCheckingLocales;
}

OUString VoikkoHandlePool::getInitializationStatus() {
	OUString status = A2OU("Init OK:[");
	for (map<OString, VoikkoHandle *>::const_iterator it = handles.begin(); it != handles.end(); ++it) {
		status += OStringToOUString(it->first, RTL_TEXTENCODING_UTF8) + A2OU(" ");
	}
	
	status += A2OU("] FAILED:[");
	for (map<OString, const char *>::const_iterator it = initializationErrors.begin(); it != initializationErrors.end(); ++it) {
		status += OStringToOUString(it->first, RTL_TEXTENCODING_UTF8) + A2OU(":'") + A2OU(it->second) + A2OU("' ");
	}
	status += A2OU("]");
	
	return status;
}

void VoikkoHandlePool::setPreferredGlobalVariant(const OUString & variant) {
	if (variant != this->preferredGlobalVariant) {
		this->preferredGlobalVariant = variant;
		closeAllHandles();
	}
}

void VoikkoHandlePool::setInstallationPath(const rtl::OString & path) {
	this->installationPath = path;
}

OUString VoikkoHandlePool::getPreferredGlobalVariant() {
	return this->preferredGlobalVariant;
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
