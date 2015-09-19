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
import os
import sys
import uno
from VoikkoHandlePool import VoikkoHandlePool
from com.sun.star.beans import XPropertyChangeListener, UnknownPropertyException, PropertyValue
from com.sun.star.linguistic2 import LinguServiceEvent
from com.sun.star.linguistic2.LinguServiceEventFlags import SPELL_CORRECT_WORDS_AGAIN, SPELL_WRONG_WORDS_AGAIN, HYPHENATE_AGAIN, PROOFREAD_AGAIN

class PropertyManager(unohelper.Base, XPropertyChangeListener):

	def __init__(self):
		# TODO linguEventListeners(getVoikkoMutex()),
		self.__messageLanguage = "en_US"
		VoikkoHandlePool.getInstance().setInstallationPath(self.__getInstallationPath())
		logging.debug("PropertyManager.__init__")
		self.__linguPropSet = None
		self.__hyphMinLeading = 2
		self.__hyphMinTrailing = 2
		self.__hyphMinWordLength = 5
		self.__hyphWordParts = False
		self.__hyphUnknownWords = True
		try:
			dictVariant = self.readFromRegistry("/org.puimula.ooovoikko.Config/dictionary", "variant")
			## TODO "Any"?
			VoikkoHandlePool.getInstance().setPreferredGlobalVariant(dictVariant)
			logging.debug("Initial dictionary variant '" + dictVariant + "'")
		except UnknownPropertyException as e:
			logging.debug("Setting initial dictionary variant to default")
			VoikkoHandlePool.getInstance().setPreferredGlobalVariant("")
		self.initialize()

	def __setUiLanguage(self):
		pass # TODO

	def initialize(self):
		logging.debug("PropertyManager.initialize: starting")
		self.__setUiLanguage()

		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_IGNORE_DOT, True)
		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_NO_UGLY_HYPHENATION, True)

		# Set these options globally until OOo bug #97945 is resolved.
		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_ACCEPT_TITLES_IN_GC, True)
		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_ACCEPT_BULLETED_LISTS_IN_GC, True)

		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_ACCEPT_UNFINISHED_PARAGRAPHS_IN_GC, True)

		compContext = uno.getComponentContext()
		servManager = compContext.ServiceManager
		self.__linguPropSet = servManager.createInstanceWithContext("com.sun.star.linguistic2.LinguProperties", compContext)
		self.__linguPropSet.addPropertyChangeListener("IsSpellWithDigits", self)
		self.__linguPropSet.addPropertyChangeListener("IsSpellUpperCase", self)
		logging.debug("PropertyManager.initialize: property manager initalized")

		# synchronize the local settings from global preferences
		self.__setProperties(self.__linguPropSet)
		self.readVoikkoSettings()
		# request that all users of linguistic services run the spellchecker and hyphenator
		# again with updated settings
		event = LinguServiceEvent()
		event.nEvent = SPELL_CORRECT_WORDS_AGAIN | SPELL_WRONG_WORDS_AGAIN | HYPHENATE_AGAIN | PROOFREAD_AGAIN
		self.__sendLinguEvent(event)

	def getHyphMinLeading(self):
		return self.__hyphMinLeading

	def getHyphMinTrailing(self):
		return self.__hyphMinTrailing

	def getHyphMinWordLength(self):
		return self.__hyphMinWordLength

	def readVoikkoSettings(self):
		pass # TODO

	def __getInstallationPath(self):
		dname = os.path.dirname(sys.modules[__name__].__file__)
		if dname.endswith("pythonpath"):
			dname = dname[:-11]
			logging.debug("PropertyManager.getInstallationPath: '" + dname + "'")
			return dname
		else:
			logging.debug("PropertyManager.getInstallationPath: not using unexpect '" + dname + "'")
			return None

	def readFromRegistry(self, group, key):
		# TODO
		return ""

	def getMessageLanguage(self):
		return self.__messageLanguage

	def __setProperties(self, properties):
		pass # TODO

	def setValues(self, values):
		for v in values:
			self.setValue(v)

	def resetValues(self, values):
		for v in values:
			globalV = PropertyValue()
			globalV.Name = v.Name
			globalV.Value = self.__linguPropSet.getPropertyValue(v.Name)
			self.setValue(globalV)

	def setValue(self, value):
		if value.Name == "IsSpellWithDigits":
			VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_IGNORE_NUMBERS, not value.Value)
		elif value.Name == "IsSpellUpperCase":
			VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_IGNORE_UPPERCASE, not value.Value)
		elif value.Name == "HyphMinLeading":
			if value.Value is not None:
				self.__hyphMinLeading = value.Value
				self.__syncHyphenatorSettings()
		elif value.Name == "HyphMinTrailing":
			if value.Value is not None:
				self.__hyphMinTrailing = value.Value
				self.__syncHyphenatorSettings()
		elif value.Name == "HyphMinWordLength":
			if value.Value is not None:
				self.__hyphMinWordLength = value.Value
				self.__syncHyphenatorSettings()

	def __syncHyphenatorSettings(self):
		if self.__hyphWordParts:
			VoikkoHandlePool.getInstance().setGlobalIntegerOption(PropertyManager.VOIKKO_MIN_HYPHENATED_WORD_LENGTH, self.__hyphMinWordLength)
		else:
			VoikkoHandlePool.getInstance().setGlobalIntegerOption(PropertyManager.VOIKKO_MIN_HYPHENATED_WORD_LENGTH, 2)
		VoikkoHandlePool.getInstance().setGlobalBooleanOption(PropertyManager.VOIKKO_OPT_HYPHENATE_UNKNOWN_WORDS, self.__hyphUnknownWords)

	def __sendLinguEvent(self, event):
		pass # TODO

	def getInstance():
		if PropertyManager.instance is None:
			PropertyManager.instance = PropertyManager()
		return PropertyManager.instance
	getInstance = staticmethod(getInstance)

PropertyManager.instance = None
PropertyManager.VOIKKO_OPT_IGNORE_NUMBERS = 1
PropertyManager.VOIKKO_OPT_IGNORE_UPPERCASE = 3
PropertyManager.VOIKKO_MIN_HYPHENATED_WORD_LENGTH = 9
PropertyManager.VOIKKO_OPT_HYPHENATE_UNKNOWN_WORDS = 15
PropertyManager.VOIKKO_OPT_IGNORE_DOT = 0
PropertyManager.VOIKKO_OPT_NO_UGLY_HYPHENATION = 4
PropertyManager.VOIKKO_OPT_ACCEPT_TITLES_IN_GC = 13
PropertyManager.VOIKKO_OPT_ACCEPT_BULLETED_LISTS_IN_GC = 16
PropertyManager.VOIKKO_OPT_ACCEPT_UNFINISHED_PARAGRAPHS_IN_GC = 14
