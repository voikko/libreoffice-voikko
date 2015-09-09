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

from libvoikko import Voikko

class VoikkoHandlePool:

	def __init__(self):
		self.__supportedSpellingLocales = ()
		self.__installationPath = None

	def getInstance():
		if VoikkoHandlePool.instance is None:
			VoikkoHandlePool.instance = VoikkoHandlePool()
		return VoikkoHandlePool.instance
	getInstance = staticmethod(getInstance)

	def getInstallationPath(self):
		return self.__installationPath

	def __addLocale(self, locales, language):
		pass # TODO

	def getSupportedSpellingLocales(self):
		# optimization: if we already have found some locales, don't search for more
		if len(self.__supportedSpellingLocales) == 0:
			languages = Voikko.listSupportedSpellingLanguages(self.getInstallationPath())
			for lang in languages:
				self.__addLocale(self.__supportedSpellingLocales, lang)
		return self.__supportedSpellingLocales


VoikkoHandlePool.instance = None
