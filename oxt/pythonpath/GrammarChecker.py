# Libreoffice-voikko: Linguistic extension for LibreOffice
# Copyright (C) 2015 Harri Pitkänen <hatapitk@iki.fi>
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at http://mozilla.org/MPL/2.0/.
# 
# Alternatively, the contents of this file may be used under the terms of
# the GNU General Public License Version 3 or later (the "GPL"), in which
# case the provisions of the GPL are applicable instead of those above.

import logging
import unohelper
from com.sun.star.linguistic2 import XProofreader, ProofreadingResult, SingleProofreadingError
from com.sun.star.lang import XServiceInfo, XInitialization, XServiceDisplayName
from com.sun.star.beans import PropertyValue
from com.sun.star.text.TextMarkupType import PROOFREADING
from VoikkoHandlePool import VoikkoHandlePool
from PropertyManager import PropertyManager

class GrammarChecker(unohelper.Base, XServiceInfo, XProofreader, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("GrammarChecker.__init__")
		self.__ignoredErrors = set() # Grammar checker error codes that should be ignored

	# From XServiceInfo
	def getImplementationName(self):
		return GrammarChecker.IMPLEMENTATION_NAME

	def supportsService(self, serviceName):
		return serviceName in self.getSupportedServiceNames()

	def getSupportedServiceNames(self):
		return GrammarChecker.SUPPORTED_SERVICE_NAMES

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedGrammarLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsGrammarLocale(aLocale)

	# From XProofreader
	def isSpellChecker(self):
		return False

	def doProofreading(self, aDocumentIdentifier, aText, aLocale, nStartOfSentencePos, nSuggestedBehindEndOfSentencePosition, aProperties):
		logging.debug("GrammarChecker.doProofreading")
		result = ProofreadingResult()
		result.aDocumentIdentifier = aDocumentIdentifier
		result.xFlatParagraph = None
		result.aText = aText
		result.aLocale = aLocale
		result.nStartOfSentencePosition = nStartOfSentencePos
		result.nBehindEndOfSentencePosition = nSuggestedBehindEndOfSentencePosition
		result.xProofreader = self

		VoikkoHandlePool.mutex.acquire()
		try:
			voikko = VoikkoHandlePool.getInstance().getHandle(aLocale)
			if voikko is None:
				logging.error("GrammarChecker.doProofreading called without initializing libvoikko")
				return result

			gcErrors = []
			gcI = 0
			vErrorCount = 0
			for vError in voikko.grammarErrors(aText, PropertyManager.getInstance().getMessageLanguage()):
				startPos = vError.startPos
				errorLength = vError.errorLen

				if startPos < result.nStartOfSentencePosition:
					continue
				if startPos >= result.nBehindEndOfSentencePosition:
					break
				if startPos + errorLength > result.nBehindEndOfSentencePosition:
					result.nBehindEndOfSentencePosition = startPos + errorLength

				# we have a real grammar error
				errorCode = vError.errorCode
				ruleIdentifier = str(errorCode)
				if ruleIdentifier in self.__ignoredErrors:
					# ignore this error
					continue

				suggestions = vError.suggestions

				gcError = SingleProofreadingError()
				gcErrors.append(gcError)
				gcError.nErrorStart = startPos
				gcError.nErrorLength = errorLength
				gcError.nErrorType = PROOFREADING
				comment = vError.shortDescription
				gcError.aShortComment = comment
				gcError.aFullComment = comment
				gcError.aRuleIdentifier = ruleIdentifier

				detailUrl = PropertyValue()
				detailUrl.Name = "FullCommentURL"
				detailUrl.Value = "http://voikko.puimula.org/gchelp/fi/" + ruleIdentifier + ".html"
				gcError.aProperties = (detailUrl,)

				# add suggestions
				if len(suggestions) > 0:
					gcError.aSuggestions = tuple(suggestions)

			result.aErrors = tuple(gcErrors)
			result.nStartOfNextSentencePosition = result.nBehindEndOfSentencePosition
			return result
		finally:
			VoikkoHandlePool.mutex.release()

	def ignoreRule(self, ruleIdentifier, locale):
		self.__ignoredErrors.add(ruleIdentifier)

	def resetIgnoreRules(self):
		ignoredErrors.clear()

	# From XInitialization
	def initialize(self):
		pass

	# From XServiceDisplayName
	def getServiceDisplayName(self, locale):
		if locale.Language == "fi":
			return "Kieliopin tarkistus (Voikko)"
		else:
			return "Grammar checker (Voikko)"

GrammarChecker.IMPLEMENTATION_NAME = "voikko.GrammarChecker"
GrammarChecker.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.Proofreader",)
