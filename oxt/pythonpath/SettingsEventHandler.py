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

import unohelper
import logging
from com.sun.star.lang import XServiceInfo, XInitialization, XServiceDisplayName
from com.sun.star.awt import XContainerWindowEventHandler
from com.sun.star.beans import UnknownPropertyException
from PropertyManager import PropertyManager

class SettingsEventHandler(unohelper.Base, XServiceInfo, XContainerWindowEventHandler):

	def __init__(self, ctx, *args):
		print("SettingsEventHandler.__init__")

	# From XServiceInfo
	def getImplementationName(self):
		return SettingsEventHandler.IMPLEMENTATION_NAME

	# From XContainerWindowEventHandler
	def callHandlerMethod(self, xWindow, eventObject, methodName):
		if methodName != "external_event":
			return False
		if eventObject == "ok":
			self.__saveOptionsFromWindowToRegistry(xWindow)
			PropertyManager.getInstance().reloadVoikkoSettings()
			return True
		if eventObject == "back" or eventObject == "initialize":
			self.__initOptionsWindowFromRegistry(xWindow)
			return True
		return False

	def getSupportedMethodNames(self):
		return ("external_event",)

	def __initOptionsWindowFromRegistry(self, window):
		logging.debug("initOptionsWindowFromRegistry()");
		hyphWordPartsValue = False
		hyphUnknownWordsValue = True
		try:
			hyphWordPartsValue = PropertyManager.getInstance().readFromRegistry("/org.puimula.ooovoikko.Config/hyphenator",  "hyphWordParts")
			hyphUnknownWordsValue = PropertyManager.getInstance().readFromRegistry("/org.puimula.ooovoikko.Config/hyphenator", "hyphUnknownWords")
		except UnknownPropertyException as e:
			logging.exception("SettingsEventHandler: UnknownPropertyException", e)
			return
		logging.debug("hyphWordParts = " + str(hyphWordPartsValue))
		hyphWordParts = window.getControl("hyphWordParts")

		hyphWordPartsProps = hyphWordParts.getModel()
		hyphWordPartsProps.setPropertyValue("State", 1 if hyphWordPartsValue else 0)

		hyphUnknownWords = window.getControl("hyphUnknownWords")
		hyphUnknownWordsProps = hyphUnknownWords.getModel()
		hyphUnknownWordsProps.setPropertyValue("State", 1 if hyphUnknownWordsValue else 0)

		self.__initVariantDropdown(window)

SettingsEventHandler.IMPLEMENTATION_NAME = "org.puimula.ooovoikko.SettingsEventHandlerImplementation"
SettingsEventHandler.SUPPORTED_SERVICE_NAMES = ("org.puimula.ooovoikko.SettingsEventHandlerService",)
