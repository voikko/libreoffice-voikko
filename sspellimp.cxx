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

/*
 * This file is originally taken from linguistic/workben/sspellimp.cxx.
 * Modifications by Pasi Ryhänen <pry@iki.fi> 2002
 *
 * Disclaimer: the changes/additions are made without much knowledge
 * about the existing OpenOffice source code and practices, so there
 * may be stupid errors. All feedback is welcome.
 *
 * TODO:
 * - Handle different character sets (Latin 1, Latin 9, Windows?) correctly.
 *   initialize() calls gsl_getSystemTextEncoding(), but do we need
 *   to do anything else?
 * - Should we check (and use) the Locale parameter in every function?
 */

 
#include <cstdio>
#include <cppuhelper/factory.hxx>
#include <osl/file.hxx>
#include <cppuhelper/bootstrap.hxx>

#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/linguistic2/XSearchableDictionaryList.hpp>
#include <com/sun/star/linguistic2/SpellFailure.hpp>
#include <com/sun/star/registry/XRegistryKey.hpp>

#include "sspellimp.hxx"

using namespace osl;
using namespace rtl;
using namespace cppu;
using namespace com::sun::star;
using namespace com::sun::star::beans;
using namespace com::sun::star::lang;
using namespace com::sun::star::uno;
using namespace com::sun::star::linguistic2;


///////////////////////////////////////////////////////////////////////////

namespace LF_NAMESPACE {

SpellChecker::SpellChecker() :
  aEvtListeners	( GetLinguMutex() )
{
  LF_LOG(("SpellChecker()\n"));
  lfInitSpeller();
  bDisposing = FALSE;
  pPropHelper = NULL;
}


SpellChecker::~SpellChecker()
{
  LF_LOG(("~SpellChecker()\n"));
  if (pPropHelper)
    pPropHelper->RemoveAsPropListener();
}


PropertyHelper_Spell & 
SpellChecker::GetPropHelper_Impl()
{
  LF_LOG(("GetProperHelper_Impl()\n"));
  if (!pPropHelper)
    {
      Reference< XComponentContext > compContext = defaultBootstrap_InitialComponentContext();
      Reference< XMultiComponentFactory > servManager = compContext->getServiceManager();
      Reference< XInterface > iFace = servManager->createInstanceWithContext(
           A2OU("com.sun.star.configuration.ConfigurationProvider"), compContext);
      Reference< XMultiServiceFactory > provider(iFace, UNO_QUERY);
      Reference< XInterface > iFace2 = provider->createInstance(A2OU("com.sun.star.linguistic2.LinguProperties"));
      Reference< XPropertySet >	xPropSet( iFace2, UNO_QUERY );

      pPropHelper = new PropertyHelper_Spell( (XSpellChecker *) this, xPropSet );
      xPropHelper = pPropHelper;
      pPropHelper->AddAsPropListener();	//! after a reference is established
    }
  return *pPropHelper;
}


Sequence< Locale > SAL_CALL 
SpellChecker::getLocales()
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  LF_LOG(("getLocales()\n"));
  if (!aSuppLocales.getLength())
    {
      aSuppLocales.realloc( 1 );
      Locale *pLocale = aSuppLocales.getArray();
      pLocale[0] = Locale( A2OU("fi"), A2OU("FI"), OUString() );
    }

  return aSuppLocales;
}


sal_Bool SAL_CALL 
SpellChecker::hasLocale(const Locale& rLocale)
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

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


sal_Bool SAL_CALL 
SpellChecker::isValid( const OUString& rWord, const Locale& rLocale, 
		       const PropertyValues& rProperties ) 
  throw(IllegalArgumentException, RuntimeException)
{
	return lfIsValid(rWord, TRUE, TRUE); //FIXME
}


// Retrieves the return values for the 'spell' function call in case
// of a misspelled word.
// Especially it may give a list of suggested (correct) words.
/*Reference< XSpellAlternatives >
SpellChecker::GetProposals( const OUString &rWord, const Locale &rLocale )
{
	return lfGetProposals(rWord);
}*/


Reference< XSpellAlternatives > SAL_CALL 
SpellChecker::spell( const OUString& rWord, const Locale& rLocale, 
		     const PropertyValues& rProperties ) 
  throw(IllegalArgumentException, RuntimeException) {
	return lfSpell(rWord, TRUE, TRUE); //FIXME
}


    
	
