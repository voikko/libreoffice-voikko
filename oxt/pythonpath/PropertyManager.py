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
from VoikkoHandlePool import VoikkoHandlePool
from com.sun.star.beans import XPropertyChangeListener, UnknownPropertyException

class PropertyManager(unohelper.Base, XPropertyChangeListener):

	def __init__(self):
		# TODO linguEventListeners(getVoikkoMutex()),
		self.__messageLanguage = "en_US"
		VoikkoHandlePool.getInstance().setInstallationPath(self.__getInstallationPath())
		logging.debug("PropertyManager.__init__")
		self.__linguPropSet = 0
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

	def initialize(self):
		logging.debug("PropertyManager.initialize: starting")
		# TODO

	def getHyphMinLeading(self):
		return self.__hyphMinLeading

	def getHyphMinTrailing(self):
		return self.__hyphMinTrailing

	def getHyphMinWordLength(self):
		return self.__hyphMinWordLength

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

	def setValues(self, values):
		pass # TODO

	def getInstance():
		if PropertyManager.instance is None:
			PropertyManager.instance = PropertyManager()
		return PropertyManager.instance
	getInstance = staticmethod(getInstance)

PropertyManager.instance = None
