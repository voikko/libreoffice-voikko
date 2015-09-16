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
from com.sun.star.linguistic2 import XHyphenatedWord

class HyphenatedWord(unohelper.Base, XHyphenatedWord):

	def __init__(self, word, pos, locale):
		self.__word = word
		self.__hyphenPos = pos
		self.__locale = locale
		if word[pos + 1] == '-':
			self.__hyphenatedWord = word[:pos+1] + word[pos+2:]
			self.__isAlternative = True
		else:
			self.__hyphenatedWord = word
			self.__isAlternative = False
		logging.debug("HyphenatedWord.__init__: " + word)
		logging.debug("HyphenatedWord.__init__: " + str(pos))

	def getWord(self):
		return self.__word

	def getHyphenatedWord(self):
		return self.__hyphenatedWord

	def getLocale(self):
		return self.__locale

	def getHyphenationPos(self):
		return self.__hyphenPos

	def getHyphenPos(self):
		return self.__hyphenPos

	def isAlternativeSpelling(self):
		return self.__isAlternative
