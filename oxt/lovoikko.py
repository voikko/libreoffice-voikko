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

import os
import uno
import sys
import logging
import unohelper
from com.sun.star.awt.MessageBoxType import ERRORBOX
from com.sun.star.awt.MessageBoxButtons import BUTTONS_OK
from libvoikko import Voikko
from SettingsEventHandler import SettingsEventHandler
from SpellChecker import SpellChecker
from Hyphenator import Hyphenator
from GrammarChecker import GrammarChecker
from PropertyManager import PropertyManager

if "VOIKKO_DEBUG" in os.environ:
	logging.getLogger().setLevel(logging.DEBUG)

def messageBox(messageText):
	ctx = uno.getComponentContext()
	sManager = ctx.ServiceManager
	toolkit = sManager.createInstance("com.sun.star.awt.Toolkit")
	msgbox = toolkit.createMessageBox(None, ERRORBOX, BUTTONS_OK, "Error initializing Voikko", messageText)
	return msgbox.execute()

if not PropertyManager.loadingFailed:
	try:
		# Force initialization of property manager so that it is done before anything else.
		PropertyManager.getInstance()
		# We could check for specific version but this at least ensures that libvoikko is installed
		# (this would throw an exception if it's not).
		Voikko.getVersion()
		# name of g_ImplementationHelper is significant, Python component loader expects to find it
		g_ImplementationHelper = unohelper.ImplementationHelper()
		g_ImplementationHelper.addImplementation(SettingsEventHandler, \
		                    SettingsEventHandler.IMPLEMENTATION_NAME,
		                    SettingsEventHandler.SUPPORTED_SERVICE_NAMES,)
		g_ImplementationHelper.addImplementation(SpellChecker, \
		                    SpellChecker.IMPLEMENTATION_NAME,
		                    SpellChecker.SUPPORTED_SERVICE_NAMES,)
		g_ImplementationHelper.addImplementation(Hyphenator, \
		                    Hyphenator.IMPLEMENTATION_NAME,
		                    Hyphenator.SUPPORTED_SERVICE_NAMES,)
		g_ImplementationHelper.addImplementation(GrammarChecker, \
		                    GrammarChecker.IMPLEMENTATION_NAME,
		                    GrammarChecker.SUPPORTED_SERVICE_NAMES,)
	except OSError as e:
		PropertyManager.loadingFailed = True
		messageBox("OSError: {0}".format(e))
	except:
		PropertyManager.loadingFailed = True
		messageBox(str(sys.exc_info()[0]))
