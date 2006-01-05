/*************************************************************************
 *
 *  Authors:
 *  Sun Microsystems Inc., October, 2000
 *  Pasi Ryhänen, 2002/04/03
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


#include <com/sun/star/linguistic2/LinguServiceEvent.hpp>
#include <com/sun/star/linguistic2/LinguServiceEventFlags.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventListener.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <osl/mutex.hxx>

#include "sprophelp.hxx"
#include "lfmacros.hxx"

using namespace osl;
using namespace rtl;
using namespace com::sun::star;
using namespace com::sun::star::beans;
using namespace com::sun::star::lang;
using namespace com::sun::star::uno;
using namespace com::sun::star::linguistic2;


///////////////////////////////////////////////////////////////////////////

namespace LF_NAMESPACE {

PropertyChgHelper::PropertyChgHelper(
		const Reference< XInterface > & rxSource,
		Reference< XPropertySet > &rxPropSet,
		const char *pPropNames[], sal_uInt16 nPropCount ) :
	xMyEvtObj			(rxSource),
	xPropSet			(rxPropSet),
	aPropNames			(nPropCount),
	aLngSvcEvtListeners	(GetLinguMutex())
{
	OUString *pName = aPropNames.getArray();
	for (sal_Int32 i = 0;  i < nPropCount;  ++i)
	{
		pName[i] = A2OU( pPropNames[i] );
	}
}


PropertyChgHelper::PropertyChgHelper( const PropertyChgHelper &rHelper ) :
	aLngSvcEvtListeners	(GetLinguMutex())
{
	xPropSet	= rHelper.xPropSet;
	aPropNames	= rHelper.aPropNames;
	AddAsPropListener();
	
	xMyEvtObj	= rHelper.xMyEvtObj;
}


PropertyChgHelper::~PropertyChgHelper()
{	
}


void PropertyChgHelper::AddAsPropListener()
{
	if (xPropSet.is())
	{
		sal_Int32 nLen = aPropNames.getLength();
		const OUString *pPropName = aPropNames.getConstArray();
		for (sal_Int32 i = 0;  i < nLen;  ++i)
		{
			if (pPropName[i].getLength())
				xPropSet->addPropertyChangeListener( pPropName[i], this );
		}
	}
}

void PropertyChgHelper::RemoveAsPropListener()
{
	if (xPropSet.is())
	{
		sal_Int32 nLen = aPropNames.getLength();
		const OUString *pPropName = aPropNames.getConstArray();
		for (sal_Int32 i = 0;  i < nLen;  ++i)
		{
			if (pPropName[i].getLength())
				xPropSet->removePropertyChangeListener( pPropName[i], this );
		}
	}
}
		 

void PropertyChgHelper::LaunchEvent( const LinguServiceEvent &rEvt )
{
	cppu::OInterfaceIteratorHelper aIt( aLngSvcEvtListeners );
	while (aIt.hasMoreElements())
	{
		Reference< XLinguServiceEventListener > xRef( aIt.next(), UNO_QUERY );
		if (xRef.is())
			xRef->processLinguServiceEvent( rEvt );
	}
}


void SAL_CALL PropertyChgHelper::disposing( const EventObject& rSource ) 
		throw(RuntimeException)
{
	MutexGuard	aGuard( GetLinguMutex() );
	if (rSource.Source == xPropSet)
	{
		RemoveAsPropListener();
		xPropSet = NULL;
		aPropNames.realloc( 0 );
	}
}
    
	
sal_Bool SAL_CALL 
	PropertyChgHelper::addLinguServiceEventListener( 
			const Reference< XLinguServiceEventListener >& rxListener ) 
		throw(RuntimeException)
{
	MutexGuard	aGuard( GetLinguMutex() );
	
	sal_Bool bRes = FALSE;
	if (rxListener.is())
	{
		sal_Int32	nCount = aLngSvcEvtListeners.getLength();
		bRes = aLngSvcEvtListeners.addInterface( rxListener ) != nCount;
	}
	return bRes;
}


sal_Bool SAL_CALL 
	PropertyChgHelper::removeLinguServiceEventListener( 
			const Reference< XLinguServiceEventListener >& rxListener ) 
		throw(RuntimeException)
{
	MutexGuard	aGuard( GetLinguMutex() );
	
	sal_Bool bRes = FALSE;
	if (rxListener.is())
	{
		sal_Int32	nCount = aLngSvcEvtListeners.getLength();
		bRes = aLngSvcEvtListeners.removeInterface( rxListener ) != nCount;
	}
	return bRes;
}

///////////////////////////////////////////////////////////////////////////

static const char *aSP[] =
{
	UPN_IS_GERMAN_PRE_REFORM,
	UPN_IS_IGNORE_CONTROL_CHARACTERS,
	UPN_IS_USE_DICTIONARY_LIST,
	UPN_IS_SPELL_UPPER_CASE,
	UPN_IS_SPELL_WITH_DIGITS,
	UPN_IS_SPELL_CAPITALIZATION
};


PropertyHelper_Spell::PropertyHelper_Spell(
		const Reference< XInterface > & rxSource,
		Reference< XPropertySet > &rxPropSet ) :
	PropertyChgHelper	( rxSource, rxPropSet, aSP, sizeof(aSP) / sizeof(aSP[0]) )
{
	SetDefault();
	sal_Int32 nLen = GetPropNames().getLength();
	if (rxPropSet.is() && nLen)
	{
		const OUString *pPropName = GetPropNames().getConstArray();
		for (sal_Int32 i = 0;  i < nLen;  ++i)
		{
			sal_Bool *pbVal		= NULL,
				 *pbResVal	= NULL;

			if (A2OU( UPN_IS_GERMAN_PRE_REFORM ) == pPropName[i])
			{
				pbVal	 = &bIsGermanPreReform;
				pbResVal = &bResIsGermanPreReform;
			}
			else if (A2OU( UPN_IS_IGNORE_CONTROL_CHARACTERS ) == pPropName[i])
			{
				pbVal	 = &bIsIgnoreControlCharacters;
				pbResVal = &bResIsIgnoreControlCharacters;
			}
			else if (A2OU( UPN_IS_USE_DICTIONARY_LIST ) == pPropName[i])
			{
				pbVal	 = &bIsUseDictionaryList;
				pbResVal = &bResIsUseDictionaryList;
			}
			else if (A2OU( UPN_IS_SPELL_UPPER_CASE ) == pPropName[i])
			{
				pbVal	 = &bIsSpellUpperCase;
				pbResVal = &bResIsSpellUpperCase;
			}
			else if (A2OU( UPN_IS_SPELL_WITH_DIGITS ) == pPropName[i])
			{
				pbVal	 = &bIsSpellWithDigits;
				pbResVal = &bResIsSpellWithDigits;
			}
			else if (A2OU( UPN_IS_SPELL_CAPITALIZATION ) == pPropName[i])
			{
				pbVal	 = &bIsSpellCapitalization;
				pbResVal = &bResIsSpellCapitalization;
			}
			
			if (pbVal && pbResVal)
			{
				rxPropSet->getPropertyValue( pPropName[i] ) >>= *pbVal;
				*pbResVal = *pbVal;
			}
		}
	}
}


PropertyHelper_Spell::~PropertyHelper_Spell()
{
}


void PropertyHelper_Spell::SetDefault()
{
	bResIsGermanPreReform			= bIsGermanPreReform			= FALSE;
	bResIsIgnoreControlCharacters	= bIsIgnoreControlCharacters	= TRUE;
	bResIsUseDictionaryList			= bIsUseDictionaryList			= TRUE;
	bResIsSpellUpperCase			= bIsSpellUpperCase				= FALSE;
	bResIsSpellWithDigits			= bIsSpellWithDigits			= FALSE;
	bResIsSpellCapitalization		= bIsSpellCapitalization		= TRUE;
}


void SAL_CALL 
	PropertyHelper_Spell::propertyChange( const PropertyChangeEvent& rEvt ) 
		throw(RuntimeException)
{
	MutexGuard	aGuard( GetLinguMutex() );

	if (GetPropSet().is()  &&  rEvt.Source == GetPropSet())
	{
		sal_Int16 nLngSvcFlags = 0;
		sal_Bool bSCWA = FALSE,	// SPELL_CORRECT_WORDS_AGAIN ?
			 bSWWA = FALSE;	// SPELL_WRONG_WORDS_AGAIN ?

		sal_Bool *pbVal = NULL;
		switch (rEvt.PropertyHandle)
		{
			case UPH_IS_IGNORE_CONTROL_CHARACTERS :
			{
				pbVal = &bIsIgnoreControlCharacters; 
				break;
			}
			case UPH_IS_GERMAN_PRE_REFORM		  :
			{
				pbVal = &bIsGermanPreReform; 
				bSCWA = bSWWA = TRUE;
				break;
			}
			case UPH_IS_USE_DICTIONARY_LIST		  :
			{
				pbVal = &bIsUseDictionaryList; 
				bSCWA = bSWWA = TRUE;
				break;
			}
			case UPH_IS_SPELL_UPPER_CASE		  :
			{
				pbVal = &bIsSpellUpperCase;
				bSCWA = FALSE == *pbVal;	// FALSE->TRUE change?
				bSWWA = !bSCWA;				// TRUE->FALSE change?
				break;
			}
			case UPH_IS_SPELL_WITH_DIGITS		  :
			{
				pbVal = &bIsSpellWithDigits; 
				bSCWA = FALSE == *pbVal;	// FALSE->TRUE change?
				bSWWA = !bSCWA;				// TRUE->FALSE change?
				break;
			}
			case UPH_IS_SPELL_CAPITALIZATION	  :
			{
				pbVal = &bIsSpellCapitalization; 
				bSCWA = FALSE == *pbVal;	// FALSE->TRUE change?
				bSWWA = !bSCWA;				// TRUE->FALSE change?
				break;
			}
			default:
				LF_LOG(("PropertyHelper_Spell::propertyChange: Error: unknown property\n"));
		}
		if (pbVal)
			rEvt.NewValue >>= *pbVal;

		if (bSCWA)
			nLngSvcFlags |= LinguServiceEventFlags::SPELL_CORRECT_WORDS_AGAIN;
		if (bSWWA)
			nLngSvcFlags |= LinguServiceEventFlags::SPELL_WRONG_WORDS_AGAIN;
		if (nLngSvcFlags)
		{
			LinguServiceEvent aEvt( GetEvtObj(), nLngSvcFlags );
			LaunchEvent( aEvt );
		}
	}
}

		  
void PropertyHelper_Spell::SetTmpPropVals( const PropertyValues &rPropVals )
{
	// set return value to default value unless there is an 
	// explicitly supplied temporary value
	bResIsGermanPreReform			= bIsGermanPreReform;
	bResIsIgnoreControlCharacters	= bIsIgnoreControlCharacters;
	bResIsUseDictionaryList			= bIsUseDictionaryList;
	bResIsSpellUpperCase			= bIsSpellUpperCase;
	bResIsSpellWithDigits			= bIsSpellWithDigits;
	bResIsSpellCapitalization		= bIsSpellCapitalization;
	//
	sal_Int32 nLen = rPropVals.getLength();
	if (nLen)
	{
		const PropertyValue *pVal = rPropVals.getConstArray();
		for (sal_Int32 i = 0;  i < nLen;  ++i)
		{
			sal_Bool *pbResVal = NULL;
			switch (pVal[i].Handle)
			{
				case UPH_IS_GERMAN_PRE_REFORM		  : pbResVal = &bResIsGermanPreReform; break;
				case UPH_IS_IGNORE_CONTROL_CHARACTERS : pbResVal = &bResIsIgnoreControlCharacters; break;
				case UPH_IS_USE_DICTIONARY_LIST		  : pbResVal = &bResIsUseDictionaryList; break;
				case UPH_IS_SPELL_UPPER_CASE		  : pbResVal = &bResIsSpellUpperCase; break;
				case UPH_IS_SPELL_WITH_DIGITS		  : pbResVal = &bResIsSpellWithDigits; break;
				case UPH_IS_SPELL_CAPITALIZATION	  : pbResVal = &bResIsSpellCapitalization; break;
				default:
					LF_LOG(("PropertyHelper_Spell::SetTmpPropVals: Error: unknown property\n"));
			}
			if (pbResVal)
				pVal[i].Value >>= *pbResVal;
		}
	}
}

}

///////////////////////////////////////////////////////////////////////////

