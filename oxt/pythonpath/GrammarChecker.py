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
from com.sun.star.linguistic2 import XProofreader
from com.sun.star.lang import XServiceInfo, XInitialization, XServiceDisplayName
from VoikkoHandlePool import VoikkoHandlePool
from PropertyManager import PropertyManager

class GrammarChecker(unohelper.Base, XServiceInfo, XProofreader, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("GrammarChecker.__init__")
		self.__ignoredErrors = [] # Grammar checker error codes that should be ignored

	# From XServiceInfo
	def getImplementationName(self):
		return GrammarChecker.IMPLEMENTATION_NAME

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedGrammarLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsGrammarLocale(aLocale)

	# From XServiceDisplayName
	def getServiceDisplayName(self, locale):
		if locale.Language == "fi":
			return "Kieliopin tarkistus (Voikko)"
		else:
			return "Grammar checker (Voikko)"

GrammarChecker.IMPLEMENTATION_NAME = "voikko.GrammarChecker"
GrammarChecker.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.Proofreader",)
