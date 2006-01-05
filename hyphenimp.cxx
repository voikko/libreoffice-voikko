/*************************************************************************
 *
 *  Authors:
 *  Sun Microsystems Inc., October, 2000
 *  Pasi Ryhänen, 2002/08/15
 *  Harri Pitkänen (hatapitk@cc.jyu.fi), 2005
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
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

#include <cppuhelper/factory.hxx>
#include <rtl/ustring.hxx>
#include <rtl/ustrbuf.hxx>
#include <osl/file.hxx>

#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/linguistic2/XSearchableDictionaryList.hpp>
#include <com/sun/star/registry/XRegistryKey.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/PropertyState.hpp>
#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>

#include "hyphenimp.hxx"

using namespace osl;
using namespace rtl;
using namespace com::sun::star;
using namespace com::sun::star::beans;
using namespace com::sun::star::lang;
using namespace com::sun::star::uno;
using namespace com::sun::star::linguistic2;



///////////////////////////////////////////////////////////////////////////

namespace LF_NAMESPACE {

Hyphenator::Hyphenator() :
  aEvtListeners	( GetLinguMutex() )
{
  LF_LOG(("Hyphenator()\n"));
  lfInitHyphenator();
  bDisposing = FALSE;
}


Hyphenator::~Hyphenator()
{
  LF_LOG(("~Hyphenator()\n"));
}


Sequence< Locale > SAL_CALL 
Hyphenator::getLocales()
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
  LF_LOG(("Hyphenator::getLocales()\n"));
	
  if (!aSuppLocales.getLength())
    {

      aSuppLocales.realloc( 1 );
      Locale *pLocale = aSuppLocales.getArray();
      pLocale[0] = Locale( A2OU("fi"), A2OU("FI"), OUString() );

    }

  return aSuppLocales;
}


sal_Bool SAL_CALL 
Hyphenator::hasLocale(const Locale& rLocale)
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
  
  LF_LOG(("Hyphenator::hasLocale()\n"));
  sal_Bool bRes = FALSE;
  if (!aSuppLocales.getLength())
    getLocales();

  sal_Int32 nLen = aSuppLocales.getLength();
  for (sal_Int32 i = 0;  i < nLen;  ++i)
    {
      const Locale *pLocale = aSuppLocales.getConstArray();
      if (rLocale == pLocale[i])
	{
	  bRes = TRUE;
	  break;
	}
    }
  return bRes;
}


Reference< XHyphenatedWord > SAL_CALL 
Hyphenator::hyphenate( const ::rtl::OUString& aWord,
		       const ::com::sun::star::lang::Locale& aLocale,
		       sal_Int16 nMaxLeading, 
		       const ::com::sun::star::beans::PropertyValues& aProperties ) 
  throw(::com::sun::star::lang::IllegalArgumentException,
	::com::sun::star::uno::RuntimeException)
{ 
	return lfHyphenate(aWord, nMaxLeading);
}


Reference < XHyphenatedWord > SAL_CALL
Hyphenator::queryAlternativeSpelling( const ::rtl::OUString& aWord,
				      const ::com::sun::star::lang::Locale& aLocale,
				      sal_Int16 nIndex,
				      const ::com::sun::star::beans::PropertyValues& aProperties )
  throw(::com::sun::star::lang::IllegalArgumentException,
	::com::sun::star::uno::RuntimeException)
{
  LF_LOG(("Hyphenator::queryAlternativeSpellint()\n"));
  return NULL;
}


// Find possible hyphenation points for the user.
// The user selects the best one.
Reference< XPossibleHyphens > SAL_CALL
Hyphenator::createPossibleHyphens( const ::rtl::OUString& aWord,
				   const ::com::sun::star::lang::Locale& aLocale,
				   const ::com::sun::star::beans::PropertyValues& aProperties )
  throw(::com::sun::star::lang::IllegalArgumentException,
	::com::sun::star::uno::RuntimeException) {
	return lfCreatePossibleHyphens(aWord);
}


OUString SAL_CALL Hyphenator::getServiceDisplayName( const Locale& rLocale )
  throw(RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	return lfHyphenatorDisplayName(rLocale);
}


void SAL_CALL 
Hyphenator::initialize( const Sequence< Any >& rArguments ) 
  throw(Exception, RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  LF_LOG(("Hyphenator::initialize()\n"));
  lfInitHyphenator();
}


void SAL_CALL Hyphenator::dispose() throw(RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	LF_LOG(("Hyphenator::dispose()\n"));
	if (!bDisposing) {
		bDisposing = TRUE;
		EventObject aEvtObj( (XHyphenator *) this );
		aEvtListeners.disposeAndClear( aEvtObj );
	}
	lfDisposeHyphenator();
}


void SAL_CALL 
Hyphenator::addEventListener( const Reference< XEventListener >& rxListener ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  if (!bDisposing && rxListener.is())
    aEvtListeners.addInterface( rxListener );
}


void SAL_CALL 
Hyphenator::removeEventListener( const Reference< XEventListener >& rxListener ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  if (!bDisposing && rxListener.is())
    aEvtListeners.removeInterface( rxListener );
}


///////////////////////////////////////////////////////////////////////////
// Service specific part
//

OUString SAL_CALL Hyphenator::getImplementationName() 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  return getImplementationName_Static();
}


sal_Bool SAL_CALL Hyphenator::supportsService( const OUString& ServiceName )
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  Sequence< OUString > aSNL = getSupportedServiceNames();
  const OUString * pArray = aSNL.getConstArray();
  for( sal_Int32 i = 0; i < aSNL.getLength(); i++ )
    if( pArray[i] == ServiceName )
      return TRUE;
  return FALSE;
}


Sequence< OUString > SAL_CALL Hyphenator::getSupportedServiceNames()
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  return getSupportedServiceNames_Static();
}


Sequence< OUString > Hyphenator::getSupportedServiceNames_Static() 
  throw()
{
  MutexGuard	aGuard( GetLinguMutex() );

  Sequence< OUString > aSNS( 1 );
  aSNS.getArray()[0] = A2OU( "com.sun.star.linguistic2.Hyphenator" );
  return aSNS;
}

}

