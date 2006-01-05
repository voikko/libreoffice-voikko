/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@cc.jyu.fi), 2005
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301, USA
 *
 ************************************************************************/ 

#include "XPossibleHyphens_impl.hxx"
#include "lfmacros.hxx"

namespace LF_NAMESPACE {

PossibleHyphens::PossibleHyphens(OUString wrd, OUString hypWrd, Sequence < sal_Int16 > positions) {
  word = wrd;
  hyphenatedWord = hypWrd;
  hyphenationPositions = positions;
}

OUString SAL_CALL PossibleHyphens::getWord() throw (RuntimeException) {
  return word;
}

::com::sun::star::lang::Locale SAL_CALL PossibleHyphens::getLocale() throw (RuntimeException) {
  return ::com::sun::star::lang::Locale(A2OU("fi"), A2OU("FI"), A2OU(""));
}

OUString SAL_CALL PossibleHyphens::getPossibleHyphens() throw (RuntimeException) {
  return hyphenatedWord;
}

Sequence < sal_Int16 > SAL_CALL PossibleHyphens::getHyphenationPositions() throw (RuntimeException) {
  return hyphenationPositions;
}

}
