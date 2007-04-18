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

#include <rtl/ustrbuf.hxx>
#include <libvoikko/voikko.h>

#include "Hyphenator.hxx"
#include "HyphenatedWord.hxx"
#include "PossibleHyphens.hxx"
#include "../common.hxx"

namespace voikko {

Hyphenator::Hyphenator(uno::Reference<uno::XComponentContext> const & context) :
	cppu::WeakComponentImplHelper5
	     <lang::XServiceInfo,
	      linguistic2::XHyphenator,
	      linguistic2::XLinguServiceEventBroadcaster,
	      lang::XInitialization,
	      lang::XServiceDisplayName>(m_aMutex),
	compContext(context) { }

OUString SAL_CALL Hyphenator::getImplementationName() throw (uno::RuntimeException) {
	return getImplementationName_static();
}

sal_Bool SAL_CALL Hyphenator::supportsService(const OUString & serviceName)
	throw (uno::RuntimeException) {
	uno::Sequence<OUString> serviceNames = getSupportedServiceNames();
	for (sal_Int32 i = 0; i < serviceNames.getLength(); i++)
		if (serviceNames[i] == serviceName) return sal_True;
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL Hyphenator::getSupportedServiceNames() throw (uno::RuntimeException) {
	return getSupportedServiceNames_static();
}

uno::Sequence<lang::Locale> SAL_CALL Hyphenator::getLocales() throw (uno::RuntimeException) {
	uno::Sequence<lang::Locale> locales(1);
	locales.getArray()[0] = lang::Locale(A2OU("fi"), A2OU("FI"), OUString());
	return locales;
}

sal_Bool SAL_CALL Hyphenator::hasLocale(const lang::Locale & aLocale) throw (uno::RuntimeException) {
	if (aLocale.Language == A2OU("fi")) return sal_True;
	else return sal_False;
}

uno::Reference<linguistic2::XHyphenatedWord> SAL_CALL
	Hyphenator::hyphenate(const OUString & aWord, const lang::Locale & aLocale,
	                      sal_Int16 nMaxLeading,
	                      const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	thePropertyManager->setValues(aProperties);

	sal_Int16 minLeading = thePropertyManager->getHyphMinLeading();
	sal_Int16 minTrailing = thePropertyManager->getHyphMinTrailing();
	sal_Int32 wlen = aWord.getLength();
	
	// If the word is too short to be hyphenated, return no hyphenation points
	if (wlen < thePropertyManager->getHyphMinWordLength() ||
	    wlen < minLeading + minTrailing) {
		thePropertyManager->resetValues(aProperties);
		return 0;
	}
	
	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	char * hyphenationPoints = voikko_hyphenate_cstr(voikko_handle, oWord.getStr());

	// find the hyphenation point
	sal_Int16 hyphenPos = -1;
	sal_Int32 i = wlen - minTrailing; // The last generally allowed hyphenation point
	if (i > nMaxLeading) i = nMaxLeading; // The last allowed point on this line
	for (; i >= minLeading && hyphenPos == -1; i--) {
		if (hyphenationPoints[i] == '-' || hyphenationPoints[i] == '=') {
			hyphenPos = i;
			break;
		}
	}

	// return the result
	free(hyphenationPoints);
	thePropertyManager->resetValues(aProperties);
	if (hyphenPos != -1) return new HyphenatedWord(aWord, hyphenPos - 1);
	else return 0;
}

uno::Reference<linguistic2::XHyphenatedWord> SAL_CALL
	Hyphenator::queryAlternativeSpelling(const OUString & aWord, const lang::Locale & aLocale,
	                                     sal_Int16 nIndex,
	                                     const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	VOIKKO_DEBUG("Hyphenator::queryAlternativeSpelling");
	return 0;
}

uno::Reference<linguistic2::XPossibleHyphens> SAL_CALL
	Hyphenator::createPossibleHyphens(const OUString & aWord, const lang::Locale & aLocale,
	                                  const uno::Sequence<beans::PropertyValue> & aProperties)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	thePropertyManager->setValues(aProperties);

	// If the word is too short to be hyphenated, return no hyphenation points
	if (aWord.getLength() < thePropertyManager->getHyphMinWordLength()) {
		thePropertyManager->resetValues(aProperties);
		return 0;
	}

	OString oWord = OUStringToOString(aWord, RTL_TEXTENCODING_UTF8);
	uno::Reference<linguistic2::XPossibleHyphens> xRes;	
	char * hyphenationPoints = voikko_hyphenate_cstr(voikko_handle, oWord.getStr());

	/* Count the number of hyphenation points but remove the ones that correspond
	 * to a real hyphen in the word. This is required to prevent adding extra
	 * soft hyphen where it is not needed. FIXME: This check is no longer needed,
	 * must check why. */
	sal_Int16 hpcount = 0;
	for (sal_Int32 i = 0; i < oWord.getLength(); i++) {
		if (hyphenationPoints[i] == '-') {
			hpcount++;
		}
	}

	uno::Sequence<sal_Int16> hyphenSeq(hpcount);
	sal_Int16 *pPos = hyphenSeq.getArray();
	OUStringBuffer hyphenatedWordBuffer;
	OUString hyphenatedWord;
	sal_Int16 nHyphCount = 0;
	for (sal_Int32 i = 0; i < aWord.getLength(); i++) {
		hyphenatedWordBuffer.append(aWord[i]);
		if (hyphenationPoints[i + 1] == '-') {
			pPos[nHyphCount] = i;
			hyphenatedWordBuffer.append(sal_Unicode('='));
			nHyphCount++;
		}
	}

	hyphenatedWord = hyphenatedWordBuffer.makeStringAndClear();
	xRes = new PossibleHyphens(aWord, hyphenatedWord, hyphenSeq);

	free(hyphenationPoints);
	thePropertyManager->resetValues(aProperties);
	return xRes;
}

sal_Bool SAL_CALL Hyphenator::addLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("Hyphenator::addLinguServiceEventListener");
	if (thePropertyManager != 0)
		return thePropertyManager->addLinguServiceEventListener(xLstnr);
	else return sal_False;
}

sal_Bool SAL_CALL Hyphenator::removeLinguServiceEventListener(
	const uno::Reference<linguistic2::XLinguServiceEventListener> & xLstnr)
	throw (uno::RuntimeException) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("Hyphenator::removeLinguServiceEventListener");
	if (thePropertyManager != 0)
		return thePropertyManager->removeLinguServiceEventListener(xLstnr);
	else return sal_False;
}

void SAL_CALL Hyphenator::initialize(const uno::Sequence<uno::Any> & aArguments)
	throw (uno::RuntimeException, uno::Exception) {
	osl::MutexGuard vmg(getVoikkoMutex());
	VOIKKO_DEBUG("Hyphenator::initialize");
	if (thePropertyManager == 0) thePropertyManager = new PropertyManager(compContext);
	thePropertyManager->initialize();
}

OUString SAL_CALL Hyphenator::getServiceDisplayName(const lang::Locale & aLocale)
	throw (uno::RuntimeException) {
	if (aLocale.Language == A2OU("fi"))
		return A2OU("Suomen kielen tavutus (Voikko)");
	else
		return A2OU("Finnish hyphenator (Voikko)");
}

void SAL_CALL Hyphenator::disposing() {
	VOIKKO_DEBUG("Hyphenator::disposing");
}

}
