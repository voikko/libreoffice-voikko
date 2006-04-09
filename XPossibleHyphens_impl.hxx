/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@iki.fi), 2005
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

#ifndef _XPOSSIBLEHYPHENS_IMPL_HXX_
#define _XPOSSIBLEHYPHENS_IMPL_HXX_

#include <com/sun/star/linguistic2/XPossibleHyphens.hpp>
#include <cppuhelper/implbase1.hxx>

using namespace ::com::sun::star::linguistic2;
using namespace ::com::sun::star::uno;
using namespace ::rtl;

namespace LF_NAMESPACE {

class PossibleHyphens :
  public cppu::WeakImplHelper1 < XPossibleHyphens >
{
public:
  OUString word;
  OUString hyphenatedWord;
  Sequence < sal_Int16 > hyphenationPositions;
  PossibleHyphens(OUString wrd, OUString hypWrd, Sequence < sal_Int16 > positions);
  virtual OUString SAL_CALL getWord() throw (RuntimeException);
  virtual ::com::sun::star::lang::Locale SAL_CALL getLocale() throw (RuntimeException);
  virtual OUString SAL_CALL getPossibleHyphens() throw (RuntimeException);
  virtual Sequence < sal_Int16 > SAL_CALL getHyphenationPositions() throw (RuntimeException);

};

}

#endif
