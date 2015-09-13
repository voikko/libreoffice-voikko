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
import logging
import unohelper
from SettingsEventHandler import SettingsEventHandler
from SpellChecker import SpellChecker
from PropertyManager import PropertyManager

if "VOIKKO_DEBUG" in os.environ:
	logging.getLogger().setLevel(logging.DEBUG)

# name of g_ImplementationHelper is significant, Python component loader expects to find it
g_ImplementationHelper = unohelper.ImplementationHelper()
g_ImplementationHelper.addImplementation(SettingsEventHandler, \
                    "org.puimula.ooovoikko.SettingsEventHandlerImplementation",
                    ("org.puimula.ooovoikko.SettingsEventHandlerService",),)
g_ImplementationHelper.addImplementation(SpellChecker, \
                    SpellChecker.IMPLEMENTATION_NAME,
                    SpellChecker.SUPPORTED_SERVICE_NAMES,)

# Force initialization of property manager so that it is done before anything else
PropertyManager.getInstance()
