/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2010 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#ifndef _VOIKKOHANDLEPOOL_HXX_
#define _VOIKKOHANDLEPOOL_HXX_

#include <libvoikko/voikko.h>
#include <map>
#include <com/sun/star/lang/Locale.hpp>
#include <com/sun/star/uno/Sequence.h>
#include <string>

namespace voikko {

//using namespace ::rtl;
using namespace ::com::sun::star;

class VoikkoHandlePool {
	public:
		static VoikkoHandlePool * getInstance();
		
		VoikkoHandle * getHandle(const lang::Locale & locale);
		
		void closeAllHandles();
		
		void setGlobalBooleanOption(int option, bool value);
		
		void setGlobalIntegerOption(int option, int value);
		
		/**
		 * Set the preferred dictionary variant (private use tag) that
		 * will be used for all languages. If the specified variant is not
		 * available for given language then standard variant is used
		 * as a fallback.
		 */
		void setPreferredGlobalVariant(const rtl::OUString & variant);
		
		void setInstallationPath(const rtl::OString & path);
		
		const char * getInstallationPath();
		
		rtl::OUString getPreferredGlobalVariant();
		
		uno::Sequence<lang::Locale> getSupportedSpellingLocales();
		
		uno::Sequence<lang::Locale> getSupportedHyphenationLocales();
		
		uno::Sequence<lang::Locale> getSupportedGrammarLocales();
		
		bool supportsSpellingLocale(const lang::Locale & locale);
		
		bool supportsHyphenationLocale(const lang::Locale & locale);
		
		bool supportsGrammarLocale(const lang::Locale & locale);
		
		/** Returns initialization status diagnostics */
		rtl::OUString getInitializationStatus();
	private:
		VoikkoHandlePool();
		VoikkoHandle * openHandle(const rtl::OString & language);
		VoikkoHandle * openHandleWithVariant(const rtl::OString & language, const rtl::OString & fullVariant);
		void addLocale(uno::Sequence<lang::Locale> & locales, const char * language);
		std::map<rtl::OString, VoikkoHandle *> handles;
		std::map<rtl::OString, const char *> initializationErrors;
		std::map<int, bool> globalBooleanOptions;
		std::map<int, int> globalIntegerOptions;
		std::multimap<std::string, std::pair<std::string, std::string> > bcpToOOoMap;
		uno::Sequence<lang::Locale> supportedSpellingLocales;
		uno::Sequence<lang::Locale> supportedHyphenationLocales;
		uno::Sequence<lang::Locale> supportedGrammarCheckingLocales;
		rtl::OUString preferredGlobalVariant;
		rtl::OString installationPath;
		static VoikkoHandlePool * instance;
};

}

#endif
