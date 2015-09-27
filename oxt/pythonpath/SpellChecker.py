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
from com.sun.star.linguistic2 import XSpellChecker, XLinguServiceEventBroadcaster
from com.sun.star.lang import XServiceInfo, XInitialization, XServiceDisplayName
from VoikkoHandlePool import VoikkoHandlePool
from SpellAlternatives import SpellAlternatives
from PropertyManager import PropertyManager

class SpellChecker(unohelper.Base, XServiceInfo, XSpellChecker, XLinguServiceEventBroadcaster, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("SpellChecker.__init__")

	# From XServiceInfo
	def getImplementationName(self):
		return SpellChecker.IMPLEMENTATION_NAME

	def supportsService(self, serviceName):
		return serviceName in self.getSupportedServiceNames()

	def getSupportedServiceNames(self):
		return SpellChecker.SUPPORTED_SERVICE_NAMES

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedSpellingLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsSpellingLocale(aLocale)

	# From XSpellChecker
	def isValid(self, word, locale, properties):
		VoikkoHandlePool.mutex.acquire()
		try:
			voikko = VoikkoHandlePool.getInstance().getHandle(locale)
			if voikko is None:
				return False
			PropertyManager.getInstance().setValues(properties)
			result = voikko.spell(word)
			PropertyManager.getInstance().resetValues(properties)
			return result
		finally:
			VoikkoHandlePool.mutex.release()

	def spell(self, word, locale, properties):
		# Check if diagnostic message should be returned
		if word == "VoikkoGetStatusInformation":
			suggestions = [VoikkoHandlePool.getInstance().getInitializationStatus()]
			return SpellAlternatives(word, suggestions, locale)
		
		VoikkoHandlePool.mutex.acquire()
		try:
			voikko = VoikkoHandlePool.getInstance().getHandle(locale)
			if voikko is None:
				return None

			PropertyManager.getInstance().setValues(properties)
			if voikko.spell(word):
				PropertyManager.getInstance().resetValues(properties)
				return None
			suggestions = voikko.suggest(word)
			PropertyManager.getInstance().resetValues(properties)
			return SpellAlternatives(word, suggestions, locale)
		finally:
			VoikkoHandlePool.mutex.release()

	# From XLinguServiceEventBroadcaster
	def addLinguServiceEventListener(self, xLstnr):
		logging.debug("SpellChecker.addLinguServiceEventListener")
		VoikkoHandlePool.mutex.acquire()
		try:
			return PropertyManager.getInstance().addLinguServiceEventListener(xLstnr)
		finally:
			VoikkoHandlePool.mutex.release()


	def removeLinguServiceEventListener(self, xLstnr):
		logging.debug("SpellChecker.removeLinguServiceEventListener")
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
			return "Oikoluku (Voikko)"
		else:
			return "Spellchecker (Voikko)"

SpellChecker.IMPLEMENTATION_NAME = "voikko.SpellChecker"
SpellChecker.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.SpellChecker",)
