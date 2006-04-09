/*************************************************************************
 *
 *  Authors:
 *  Sun Microsystems Inc., October, 2000
 *  Pasi Ryhänen, 2002/04/03
 *  Harri Pitkänen (hatapitk@iki.fi), 2005
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


#ifndef _SPROPHELP_HXX_
#define _SPROPHELP_HXX_

#include <cppuhelper/implbase2.hxx>
#include <cppuhelper/interfacecontainer.h>

#include <com/sun/star/beans/XPropertyChangeListener.hpp>
#include <com/sun/star/beans/PropertyValues.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventBroadcaster.hpp>


using namespace ::rtl;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::linguistic2;

///////////////////////////////////////////////////////////////////////////
// PropertyChgHelper
// virtual base class for all XPropertyChangeListener members of the
// various lingu services.
// Only propertyChange needs to be implemented.

namespace LF_NAMESPACE {

class PropertyChgHelper :
	public cppu::WeakImplHelper2
	<
		XLinguServiceEventBroadcaster,
		XPropertyChangeListener
	>
{
	Reference< XInterface > xMyEvtObj;
	Reference< XPropertySet > xPropSet;
	Sequence< OUString > aPropNames;
	::cppu::OInterfaceContainerHelper aLngSvcEvtListeners;

	// disallow use of copy-constructor and assignment-operator
	PropertyChgHelper( const PropertyChgHelper & );
	PropertyChgHelper & operator = ( const PropertyChgHelper & );

public:
	PropertyChgHelper( 
			const Reference< XInterface > &rxSource,
			Reference< XPropertySet > &rxPropSet,
			const char *pPropNames[], sal_uInt16 nPropCount );
	virtual ~PropertyChgHelper();

	// XEventListener
	virtual void SAL_CALL 
		disposing( const EventObject& rSource ) 
			throw(RuntimeException);

	// XPropertyChangeListener
	virtual void SAL_CALL 
		propertyChange( const PropertyChangeEvent& rEvt ) 
			throw(RuntimeException) = 0;

	// XLinguServiceEventBroadcaster
    virtual sal_Bool SAL_CALL 
		addLinguServiceEventListener( 
				const Reference< XLinguServiceEventListener >& rxListener ) 
			throw(RuntimeException);
    virtual sal_Bool SAL_CALL 
		removeLinguServiceEventListener( 
				const Reference< XLinguServiceEventListener >& rxListener ) 
			throw(RuntimeException);

	// non UNO functions
	void 	AddAsPropListener();
	void	RemoveAsPropListener();
	void	LaunchEvent( const LinguServiceEvent& rEvt );
	
	const Sequence< OUString > &
			GetPropNames() const	{ return aPropNames; }
	const Reference< XPropertySet >	&
			GetPropSet() const		{ return xPropSet; }
	const Reference< XInterface > &
			GetEvtObj() const		{ return xMyEvtObj; }
};


///////////////////////////////////////////////////////////////////////////


class PropertyHelper_Spell :
	public PropertyChgHelper
{
	// default values
	sal_Bool	bIsGermanPreReform;
	sal_Bool	bIsIgnoreControlCharacters;
	sal_Bool	bIsUseDictionaryList;
	sal_Bool	bIsSpellUpperCase;
	sal_Bool	bIsSpellWithDigits;
	sal_Bool	bIsSpellCapitalization;

	// return values, will be set to default value or current temporary value
	sal_Bool	bResIsGermanPreReform;
	sal_Bool	bResIsIgnoreControlCharacters;
	sal_Bool	bResIsUseDictionaryList;
	sal_Bool	bResIsSpellUpperCase;
	sal_Bool	bResIsSpellWithDigits;
	sal_Bool	bResIsSpellCapitalization;

	
	// disallow use of copy-constructor and assignment-operator
	PropertyHelper_Spell( const PropertyHelper_Spell & );
	PropertyHelper_Spell & operator = ( const PropertyHelper_Spell & );
		
	void	SetDefault();

public:
	PropertyHelper_Spell(
			const Reference< XInterface > &rxSource,
			Reference< XPropertySet > &rxPropSet );
	virtual ~PropertyHelper_Spell();

	// XPropertyChangeListener
	virtual void SAL_CALL 
		propertyChange( const PropertyChangeEvent& rEvt ) 
			throw(RuntimeException);
	
	void	SetTmpPropVals( const PropertyValues &rPropVals );
	
	sal_Bool	IsGermanPreReform() const			{ return bResIsGermanPreReform; }
	sal_Bool	IsIgnoreControlCharacters() const	{ return bResIsIgnoreControlCharacters; }
	sal_Bool	IsUseDictionaryList() const			{ return bResIsUseDictionaryList; }
	sal_Bool	IsSpellUpperCase() const			{ return bResIsSpellUpperCase; }
	sal_Bool	IsSpellWithDigits() const			{ return bResIsSpellWithDigits; }
	sal_Bool	IsSpellCapitalization() const		{ return bResIsSpellCapitalization; }
};

}

///////////////////////////////////////////////////////////////////////////

#endif

