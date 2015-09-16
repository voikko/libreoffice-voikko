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
from com.sun.star.linguistic2 import XPossibleHyphens

class PossibleHyphens(unohelper.Base, XPossibleHyphens):

	def __init__(self, word, hyphenatedWord, positions, locale):
		self.__word = word
		self.__hyphenatedWord = hyphenatedWord
		self.__hyphenationPositions = positions
		self.__locale = locale

	def getWord(self):
		return self.__word

	def getLocale(self):
		return self.__locale

	def getPossibleHyphens(self):
		return self.__hyphenatedWord

	def getHyphenationPositions(self):
		return tuple(self.__hyphenationPositions)
