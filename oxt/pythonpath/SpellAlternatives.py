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
from com.sun.star.linguistic2 import XSpellAlternatives
from com.sun.star.linguistic2.SpellFailure import SPELLING_ERROR

class SpellAlternatives(unohelper.Base, XSpellAlternatives):

	def __init__(self, word, alternatives, locale):
		self.__word = word
		self.__alternatives = alternatives
		self.__locale = locale

	def getWord(self):
		return self.__word

	def getLocale(self):
		return self.__locale

	def getFailureType(self):
		# If Voikko supported specific failure types, we could use them here
		return SPELLING_ERROR

	def getAlternativesCount(self):
		return len(self.__alternatives)

	def getAlternatives(self):
		return tuple(self.__alternatives)
