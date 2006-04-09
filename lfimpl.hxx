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

#ifndef _LFIMPL_HXX_
#define _LFIMPL_HXX_

#include <com/sun/star/beans/PropertyValues.hpp>
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <com/sun/star/uno/RuntimeException.hpp>
#include <com/sun/star/linguistic2/SpellFailure.hpp>

#include "XSpellAlternatives_impl.hxx"
#include "XHyphenatedWord_impl.hxx"
#include "XPossibleHyphens_impl.hxx"

using namespace ::osl;
using namespace ::rtl;

namespace LF_NAMESPACE {

//static sal_Int16 lfGetSpellFailure(const OUString &rWord);
void lfInitSpeller();
void lfDisposeSpeller();
sal_Bool SAL_CALL lfIsValid(const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase) 
		throw(::com::sun::star::lang::IllegalArgumentException, ::com::sun::star::uno::RuntimeException);
//Reference< XSpellAlternatives > lfGetProposals(const OUString &rWord);
Reference< XSpellAlternatives > SAL_CALL lfSpell(const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase)
		throw(::com::sun::star::uno::RuntimeException);
OUString lfSpellerDisplayName(const Locale& rLocale);

void lfInitHyphenator();
void lfDisposeHyphenator();
Reference< XHyphenatedWord > SAL_CALL lfHyphenate(const ::rtl::OUString& aWord, sal_Int16 nMaxLeading)
		throw(::com::sun::star::uno::RuntimeException);
Reference< XPossibleHyphens > SAL_CALL lfCreatePossibleHyphens(const ::rtl::OUString& aWord)
		throw(::com::sun::star::uno::RuntimeException);
OUString lfHyphenatorDisplayName(const Locale& rLocale);
}

#endif
