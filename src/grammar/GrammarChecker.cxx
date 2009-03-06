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

#include <com/sun/star/text/TextMarkupType.hpp>
#include <libvoikko/voikko.h>

#include "GrammarChecker.hxx"
#include "../common.hxx"

namespace voikko {

GrammarChecker::GrammarChecker(uno::Reference<uno::XComponentContext> const & context) :
	cppu::WeakComponentImplHelper4
	     <lang::XServiceInfo,
	      linguistic2::XProofreader,
	      lang::XInitialization,
	      lang::XServiceDisplayName>(m_aMutex),
	compContext(context) {
	VOIKKO_DEBUG("GrammarChecker:CTOR");
	PropertyManager::get(compContext);
}

OUString SAL_CALL GrammarChecker::getImplementationName() throw (uno::RuntimeException) {
	return getImplementationName_static();
}

sal_Bool SAL_CALL GrammarChecker::supportsService(const OUString & serviceName)
	throw (uno::RuntimeException) {
	VOIKKO_DEBUG("GrammarChecker::supportsService");
	uno::Sequence<OUString> serviceNames = getSupportedServiceNames();
	for (sal_Int32 i = 0; i < serviceNames.getLength(); i++)
		if (serviceNames[i] == serviceName) return sal_True;
	return sal_False;
}

uno::Sequence<OUString> SAL_CALL GrammarChecker::getSupportedServiceNames() throw (uno::RuntimeException) {
	VOIKKO_DEBUG("GrammarChecker::getSupportedServiceNames");
	return getSupportedServiceNames_static();
}

uno::Sequence<lang::Locale> SAL_CALL GrammarChecker::getLocales() throw (uno::RuntimeException) {
	VOIKKO_DEBUG("GrammarChecker::getLocales");
	uno::Sequence<lang::Locale> locales(1);
	locales.getArray()[0] = lang::Locale(A2OU("fi"), A2OU("FI"), OUString());
	return locales;
}

sal_Bool SAL_CALL GrammarChecker::hasLocale(const lang::Locale & aLocale) throw (uno::RuntimeException) {
	VOIKKO_DEBUG("GrammarChecker::hasLocale");
	if (aLocale.Language == A2OU("fi")) return sal_True;
	else return sal_False;
}

sal_Bool SAL_CALL GrammarChecker::isSpellChecker() throw (uno::RuntimeException) {
	return sal_False;
}

linguistic2::ProofreadingResult SAL_CALL GrammarChecker::doProofreading(
	const OUString & aDocumentIdentifier,
	const OUString & aText,
	const lang::Locale & aLocale,
	sal_Int32 nStartOfSentencePos,
	sal_Int32 nSuggestedBehindEndOfSentencePosition,
	const uno::Sequence<beans::PropertyValue> & /* aProperties */)
	throw (uno::RuntimeException, lang::IllegalArgumentException) {
	
	VOIKKO_DEBUG("GrammarChecker::doProofreading");
	linguistic2::ProofreadingResult result;
	result.aDocumentIdentifier = aDocumentIdentifier;
	result.xFlatParagraph = 0;
	result.aText = aText;
	result.aLocale = aLocale;
	result.nStartOfSentencePosition = nStartOfSentencePos;
	result.nBehindEndOfSentencePosition = nSuggestedBehindEndOfSentencePosition;
	result.xProofreader = this;
	
	if (!voikko_initialized) {
		VOIKKO_DEBUG("ERROR: GrammarChecker::doProofreading called without initializing libvoikko");
		return result;
	}

	OString textUtf8 = ::rtl::OUStringToOString(aText, RTL_TEXTENCODING_UTF8);
	sal_Int32 paraLen = textUtf8.getLength();
	uno::Sequence<linguistic2::SingleProofreadingError> gcErrors(0);
	sal_Int32 gcI = 0;
	sal_Int32 vErrorCount = 0;
	while (paraLen < 1000000) { // sanity check
		voikko_grammar_error vError = voikko_next_grammar_error_cstr(
			voikko_handle, textUtf8.getStr(), paraLen, 0, vErrorCount++);
		if (vError.error_code == 0) break;
		if ((sal_Int32) vError.startpos < result.nStartOfSentencePosition) continue;
		if ((sal_Int32) vError.startpos >= result.nBehindEndOfSentencePosition) break;
		if ((sal_Int32) (vError.startpos + vError.errorlen) > result.nBehindEndOfSentencePosition)
			result.nBehindEndOfSentencePosition = vError.startpos + vError.errorlen;
		
		// we have a real grammar error
		gcErrors.realloc(gcI + 1);
		gcErrors[gcI].nErrorStart = vError.startpos;
		gcErrors[gcI].nErrorLength = vError.errorlen;
		gcErrors[gcI].nErrorType = text::TextMarkupType::PROOFREADING;
		OString commentOString = OString(voikko_error_message_cstr(vError.error_code,
			PropertyManager::get(compContext)->getMessageLanguage()));
		OUString comment = OStringToOUString(commentOString, RTL_TEXTENCODING_UTF8);
		#ifdef TEKSTINTUHO
			comment += UTF82OU(" TEKSTINTUHO K\xc3\x84YT\xc3\x96SS\xc3\x84!");
		#endif
		gcErrors[gcI].aShortComment = comment;
		gcErrors[gcI].aFullComment = comment;

		// add suggestions
		int scount = 0;
		while (vError.suggestions && vError.suggestions[scount] != 0) scount++;
		uno::Sequence<OUString> suggSeq(scount);
		for (int i = 0; i < scount; i++) {
			OString ostr = OString(vError.suggestions[i]);
			suggSeq[i] = OStringToOUString(ostr, RTL_TEXTENCODING_UTF8);
		}
		voikko_free_suggest_cstr(vError.suggestions);
		gcErrors[gcI].aSuggestions = suggSeq;

		gcI++;
	}

	result.aErrors = gcErrors;
	result.nStartOfNextSentencePosition = result.nBehindEndOfSentencePosition;
	return result;
}

void SAL_CALL GrammarChecker::ignoreRule(const OUString & /* aRuleIdentifier */, const lang::Locale & /* aLocale */)
		throw (lang::IllegalArgumentException, uno::RuntimeException) {
	// TODO: unimplemented
}

void SAL_CALL GrammarChecker::resetIgnoreRules() throw (uno::RuntimeException) {
	// TODO: unimplemented
}

void SAL_CALL GrammarChecker::initialize(const uno::Sequence<uno::Any> &)
	throw (uno::RuntimeException, uno::Exception) {
}

OUString SAL_CALL GrammarChecker::getServiceDisplayName(const lang::Locale & aLocale)
	throw (uno::RuntimeException) {
	VOIKKO_DEBUG("GrammarChecker::getServiceDisplayName");
	if (aLocale.Language == A2OU("fi"))
		return A2OU("Suomen kieliopin tarkistus (Voikko)");
	else
		return A2OU("Finnish grammar checker (Voikko)");
}

static uno::Reference<uno::XInterface> theGrammarChecker;

void SAL_CALL GrammarChecker::disposing() {
	VOIKKO_DEBUG("GrammarChecker:DISPOSING");
	theGrammarChecker = 0;
}

uno::Reference<uno::XInterface> SAL_CALL GrammarChecker::get(uno::Reference<uno::XComponentContext> const & context) {
	VOIKKO_DEBUG("GrammarChecker::get");
	if (!theGrammarChecker.is()) {
		theGrammarChecker = static_cast< ::cppu::OWeakObject * >(new GrammarChecker(context));
	}
	return theGrammarChecker;
}

}
