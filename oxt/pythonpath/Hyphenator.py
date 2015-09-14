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

class Hyphenator(unohelper.Base, XServiceInfo, XHyphenator, XLinguServiceEventBroadcaster, XInitialization, XServiceDisplayName):

	def __init__(self, ctx, *args):
		logging.debug("Hyphenator.__init__")

	# From XServiceInfo
	def getImplementationName(self):
		return Hyphenator.IMPLEMENTATION_NAME

	# From XSupportedLocales
	def getLocales(self):
		return VoikkoHandlePool.getInstance().getSupportedHyphenationLocales()

	def hasLocale(self, aLocale):
		return VoikkoHandlePool.getInstance().supportsHyphenationLocale(aLocale)

	# From XLinguServiceEventBroadcaster
	def addLinguServiceEventListener(self, xLstnr):
		logging.debug("Hyphenator.addLinguServiceEventListener")
		return False # TODO

Hyphenator.IMPLEMENTATION_NAME = "voikko.Hyphenator"
Hyphenator.SUPPORTED_SERVICE_NAMES = ("com.sun.star.linguistic2.Hyphenator",)