sal_Bool SAL_CALL 
SpellChecker::addLinguServiceEventListener( 
					   const Reference< XLinguServiceEventListener >& rxLstnr ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  sal_Bool bRes = FALSE;   
  if (!bDisposing && rxLstnr.is())
    {
      bRes = GetPropHelper().addLinguServiceEventListener( rxLstnr );
    }
  return bRes;
}


sal_Bool SAL_CALL 
SpellChecker::removeLinguServiceEventListener( 
					      const Reference< XLinguServiceEventListener >& rxLstnr ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  sal_Bool bRes = FALSE;   
  if (!bDisposing && rxLstnr.is())
    {
      LF_ASSERT( xPropHelper.is(), "xPropHelper non existent" );
      bRes = GetPropHelper().removeLinguServiceEventListener( rxLstnr );
    }
  return bRes;
}


OUString SAL_CALL SpellChecker::getServiceDisplayName( const Locale& rLocale )
	throw(RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	return lfSpellerDisplayName(rLocale);
}


void SAL_CALL 
SpellChecker::initialize( const Sequence< Any >& rArguments ) 
  throw(Exception, RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  LF_LOG(("initialize()\n"));
  if (!pPropHelper)
    {
      sal_Int32 nLen = rArguments.getLength();
      if (2 == nLen)
	{
	  Reference< XPropertySet > xPropSet;
	  rArguments.getConstArray()[0] >>= xPropSet;
	  //rArguments.getConstArray()[1] >>= xDicList;

	  //! Pointer allows for access of the non-UNO functions.
	  //! And the reference to the UNO-functions while increasing
	  //! the ref-count and will implicitly free the memory
	  //! when the object is not longer used.
	  pPropHelper = new PropertyHelper_Spell( (XSpellChecker *) this, xPropSet );
	  xPropHelper = pPropHelper;
	  pPropHelper->AddAsPropListener();	//! after a reference is established
	}
      else
	      LF_LOG(("SpellChecker::initialize: Error: wrong number of arguments in sequence\n"));
    }
  lfInitSpeller();
}




void SAL_CALL SpellChecker::dispose() throw(RuntimeException) {
	MutexGuard aGuard(GetLinguMutex());
	LF_LOG(("dispose()\n"));
	if (!bDisposing) {
		bDisposing = TRUE;
		EventObject aEvtObj( (XSpellChecker *) this );
		aEvtListeners.disposeAndClear( aEvtObj );
	}
	lfDisposeSpeller();
}


void SAL_CALL 
SpellChecker::addEventListener( const Reference< XEventListener >& rxListener ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  LF_LOG(("addEventListener()\n"));
  if (!bDisposing && rxListener.is())
    aEvtListeners.addInterface( rxListener );
}


void SAL_CALL 
SpellChecker::removeEventListener( const Reference< XEventListener >& rxListener ) 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
	
  LF_LOG(("removeEventListener()\n"));
  if (!bDisposing && rxListener.is())
    aEvtListeners.removeInterface( rxListener );
}


///////////////////////////////////////////////////////////////////////////
// Service specific part
//

OUString SAL_CALL 
SpellChecker::getImplementationName() 
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
  LF_LOG(("getImplementationName()\n"));
  return getImplementationName_Static();
}


sal_Bool SAL_CALL 
SpellChecker::supportsService( const OUString& ServiceName )
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );

  LF_LOG(("supportsService()\n"));
  Sequence< OUString > aSNL = getSupportedServiceNames();
  const OUString * pArray = aSNL.getConstArray();
  for( sal_Int32 i = 0; i < aSNL.getLength(); i++ )
    if( pArray[i] == ServiceName )
      return TRUE;
  return FALSE;
}


Sequence< OUString > SAL_CALL 
SpellChecker::getSupportedServiceNames()
  throw(RuntimeException)
{
  MutexGuard	aGuard( GetLinguMutex() );
  LF_LOG(("getSupportedServiceNames()\n"));
  return getSupportedServiceNames_Static();
}


Sequence< OUString > 
SpellChecker::getSupportedServiceNames_Static() 
{
  LF_LOG(("getSupportedServiceNames_Static()\n"));
  Sequence< OUString > aSNS( 1 );
  aSNS.getArray()[0] = A2OU("com.sun.star.linguistic2.SpellChecker");
  return aSNS;
}

}


