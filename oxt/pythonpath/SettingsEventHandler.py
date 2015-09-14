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

class SettingsEventHandler(unohelper.Base):

	def __init__(self, ctx, *args):
		print("SettingsEventHandler.__init__")

SettingsEventHandler.IMPLEMENTATION_NAME = "org.puimula.ooovoikko.SettingsEventHandlerImplementation"
SettingsEventHandler.SUPPORTED_SERVICE_NAMES = ("org.puimula.ooovoikko.SettingsEventHandlerService",)
