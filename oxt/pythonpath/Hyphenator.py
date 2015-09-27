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
from com.sun.star.linguistic2 import XHyphenator, XLinguServiceEventBroadcaster
from com.sun.star.lang import XServiceInfo, XInitialization, XServiceDisplayName
from VoikkoHandlePool import VoikkoHandlePool
from HyphenatedWord import HyphenatedWord
from PossibleHyphens import PossibleHyphens
from PropertyManager import PropertyManager

class Hyphenator(unohelper.Base, XServiceInfo, XHyphenator, XLinguServiceEventBroadcaster, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("Hyphenator.__init__")

	# From XServiceInfo
	def getImplementationName(self):
		return Hyphenator.IMPLEMENTATION_NAME

	def supportsService(self, serviceName):
		return serviceName in self.getSupportedServiceNames()

	def getSupportedServiceNames(self):
		return Hyphenator.SUPPORTED_SERVICE_NAMES

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedHyphenationLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsHyphenationLocale(aLocale)

	# From XHyphenator
	def hyphenate(self, word, locale, nMaxLeading, properties):
		logging.debug("Hyphenator.hyphenate")
		if len(word) > 10000:
			return None
		VoikkoHandlePool.mutex.acquire()
		try:
			voikko = VoikkoHandlePool.getInstance().getHandle(locale)
			if voikko is None:
				return None
			PropertyManager.getInstance().setValues(properties)

			minLeading = PropertyManager.getInstance().getHyphMinLeading()
			minTrailing = PropertyManager.getInstance().getHyphMinTrailing()
			wlen = len(word)

			# If the word is too short to be hyphenated, return no hyphenation points
			if wlen < PropertyManager.getInstance().getHyphMinWordLength() or wlen < minLeading + minTrailing:
				PropertyManager.getInstance().resetValues(properties)
				return None

			hyphenationPoints = voikko.getHyphenationPattern(word)
			if hyphenationPoints is None:
				PropertyManager.getInstance().resetValues(properties)
				return None

			# find the hyphenation point
			hyphenPos = -1
			i = wlen - minTrailing # The last generally allowed hyphenation point
			if i > nMaxLeading:
				i = nMaxLeading # The last allowed point on this line
			while i >= minLeading and hyphenPos == -1:
				if word[i] == '\'':
					i = i - 1
					continue
				if hyphenationPoints[i] == '-' or hyphenationPoints[i] == '=':
					hyphenPos = i
					break
				i = i - 1

			# return the result
			PropertyManager.getInstance().resetValues(properties)
			if hyphenPos != -1:
				return HyphenatedWord(word, hyphenPos - 1, locale)
			else:
				return None
		finally:
			VoikkoHandlePool.mutex.release()

	def queryAlternativeSpelling(self, word, locale, index, properties):
		logging.debug("Hyphenator.queryAlternativeSpelling")
		# Implementing this might be necessary, although everything seems to work fine without it.
		return None

	def createPossibleHyphens(self, word, locale, properties):
		logging.debug("Hyphenator.createPossibleHyphens")
		wlen = len(word)
		if wlen > 10000:
			return None
		VoikkoHandlePool.mutex.acquire()
		try:
			voikko = VoikkoHandlePool.getInstance().getHandle(locale)
			if voikko is None:
				return None
			PropertyManager.getInstance().setValues(properties)

			# If the word is too short to be hyphenated, return no hyphenation points
			minLeading = PropertyManager.getInstance().getHyphMinLeading()
			minTrailing = PropertyManager.getInstance().getHyphMinTrailing()
			if wlen < PropertyManager.getInstance().getHyphMinWordLength() or wlen < minLeading + minTrailing:
				PropertyManager.getInstance().resetValues(properties)
				return None

			hyphenationPoints = voikko.getHyphenationPattern(word)
			if hyphenationPoints is None:
				PropertyManager.getInstance().resetValues(properties)
				return None

			hyphenSeq = []
			hyphenatedWord = ""
			for i in range(0, wlen):
				hyphenatedWord = hyphenatedWord + word[i]
				if i >= minLeading - 1 and i < wlen - minTrailing and hyphenationPoints[i + 1] == '-':
					hyphenSeq.append(i)
					hyphenatedWord = hyphenatedWord + "="

			res = PossibleHyphens(word, hyphenatedWord, hyphenSeq, locale)
			PropertyManager.getInstance().resetValues(properties)
			return res
		finally:
			VoikkoHandlePool.mutex.release()

	# From XLinguServiceEventBroadcaster
	def addLinguServiceEventListener(self, xLstnr):
		logging.debug("Hyphenator.addLinguServiceEventListener")
		VoikkoHandlePool.mutex.acquire()
		try:
			return PropertyManager.getInstance().addLinguServiceEventListener(xLstnr)
		finally:
			VoikkoHandlePool.mutex.release()

	def removeLinguServiceEventListener(self, xLstnr):
		logging.debug("Hyphenator.removeLinguServiceEventListener")
		VoikkoHandlePool.mutex.acquire()
		try:
			return PropertyManager.getInstance().removeLinguServiceEventListener(xLstnr)
		finally:
			VoikkoHandlePool.mutex.release()

	# From XInitialization
	def initialize(self, seq):
		pass

	# From XServiceDisplayName
	def getServiceDisplayName(self, locale):
		if locale.Language == "fi":
			return "Tavutus (Voikko)"
		else:
			return "Hyphenator (Voikko)"

Hyphenator.IMPLEMENTATION_NAME = "voikko.Hyphenator"
Hyphenator.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.Hyphenator",)
