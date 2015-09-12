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

class SpellChecker(unohelper.Base, XServiceInfo, XSpellChecker, XLinguServiceEventBroadcaster, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("SpellChecker.__init__")

	# From XServiceInfo
	def getImplementationName(self):
		return SpellChecker.IMPLEMENTATION_NAME

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedSpellingLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsSpellingLocale(aLocale)

	# From XSpellChecker
	def isValid(self, word, locale, properties):
		# TODO mutex
		voikko = VoikkoHandlePool.getInstance().getHandle(locale)
		if voikko is None:
			return False
		# TODO PropertyManager::get(compContext)->setValues(aProperties);
		result = voikko.spell(word)
		# TODO PropertyManager::get(compContext)->resetValues(aProperties);
		return result

	def spell(self, word, locale, properties):
		# Check if diagnostic message should be returned
		if word == "VoikkoGetStatusInformation":
			suggestions = [VoikkoHandlePool.getInstance().getInitializationStatus()]
			return SpellAlternatives(word, suggestions, locale)
		
		# TODO mutex
		voikko = VoikkoHandlePool.getInstance().getHandle(locale)
		if voikko is None:
			return None
		
		# TODO PropertyManager::get(compContext)->setValues(aProperties);
		if voikko.spell(word):
			# TODO PropertyManager::get(compContext)->resetValues(aProperties);
			return None
		suggestions = voikko.suggest(word)
		# TODO PropertyManager::get(compContext)->resetValues(aProperties);
		return SpellAlternatives(word, suggestions, locale)

	# From XLinguServiceEventBroadcaster
	def addLinguServiceEventListener(self, xLstnr):
		logging.debug("SpellChecker.addLinguServiceEventListener")
		return False # TODO

SpellChecker.IMPLEMENTATION_NAME = "voikko.SpellChecker"
SpellChecker.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.SpellChecker",)
